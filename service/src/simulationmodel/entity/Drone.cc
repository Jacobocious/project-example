#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "BfsStrategy.h"
#include "DataDashboard.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "Package.h"
#include "SimulationModel.h"
#include "SpinDecorator.h"

Drone::Drone(const JsonObject& obj) : IEntity(obj) { available = true; }

Drone::~Drone() {
  if (toPackage) delete toPackage;
  if (toFinalDestination) delete toFinalDestination;
}

void Drone::getNextDelivery() {
  if (model && model->scheduledDeliveries.size() > 0) {
    package = model->scheduledDeliveries.front();
    model->scheduledDeliveries.pop_front();

    if (package) {
      std::string message = getName() + " heading to: " + package->getName();
      notifyObservers(message);
      available = false;
      pickedUp = false;

      Vector3 packagePosition = package->getPosition();
      Vector3 finalDestination = package->getDestination();

      toPackage = new BeelineStrategy(position, packagePosition);

      std::string strat = package->getStrategyName();
      if (strat == "astar") {
        toFinalDestination = new JumpDecorator(new AstarStrategy(
            packagePosition, finalDestination, model->getGraph()));
      } else if (strat == "dfs") {
        toFinalDestination =
            new SpinDecorator(new JumpDecorator(new DfsStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "bfs") {
        toFinalDestination =
            new SpinDecorator(new SpinDecorator(new BfsStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else if (strat == "dijkstra") {
        toFinalDestination =
            new JumpDecorator(new SpinDecorator(new DijkstraStrategy(
                packagePosition, finalDestination, model->getGraph())));
      } else {
        toFinalDestination =
            new BeelineStrategy(packagePosition, finalDestination);
      }
    }
  }
}

void Drone::update(double dt) {
  static std::ostringstream oss;

  if (available) {
    getNextDelivery();

    if (toPackage) {
      initializeTrip();
      package->initializeTrip();
      storeId();
      storeType();
      package->storeTypeAndId(getTripId());
      sendStrategy();
      package->storeMovingStrat();
    }
  }

  if (toPackage) {
    toPackage->move(this, dt);
    updateTrip(dt);
    package->updateTrip(dt);

    if (toPackage->isCompleted()) {
      delete toPackage;
      toPackage = nullptr;
      pickedUp = true;

      std::string pickedUpMsg =
          name + " has picked up " + package->getName() + ".";
      notifyObservers(pickedUpMsg);
    }
  } else if (toFinalDestination) {
    toFinalDestination->move(this, dt);
    updateTrip(dt);
    package->updateTrip(dt);

    if (package && pickedUp) {
      package->setPosition(position);
      package->setDirection(direction);
    }

    if (toFinalDestination->isCompleted()) {
      delete toFinalDestination;
      toFinalDestination = nullptr;

      storeDestination();
      package->storeDestination();

      sendDroneData();
      package->sendPackageData();

      std::string droppedOffMsg = name + " has dropped off " +
                                  package->getName() + " at the destination.";
      notifyObservers(droppedOffMsg);

      package->handOff();
      package = nullptr;
      available = true;
      pickedUp = false;
    }
  }
}

void Drone::sendStrategy() {
  entityStorage.strategy = package ? package->getStrategyName() : "N/A";
}

void Drone::storeId() {
  entityStorage.tripID = DataDashboard::getInstance()->getTripID();
}

int Drone::getTripId() { return entityStorage.tripID; }

void Drone::storeType() { entityStorage.entityType = "DRONE"; }

void Drone::sendDroneData() {
  DataDashboard::getInstance()->addEntity(entityStorage);
}

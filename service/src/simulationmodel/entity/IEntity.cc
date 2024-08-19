#include "IEntity.h"

#include "DataDashboard.h"

IEntity::IEntity() {
  static int currentId = 0;
  id = currentId;
  currentId++;
}

IEntity::IEntity(const JsonObject& details) : IEntity() {
  this->details = details;
  JsonArray pos(details["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(details["direction"]);
  direction = {dir[0], dir[1], dir[2]};
  if (details.contains("color")) {
    std::string col = details["color"];
    color = col;
  }
  std::string n = details["name"];
  name = n;
  speed = details["speed"];
}

IEntity::~IEntity() {}

void IEntity::linkModel(SimulationModel* model) { this->model = model; }

int IEntity::getId() const { return id; }

Vector3 IEntity::getPosition() const { return position; }

Vector3 IEntity::getDirection() const { return direction; }

const JsonObject& IEntity::getDetails() const { return details; }

std::string IEntity::getColor() const { return color; }

std::string IEntity::getName() const { return name; }

double IEntity::getSpeed() const { return speed; }

void IEntity::setPosition(Vector3 pos_) { position = pos_; }

void IEntity::setDirection(Vector3 dir_) { direction = dir_; }

void IEntity::setColor(std::string col_) { color = col_; }

void IEntity::rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

std::string IEntity::formatPosition(const Vector3& position) {
  std::ostringstream oss;
  oss << position;
  return oss.str();
}

void IEntity::initializeTrip() {
  currTripNum++;
  entityStorage.entityID = getId();
  entityStorage.entityName = getName();
  entityStorage.entryNum = DataDashboard::getInstance()->getEntryNum();
  entityStorage.origin = formatPosition(getPosition());
  entityStorage.tripNum = currTripNum;

  // clear distance and time for each trip
  entityStorage.distanceTraveled = 0;
  entityStorage.timeTaken = 0;
}

void IEntity::updateTrip(double dt) {
  entityStorage.distanceTraveled += (dt * getSpeed());
  entityStorage.timeTaken += dt;
}

void IEntity::storeDestination() {
  entityStorage.destination = formatPosition(getPosition());
}

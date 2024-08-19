#include "DataDashboard.h"

#include <iomanip>
namespace fs = std::filesystem;

DataDashboard* DataDashboard::instance = nullptr;

DataDashboard* DataDashboard::getInstance() {
  if (instance == nullptr) {
    instance = new DataDashboard();
  }
  return instance;
}

void DataDashboard::addEntity(const EntityData& addTrip) {
  if (tripData.find(addTrip.entryNum) == tripData.end()) {
    tripData[addTrip.entryNum] = addTrip;
  }
}

int DataDashboard::getTripID() {
  u_trip_ID++;
  return (u_trip_ID - 1);
}

int DataDashboard::getEntryNum() {
  u_entry_ID++;
  return (u_entry_ID - 1);
}

void DataDashboard::exportToCSV(const std::string& filename) const {
  std::ofstream outFile(filename);

  if (!outFile.is_open()) {
    std::cerr << "Failed to open the file: " << filename << std::endl;
    return;
  }
  outFile << "EntityName,EntityType,EntryNum,EntityID,TripUID,TripNum,"
             "PathStrategy,Distance "
             "(miles),Time (seconds),Origin,Destination\n";

  for (const auto& entry : tripData) {
    const EntityData& tempEntity = entry.second;
    double distMiles = tempEntity.distanceTraveled / 1625.0;
    outFile << tempEntity.entityName << "," << tempEntity.entityType << ","
            << tempEntity.entryNum << "," << tempEntity.entityID << ","
            << tempEntity.tripID << "," << tempEntity.tripNum << ","
            << tempEntity.strategy << "," << std::fixed << std::setprecision(4)
            << distMiles << "," << tempEntity.timeTaken << ","
            << "\"" << tempEntity.origin << "\","
            << "\"" << tempEntity.destination << "\""
            << "\n";
  }
  outFile.close();
}

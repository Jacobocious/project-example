#ifndef DATA_DASHBOARD_H_
#define DATA_DASHBOARD_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

#include "IEntity.h"
#include "entity/EntityData.h"
#include "math/vector3.h"
#include "util/json.h"

/**
 * @brief DataDashboard class which collects simulation data--
 * uses a singleton pattern.
 */
class DataDashboard {
 private:
  /**
   * @brief DataDashboard constructor.
   */
  DataDashboard() {}

  /**
   * @brief Private instance of the DataDashboard class.
   */
  static DataDashboard* instance;

  std::map<int, EntityData> tripData;
  int u_trip_ID = 0;
  int u_entry_ID = 0;

 public:
  /**
   * @brief Returns the instance of the DataDashboard singleton.
   */
  static DataDashboard* getInstance();

  /**
   * Prevents cloning of DataDashboard.
   */
  DataDashboard(DataDashboard const&) = delete;

  /**
   * Prevents assignment of DataDashboard.
   */
  void operator=(DataDashboard const&) = delete;

  /**
   * @brief Destructor for the entire DataDashboard.
   */
  ~DataDashboard() {}

  /**
   * @brief Adds a drone to the DataDashboard's drone mapping.
   * @param addTrip The trip to be added to DataDashboard.
   */
  void addEntity(const EntityData& addTrip);

  /**
   * @brief Returns a unique trip ID for usage.
   */
  int getTripID();

  /**
   * @brief Returns unique entry number.
   */
  int getEntryNum();

  /**
   * @brief Calculates and exports the trip data to a CSV file.
   * @param filename The file to write the data to.
   */
  void exportToCSV(const std::string& filename) const;
};

#endif

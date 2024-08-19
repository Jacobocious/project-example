#ifndef ENTITY_DATA_H_
#define ENTITY_DATA_H_

#include <string>

/**
 * @brief Struct containing entity data used for data processing.
 */
struct EntityData {
  std::string entityName;
  std::string entityType;
  int entryNum;
  int entityID;
  int tripID;
  int tripNum;
  std::string strategy;
  double distanceTraveled = 0;
  double timeTaken = 0;
  std::string origin;
  std::string destination = "";

  /**
   * @brief Default constructor for entity data.
   */
  EntityData()
      : entityName(""),
        entityType(""),
        entryNum(0),
        entityID(0),
        tripNum(0),
        strategy(""),
        distanceTraveled(0.0),
        timeTaken(0.0),
        origin(""),
        destination("") {}

  /**
   * @brief Custom constructor for entity data.
   * @param name Name of entity.
   * @param eType Type of entity.
   * @param num The entry number of the entity behavior.
   * @param id The ID of the entity.
   * @param trip The current trip number for the entity.
   * @param strat The strategy used by the entity to travel.
   * @param distance The distance the entity goes.
   * @param time The time taken for a given behavior for the entity.
   * @param orig The starting point of the entity for some measurement.
   * @param dest The destination the entity arrives at.
   */
  EntityData(const std::string& name, const std::string& eType, int num, int id,
             int trip, const std::string& strat, double distance, double time,
             const std::string& orig, const std::string& dest)
      : entityName(name),
        entityType(eType),
        entryNum(num),
        entityID(id),
        tripNum(trip),
        strategy(strat),
        distanceTraveled(distance),
        timeTaken(time),
        origin(orig),
        destination(dest) {}
};

#endif

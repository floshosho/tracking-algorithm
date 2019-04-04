/*!
 * @file
 */

// Copyright 2018 SPACEHAUC Groundstation Team, David Baumann

#ifndef DISH_H_
#define DISH_H_

#include <list>
#include "../include/Satellite.h"
#include "../include/passpredict.h"
#include "../libsgp4/CoordGeodetic.h"
#include "../libsgp4/CoordTopocentric.h"
#include "../libsgp4/Observer.h"
#include "../libsgp4/SGP4.h"

enum DISH_STATUS {
  NO_TARGET,
  ON_TARGET,
  OFF_TARGET,
  WAITING,
  MOVING,
  TRANSMITING
};

class Dish {
 private:
  Observer *mpLocation;
  Satellite *mpTarget;
  SGP4 *mpSGP4;
  DISH_STATUS mCurrentStatus;
  void move(double azimuth, double elevation);

 public:
  Dish(double latitude, double longitude, double elevation)
      : mpLocation(new Observer(latitude, longitude, elevation)),
        mpTarget(nullptr),
        mpSGP4(nullptr),
        mCurrentStatus(NO_TARGET) {}
  ~Dish();
  void transmit();
  void track();
  DISH_STATUS status() const { return mCurrentStatus; }
  void wait();
  void moveToNextAppearance();
  void setTarget(const Satellite &target);
  bool targetVisible() const;
  std::list<PassDetails_t> generatePassList();
  std::list<PassDetails_t> generatePassList(const DateTime &start_time,
                                            const DateTime &end_time);
};

#endif  // DISH_H_

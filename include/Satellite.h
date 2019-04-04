/*!
 * @file
 */

// Copyright 2018 SPACEHAUC Groundstation Team, David Baumann

#ifndef SATELLITE_H_
#define SATELLITE_H_

#include <string>
#include "../libsgp4/CoordGeodetic.h"
#include "../libsgp4/CoordTopocentric.h"
#include "../libsgp4/Observer.h"
#include "../libsgp4/SGP4.h"

class Satellite {
 private:
  Tle mTle;
  std::string mName;

 public:
  explicit Satellite(const Tle &tle) : mTle(tle) {}
  Satellite(const std::string &satelliteName,
            const std::string &tleLine1,
            const std::string &tleLine2)
            : mTle(Tle(satelliteName, tleLine1, tleLine2)) {}
  Tle getTle() const { return mTle; }
  Satellite operator = (const Satellite &rvalue) {
    mTle = rvalue.mTle;
    return *this;
  }
};

#endif  // SATELLITE_H_

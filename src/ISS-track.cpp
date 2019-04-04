// Copyright 2018 SPACEHAUC Groundstation Team, David Baumann
// demo file for tracking ISS

#include <iostream>
#include <string>
#include "../include/Dish.h"
#include "../include/Satellite.h"
#include "../libsgp4/CoordGeodetic.h"
#include "../libsgp4/CoordTopocentric.h"
#include "../libsgp4/Observer.h"
#include "../libsgp4/SGP4.h"

using std::string;
using std::cout;
using std::endl;

// Groundstation Location Data
const double LATITUDE = 42.649558;    // degrees (N)
const double LONGITUDE = -71.316263;  // degrees (W)
const double ELEVATION = 0.040;       // km

// Satellite Data (Using ISS Data for now)
const char NAME[] = "ISS (CSUNSAT1)             ";
const char TLE1[] =
    "1 36122U 09072B   19083.69841103 -.00000043  00000-0 -32199-5 0  9993";
const char TLE2[] =
    "2 36122 100.0745  90.1227 0007726 178.3631 181.7480 13.16348746445561";

/**
* notes:
* 2 states:
*  1) satellite in view
*      - follow through sky
*      - allow data transmission
*  2) satellite out of view
*      - move to next visible location
*      - wait until visible
*/

int main() {
  Dish groundstation(LATITUDE, LONGITUDE, ELEVATION);
  Satellite spacehauc(NAME, TLE1, TLE2);
  groundstation.setTarget(spacehauc);

  while (true) {
    // TODO(djbaumann): maybe add a user interface in a separate thread
    // for controlling the dish & sending commands
    if (groundstation.targetVisible() && groundstation.status() == ON_TARGET) {
      cout << "1" << endl;
      groundstation.transmit();
    } else if (groundstation.targetVisible()) {
      cout << "2" << endl;
      groundstation.track();
    } else if (!groundstation.targetVisible() &&
               groundstation.status() == WAITING) {
      cout << "3" << endl;
      groundstation.wait();
      groundstation.moveToNextAppearance();
    } else {
      cout << "4" << endl;
      groundstation.moveToNextAppearance();
    }
  }
  return 0;
}

// int main() {
//   CoordGeodetic geo(LATITUDE, LONGITUDE, ELEVATION);
//   Tle tle(NAME, TLE1, TLE2);
//   SGP4 sgp4(tle);

//   std::cout << tle << std::endl;

//   /*
//    * generate 7 day schedule
//    */
//   DateTime start_date = DateTime::Now(true);
//   DateTime end_date(start_date.AddDays(7.0));

//   std::list<PassDetails_t> pass_list;

//   std::cout << "Start time: " << start_date << std::endl;
//   std::cout << "End time  : " << end_date << std::endl << std::endl;

//   /*
//    * generate passes
//    */
//   pass_list = GeneratePassList(geo, &sgp4, start_date, end_date, 180);

//   if (pass_list.begin() == pass_list.end()) {
//     std::cout << "No passes found" << std::endl;
//   } else {
//     std::stringstream ss;

//     ss << std::right << std::setprecision(1) << std::fixed;

//     std::list<PassDetails_t>::const_iterator itr = pass_list.begin();
//     do {
//       ss << "AOS: " << itr->aos << ", LOS: " << itr->los
//          << ", Max El: " << std::setw(4)
//          << Util::RadiansToDegrees(itr->max_elevation)
//          << ", Duration: " << (itr->los - itr->aos) << std::endl;
//     } while (++itr != pass_list.end());

//     std::cout << ss.str();
//   }

//   return 0;
// }

// int old_main() {
// // Assume dish is on top of LoCSST, altitude roughly 40 meters (0.040km)
//     Observer groundstation_location(42.649558, -71.316263, 0.040);
//     Tle iss_tle = Tle("ISS (ZARYA)             ",
//         "1 25544U 98067A   18072.01686465  .00002475  00000-0  44458-4 0
//         9991",
//         "2 25544  51.6420 138.1426 0002141 176.4814 246.1618
//         15.54223596103569");
//     SGP4 sgp4(iss_tle);

//     cout << iss_tle << endl;

//     for (int i = 0; i < 10; ++i) {
//         /*
//          * current time
//          */
//         DateTime now = DateTime::Now(true);
//         /*
//          * calculate satellite position
//          */
//         Eci eci = sgp4.FindPosition(now);
//         /*
//          * get look angle for observer to satellite
//          */
//         CoordTopocentric topo = groundstation_location.GetLookAngle(eci);
//         /*
//          * convert satellite position to geodetic coordinates
//          */
//         CoordGeodetic geo = eci.ToGeodetic();

//         std::cout << now << " " << topo << " " << geo << std::endl;
//     }

//     return 0;
// }

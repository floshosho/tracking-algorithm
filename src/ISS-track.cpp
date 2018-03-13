// demo file for tracking ISS

#include <iostream>
#include "../libsgp4/CoordTopocentric.h"
#include "../libsgp4/CoordGeodetic.h"
#include "../libsgp4/Observer.h"
#include "../libsgp4/SGP4.h"
#include "../include/Dish.h"
#include "../include/Satellite.h"

using std::cout;
using std::endl;


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
    Dish groundstation;
    Satellite spacehauc;

    while(true) {
        if (spacehauc.isVisibleFrom(groundstation)) {
            groundstation.transmit();
            groundstation.track();
        } else if (groundstation.status() == isWaiting) {
            groundstation.wait();
        } else {
            groundstation.moveToNext();
        }
    }

    return 0;
}






int old_main() {
// Assume dish is on top of LoCSST, altitude roughly 40 meters (0.040km)
    Observer groundstation_location(42.649558, -71.316263, 0.040);
    Tle iss_tle = Tle("ISS (ZARYA)             ",
        "1 25544U 98067A   18072.01686465  .00002475  00000-0  44458-4 0  9991",
        "2 25544  51.6420 138.1426 0002141 176.4814 246.1618 15.54223596103569");
    SGP4 sgp4(iss_tle);


    cout << iss_tle << endl;

    for (int i = 0; i < 10; ++i) {
        /*
         * current time
         */
        DateTime now = DateTime::Now(true);
        /*
         * calculate satellite position
         */
        Eci eci = sgp4.FindPosition(now);
        /*
         * get look angle for observer to satellite
         */
        CoordTopocentric topo = groundstation_location.GetLookAngle(eci);
        /*
         * convert satellite position to geodetic coordinates
         */
        CoordGeodetic geo = eci.ToGeodetic();

        std::cout << now << " " << topo << " " << geo << std::endl;
    }

    return 0;
}
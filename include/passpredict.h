/*
 * Copyright 2013 Daniel Warner <contact@danrw.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This code was modified by David Baumann for the SPACEHAUC groundstation
 * project in 2018.
 *  Changes:
 *    * Modified Example code to work as header with implementation file
 *    * SGP4 pass by reference changed to pointers
 *    * Added function for finding next pass instead of pass list (FindNextPass)
 *    * Struct PassDetails renamed "struct passDetails" and typedefed to
 *      "PassDetails_t"
 */

#ifndef PASSPREDICT_H_
#define PASSPREDICT_H_

#include <list>
#include "../libsgp4/CoordGeodetic.h"
#include "../libsgp4/SGP4.h"

typedef struct {
  DateTime aos;
  DateTime los;
  double max_elevation;
} PassDetails_t;

double FindMaxElevation(const CoordGeodetic &user_geo,
                        SGP4 *sgp4,
                        const DateTime &aos,
                        const DateTime &los);

DateTime FindCrossingPoint(const CoordGeodetic &user_geo,
                          SGP4 *sgp4,
                          const DateTime &initial_time1,
                          const DateTime &initial_time2,
                          bool finding_aos);

std::list<PassDetails_t> GeneratePassList(const CoordGeodetic &user_geo,
                                          SGP4 *sgp4,
                                          const DateTime &start_time,
                                          const DateTime &end_time,
                                          const int time_step);

PassDetails_t FindNextPass(const CoordGeodetic &user_geo,
                          SGP4 *sgp4,
                          const int time_step);

#endif  // PASSPREDICT_H_

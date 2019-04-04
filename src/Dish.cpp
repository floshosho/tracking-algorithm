// Copyright 2018 SPACEHAUC Groundstation Team, David Baumann

#include "../include/Dish.h"

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../libsgp4/Util.h"
#include "../include/positionDish.h"

Dish::~Dish() {
  if (mpLocation != nullptr) {
    delete mpLocation;
  }
  if (mpTarget != nullptr) {
    delete mpTarget;
  }
  if (mpSGP4 != nullptr) {
    delete mpSGP4;
  }
}

void Dish::transmit() {
  // stub, do nothing
}

void Dish::track() {
  // run in new thread?
  // follows satellite through the sky
  Eci eci = mpSGP4->FindPosition(DateTime::Now(true));
  CoordTopocentric topo = mpLocation->GetLookAngle(eci);
  int stream = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  // MAY NEED TO CHANGE PORT DEPENDING ON CONNECTOR!!
  if (stream < 0) {
    perror("Unable to open UART");
    exit(1);
  }

  struct termios options;
  tcgetattr(stream, &options);
  options.c_cflag = B2400 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(stream, TCIFLUSH);
  tcsetattr(stream, TCSANOW, &options);
  // move there
  // std::cout << "azimuth :" << topo.azimuth
  //      << "elevation :" << topo.elevation << "\n";
  PA(stream, topo.elevation);
  PB(stream, topo.azimuth);
  close(stream);
}

void Dish::wait() {
  // TODO(djbaumann): update states
  // sleep for 1 second
  sleep(1);
}

void Dish::moveToNextAppearance() {
  struct termios options;
  const int TIME_STEP = 60;  // seconds
  // figure out where next appearance will be
  PassDetails_t nextPass =
      FindNextPass(mpLocation->GetLocation(), mpSGP4, TIME_STEP);

  Eci eci = mpSGP4->FindPosition(nextPass.aos);
  CoordTopocentric topo = mpLocation->GetLookAngle(eci);
  int stream = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  // MAY NEED TO CHANGE PORT DEPENDING ON CONNECTOR!
  if (stream < 0) {
    perror("Unable to open UART");
    exit(1);
  }

  tcgetattr(stream, &options);
  options.c_cflag = B2400 | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(stream, TCIFLUSH);
  tcsetattr(stream, TCSANOW, &options);
  // move there
  PA(stream, topo.elevation);
  PB(stream, topo.azimuth);
  mCurrentStatus = WAITING;
  close(stream);
}

void Dish::setTarget(const Satellite &target) {
  if (mpTarget != nullptr) {
    delete mpTarget;
  }
  mpTarget = new Satellite(target);
  if (mpSGP4 != nullptr) {
    delete mpSGP4;
  }
  mpSGP4 = new SGP4(mpTarget->getTle());
  mCurrentStatus = OFF_TARGET;
}

bool Dish::targetVisible() const {
  // visible if above horizon
  return (mpLocation->GetLookAngle(mpSGP4->FindPosition(DateTime::Now(true)))
              .elevation > 0.0);
}

void Dish::move(double azimuth, double elevation) {
  /*// somehow move to the given position
  mCurrentStatus = MOVING;
  std::cout << "Moving to: Azimuth = " << Util::RadiansToDegrees(azimuth)
            << " deg.  Elevation = " << Util::RadiansToDegrees(elevation)
            << " deg." << std::endl;
            */
}

std::list<PassDetails_t> Dish::generatePassList() {
  const int TIME_STEP = 60;
  return GeneratePassList(mpLocation->GetLocation(),
                          mpSGP4,
                          DateTime::Now(true),
                          DateTime::Now(true).AddDays(7.0),
                          TIME_STEP);
}
std::list<PassDetails_t> Dish::generatePassList(const DateTime &start_time,
                                                const DateTime &end_time) {
  const int TIME_STEP = 60;
  return GeneratePassList(mpLocation->GetLocation(),
                          mpSGP4,
                          start_time,
                          end_time,
                          TIME_STEP);
}

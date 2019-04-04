// Copyright 2018 SPACEHAUC Groundstation Team, David Baumann
/*
  Dish Positioner
  USB to Serial
  This code was produced as part of the UMass Lowell
  SPACEHAUC project. It is open for use/modificaton by anyone
  for any purpose.
*/

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <string>
#include "../include/positionDish.h"

// Change antenna elevation
int PA(int stream, double degrees) {
  int test = degrees * (180 / M_PI);
  // std::cout << "elevation :" << degrees * (180 / M_PI)
  //           << "degrees:: "<< degrees << std::endl;
  int i = 0;
  char cmd[] = {HEX_P, HEX_A, HEX_SPACE, 0, 0, 0, 0};
  int temp;
  if (test > 100) {
    temp = test / 100;
    cmd[3] = to_Hex(temp);
    test = test % 100;
    i = 1;
  }
  temp = (test / 10);
  cmd[i + 3] = to_Hex(temp);
  i++;
  temp = (test % 10);
  cmd[i + 3] = to_Hex(temp);
  i++;
  cmd[i + 3] = HEX_CR;
  /* 
  for (i = 0; i < 6; i++) {
    std::cout << "0x" << std::hex << (int)cmd[i] << std::endl;
  } 
  */
  int count = write(stream, cmd, i + 4);
  if (count < 0) {
    perror("Error writing to positioner");
    return 1;
  }

  return 0;
}


// Change antenna azimuth
int PB(int stream, double degrees) {
  int test = degrees * (180 / M_PI);
  // std::cout << "azimuth:: " << degrees * (180 / M_PI)
  //           << "degrees:: "<< degrees << std::endl;
  int i = 0;
  char cmd[] = {HEX_P, HEX_B, HEX_SPACE, 0, 0, 0, 0};
  int temp;
  if (test > 100) {
    temp = (test / 100);
    //  std::cout << "test =" << temp;
    cmd[3] = to_Hex(temp);
    test = (test % 100);
    i = 1;
  }
  temp = (test / 10);
  cmd[i + 3] = to_Hex(temp);
  i++;
  temp = (test % 10);
  //  std::cout << temp;
  cmd[i + 3] = to_Hex(temp);
  i++;
  cmd[i + 3] = HEX_CR;
  /*
  for (i = 0; i < 6; i++) {
    std::cout << "0x" << std::hex << (int)cmd[i] << std::endl;
  }
  */
  int count = write(stream, cmd, i + 4);
  if (count < 0) {
    perror("Error writing to positioner");
    return 1;
  }

  return 0;
}

char to_Hex(int integer) {
  switch (integer) {
    case 0: return 0x30;
    case 1: return 0x31;
    case 2: return 0x32;
    case 3: return 0x33;
    case 4: return 0x34;
    case 5: return 0x35;
    case 6: return 0x36;
    case 7: return 0x37;
    case 8: return 0x38;
    case 9: return 0x39;
    default: std::cout << "error, exiting." << std::endl;
      exit(1);
  }
  return 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Dylan Bruner                                              */
/*    Created:      Sun Dec 04 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftFrontMotor       motor         12              
// LeftBackMotor        motor         13              
// RightBackMotor       motor         14              
// RightFrontMotor      motor         15              
// Inert20              inertial      20              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  initDrive();

  //Main =========================================
  setBrakeMode(hold);

  turn(90);
  drive(2);
  turn(90);
  drive(2);

  competition comp;
  comp.drivercontrol(opcontrol);
  // comp.autonomous(autonomous);
}
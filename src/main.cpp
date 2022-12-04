/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\brune                                            */
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

void opcontrol(){
  while (true){
    tank(Controller1.Axis3.value(), Controller1.Axis2.value());
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  initDrive();

  //Main =========================================
  setBrakeMode(hold);

  // turn(360, 70);

  testAuton();
  opcontrol();
}
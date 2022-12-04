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
#include "vex_global.h"
#include "pid-drivetrain.h"
// #include "pid-motor.h"

using namespace vex;


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  PidDriveTrain myDrive(&LeftBackMotor, &LeftFrontMotor, &RightBackMotor, &RightFrontMotor, &Inert20);

  myDrive.turn(90);

  Brain.Screen.printAt(10, 10, "Finished!");
}
// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Flywheel             motor_group   1, 2            
// LittleNut            digital_out   A               
// BigNut               digital_out   B               
// RollerAndBtmIntake   motor         3               
// UpperIntake          motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----
     
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Dylan Bruner                                              */
/*    Created:      Sun Dec 04 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

bool flywheelOn = false;

double flywheelVoltage = 7.0;
MotorPid flywheelPid;

//TODO
//Get hold mode working *better*
//Use .pressing()
//Splut up autons
//Maybe work out some controller key binding class/system

//Quick and ugly cause i dont know how lambda works
void flywheelButtonPressed(){
  flywheelOn = !flywheelOn;
  if (flywheelOn){
    Flywheel.spin(forward, flywheelVoltage, volt);
  } else {
    Flywheel.stop();
  }
}


// Figure out how th to write lambda functions!!
void buttonXPressed(){
  flywheelVoltage += 0.2;
}

void buttonBPressed(){
  flywheelVoltage -= 0.2;
}

//Display cache
double flywheelVoltageCache = -1;
void displayControllerStuff(){
  bool hasChange = false;
  if (flywheelVoltageCache != flywheelVoltage){
    flywheelVoltageCache = flywheelVoltage;
    hasChange = true;
  } else if (flywheelPid.lastVoltage != flywheelPid.currentVoltage){
    hasChange = true;
  }

  if (hasChange){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);

    // Display voltage
    Controller1.Screen.print("Voltage: ");
    Controller1.Screen.print(flywheelVoltage);
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Flywheel: ");
    Controller1.Screen.print(flywheelPid.currentVoltage);
  }
}

void wpAuton(){
  // Do Roller =================================
  drive(-0.15);
  UpperIntake.setVelocity(100, percent);
  UpperIntake.spinFor(forward, -65, degrees);

  // Shoot Disk - align with goal
  Flywheel.setVelocity(100, percent);
  // Flywheel.spin(forward, 7.85, volt);
  Flywheel.spin(forward, 6, volt);
  drive(0.20);

  // turn(20); - High Goal
  turn(-90);

  //Shoot disk - shoot disk
  wait(2, seconds);
  UpperIntake.setVelocity(50, percent);
  RollerAndBtmIntake.setVelocity(50, percent);
  UpperIntake.spin(forward);
  RollerAndBtmIntake.spin(forward);

  //After first shoots give flywheel time to spin up for second
  wait(2.2, seconds);
  // Flywheel.spin(forward, 9, volt);
  Flywheel.spin(forward, 7, volt); //Low goal


  //Shoot second
  RollerAndBtmIntake.spin(forward);
  UpperIntake.spin(forward);

  wait(5, seconds);


  Flywheel.stop();
}

void driver(){
  // Controller1.ButtonR1.pressed(flywheelButtonPressed);
  Controller1.ButtonX.pressed(buttonXPressed);
  Controller1.ButtonB.pressed(buttonBPressed);

  flywheelPid.motor = &Flywheel;
  flywheelPid.setTargetRpm(500);

  while (true){
    tank(Controller1.Axis3.position(), Controller1.Axis2.position());

    if (Controller1.ButtonR2.pressing()){
      if (flywheelOn){
        flywheelOn = false;
        Flywheel.stop();
      } else {
        flywheelOn = true;
        // Flywheel.spin(forward, flywheelVoltage, volt);
      }

      wait(200, msec); 
    }

    if (flywheelOn){
      flywheelPid.doTick();
    }

    //Controller display & Voltage Control
    displayControllerStuff();

    // Intake ============================================================
    if (Controller1.ButtonL1.pressing()){
      UpperIntake.setVelocity(100, percent);
      UpperIntake.spin(forward);
    } else if (!Controller1.ButtonUp.pressing()) {UpperIntake.stop();}

    if (Controller1.ButtonL2.pressing()){
      RollerAndBtmIntake.setVelocity(100, percent);
      RollerAndBtmIntake.spin(forward);
    } else if (!Controller1.ButtonDown.pressing()){RollerAndBtmIntake.stop();}

    if (Controller1.ButtonUp.pressing()){
      UpperIntake.setVelocity(100, percent);
      UpperIntake.spin(reverse);
    } else if (!Controller1.ButtonL1.pressing()){UpperIntake.stop();}

    if (Controller1.ButtonDown.pressing()){
      RollerAndBtmIntake.setVelocity(100, percent);
      RollerAndBtmIntake.spin(reverse);
    } else if (!Controller1.ButtonL2.pressing()){RollerAndBtmIntake.stop();}

    // Endgame ========================================================
    if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing()
        && Controller1.ButtonX.pressing() && Controller1.ButtonY.pressing()
        && Controller1.ButtonUp.pressing() && Controller1.ButtonDown.pressing()
        && Controller1.ButtonLeft.pressing() && Controller1.ButtonRight.pressing()){
      LittleNut.set(true);
      BigNut.set(true);

      wait(200, msec);

      BigNut.set(false);
      LittleNut.set(false);
    }
  }
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  initDrive();

  //Main =========================================
  setBrakeMode(hold);

  LittleNut.set(false);
  BigNut.set(false);

  // driver();

  // Controller1.ButtonR1.pressed(flywheelButtonPressed);
  Controller1.ButtonX.pressed(buttonXPressed);
  Controller1.ButtonB.pressed(buttonBPressed);


  competition comp;
  comp.drivercontrol(driver);
  comp.autonomous(wpAuton);
}
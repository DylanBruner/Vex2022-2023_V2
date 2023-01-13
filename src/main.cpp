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

double flywheelVoltage = 12.0;

//TODO
//Get hold mode working

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
  }

  if (hasChange){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);

    // Display voltage
    Controller1.Screen.print("Voltage: ");
    Controller1.Screen.print(flywheelVoltage);
  }
}

void driver(){
  Controller1.ButtonR1.pressed(flywheelButtonPressed);
  Controller1.ButtonX.pressed(buttonXPressed);
  Controller1.ButtonB.pressed(buttonBPressed);

  while (true){
    tank(Controller1.Axis3.position(), Controller1.Axis2.position());

    if (flywheelOn){
      Flywheel.spin(forward, flywheelVoltage, volt);
    } else {
      Flywheel.stop();
    }

    //Controller display & Voltage Control
    displayControllerStuff();

    // Intake ============================================================
    if (Controller1.ButtonL1.pressing()){
      UpperIntake.setVelocity(100, percent);
      UpperIntake.spin(forward);
    } else {UpperIntake.stop();}

    if (Controller1.ButtonL2.pressing()){
      RollerAndBtmIntake.setVelocity(100, percent);
      RollerAndBtmIntake.spin(forward);
    } else {RollerAndBtmIntake.stop();}

    if (Controller1.ButtonUp.pressing()){
      UpperIntake.setVelocity(100, percent);
      UpperIntake.spin(reverse);
    } else {UpperIntake.stop();}

    if (Controller1.ButtonDown.pressing()){
      RollerAndBtmIntake.setVelocity(100, percent);
      RollerAndBtmIntake.spin(reverse);
    } else {RollerAndBtmIntake.stop();}

    // Endgame ========================================================
    if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing()
        && Controller1.ButtonX.pressing() && Controller1.ButtonY.pressing()
        && Controller1.ButtonUp.pressing() && Controller1.ButtonDown.pressing()
        && Controller1.ButtonLeft.pressing() && Controller1.ButtonRight.pressing()){
      LittleNut.set(true);
      BigNut.set(true);
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

  driver();

  // competition comp;
  // comp.drivercontrol(driver);
  // comp.autonomous(autonomous);
}
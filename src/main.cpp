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
#include <sys/types.h>

using namespace vex;

bool flywheelOn = false;
bool tableMode  = false;

int flywheelRPM = 350;
MotorPid flywheelPid;

//Quick and ugly cause i dont know how lambda works
void flywheelButtonPressed(){
  flywheelOn = !flywheelOn;
  if (flywheelOn){
    Flywheel.spin(forward, flywheelRPM, volt);
  } else {
    Flywheel.stop();
  }
}


// Figure out how th to write lambda functions!!
void buttonXPressed(){
  flywheelRPM += 10;
}

void buttonBPressed(){
  flywheelRPM -= 10;
}

//Display cache
double flywheelVoltageCache = -1;
void displayControllerStuff(){
  bool hasChange = true;
  // if (flywheelVoltageCache != flywheelVoltage){
  //   flywheelVoltageCache = flywheelVoltage;
  //   hasChange = true;
  // } else if (flywheelPid.lastVoltage != flywheelPid.currentVoltage){
  //   hasChange = true;
  // }

  if (hasChange){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);

    // Display voltage
    Controller1.Screen.print("Voltage: ");
    Controller1.Screen.print(flywheelRPM);
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Flywheel: ");
    // Controller1.Screen.print((int) flywheelPid.currentVoltage);
  }
}

void wpAuton(){
  // This will get both rollers & shoot two disks into the far goal
}

void twoDisksWithRollerHigh(){
  // This will drive forward, do the roller and then shoot two disks into the high goal
}

void twoDisksWithRollerLow(){
  // Same thing but with low goal
}

void justRoller(){

}

void skillsAuton(){}

void driver(){
  flywheelPid.motor = &Flywheel;
  flywheelPid.targetRpm = flywheelRPM;

  if (tableMode){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Warning Table Mode!!");
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Press A + Up to exit");
  }

  while (true){
    long startTime = Brain.timer(msec);
    if (!tableMode){
      tank(Controller1.Axis3.position(), Controller1.Axis2.position());
    } else if (Controller1.ButtonA.pressing() && Controller1.ButtonUp.pressing()){
      tableMode = false;
      Controller1.rumble("-..");
      Controller1.Screen.clearScreen();
    }

    flywheelPid.targetRpm = flywheelRPM;

    if (Controller1.ButtonR2.pressing()){
      flywheelPid.setTargetRpm(flywheelRPM);
      flywheelOn = !flywheelOn;
      if (!flywheelOn){
        Flywheel.stop();
      }

      wait(200, msec);
    }

    if (flywheelOn){
      flywheelPid.doTick();
    }

    // Live RPM Control
    if (Controller1.ButtonX.pressing()){
      flywheelRPM += 10;
      wait(200, msec);
    } else if (Controller1.ButtonB.pressing()){
      flywheelRPM -= 10;
      wait(200, msec);
    }
    

    // Intake ============================================================
    if (Controller1.ButtonL1.pressing()){
      // UpperIntake.setVelocity(100, percent);
      UpperIntake.spin(forward, 12, volt);
    } else if (!Controller1.ButtonUp.pressing()) {UpperIntake.stop();}

    if (Controller1.ButtonL2.pressing()){
      // RollerAndBtmIntake.setVelocity(100, percent);
      RollerAndBtmIntake.spin(forward, 12, volt);
    } else if (!Controller1.ButtonDown.pressing()){RollerAndBtmIntake.stop();}

    if (Controller1.ButtonUp.pressing()){
      // UpperIntake.setVelocity(100, percent);
      UpperIntake.spin(reverse, 12, volt);
    } else if (!Controller1.ButtonL1.pressing()){UpperIntake.stop();}

    if (Controller1.ButtonDown.pressing()){
      // RollerAndBtmIntake.setVelocity(100, percent);
      RollerAndBtmIntake.spin(reverse, 12, volt);
    } else if (!Controller1.ButtonL2.pressing()){RollerAndBtmIntake.stop();}

    // Endgame ========================================================
    if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing()
        && Controller1.ButtonX.pressing() && Controller1.ButtonY.pressing()
        && Controller1.ButtonUp.pressing() && Controller1.ButtonDown.pressing()
        && Controller1.ButtonLeft.pressing() && Controller1.ButtonRight.pressing()){
      LittleNut.set(true);
    }

    // Display ========================================================
    double execTime = Brain.timer(msec) - startTime;
    // Display exec time on brain
    if (Brain.Timer.time() % 5 == 0){
      // Brain.Screen.clearLine(1);
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("Exec Time: ");
      Brain.Screen.print(execTime);
      //Print flywheelRPM
      Brain.Screen.setCursor(2, 1);
      Brain.Screen.print("Flywheel RPM (real): ");
      Brain.Screen.print(flywheelPid.motor->velocity(rpm));
      Brain.Screen.setCursor(3, 1);
      Brain.Screen.print("Flywheel RPM (target): ");
      Brain.Screen.print(flywheelPid.targetRpm);
      //Print the percent of the flywheel relative to the target RPM
      Brain.Screen.setCursor(4, 1);
      Brain.Screen.print("Flywheel Percent: ");
      double percent = (flywheelPid.motor->velocity(rpm) / flywheelPid.targetRpm) * 100;
      Brain.Screen.print(percent);
      //Print the voltage of flywheel
      Brain.Screen.setCursor(5, 1);
      Brain.Screen.print("Flywheel Voltage: ");
      Brain.Screen.print(flywheelPid.motor->voltage(volt));
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

  competition comp;
  comp.drivercontrol(driver);
  comp.autonomous(wpAuton);

  if (!comp.isCompetitionSwitch()){
    tableMode = true;
  }
}
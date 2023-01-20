// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Flywheel             motor_group   1, 2            
// LittleNut            digital_out   A               
// RollerAndBtmIntake   motor         3               
// UpperIntake          motor         4               
// BigNut               digital_out   B               
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

int flywheelVoltage = 12;
MotorPid flywheelPid;

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
  flywheelVoltage += 10;
}

void buttonBPressed(){
  flywheelVoltage -= 10;
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
    Controller1.Screen.print(flywheelVoltage);
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Flywheel: ");
    // Controller1.Screen.print((int) flywheelPid.currentVoltage);
  }
}

void getRoller(){
  // This will get both rollers & shoot two disks into the far goal

  // Get the roller
  UpperIntake.setVelocity(100, percent);
  drive(-0.20);
  UpperIntake.spinFor(-75, deg);
}

void shootFromMiddle(){
  getRoller();
  
  Flywheel.spin(forward, 6.95, volt); // Spin up early
  
  // Drive to middle
  drive(0.2);
  turn(-57);
  drive(3);


  // Align with goal
  turn(102);

  //Shoot disks
  UpperIntake.setVelocity(30, percent);
  RollerAndBtmIntake.setVelocity(30, percent);

  RollerAndBtmIntake.spin(forward);
  UpperIntake.spin(forward);

  wait(3, sec);
  Flywheel.spin(forward, 7.5, volt);
}

void shootFromRoller(){
    // Shoot the disk in
  // Flywheel.spin(forward, 9.2, volt);
  Flywheel.spin(forward, 260, rpm);
  UpperIntake.setVelocity(30, percent);
  RollerAndBtmIntake.setVelocity(30, percent);

  drive(0.2);
  turn(18);

  wait(1000, msec);

  UpperIntake.spin(forward);
  RollerAndBtmIntake.spin(forward);
}

void soloWinpoint(){
  getRoller();
  shootFromMiddle();

  // Get the far roller
}

void rollerLowGoal(){
  getRoller();
  drive(0.2);
  turn(-95);
  drive(2.35);

  Flywheel.spin(forward, 5, volt);
  UpperIntake.spin(forward, 100, percent);
  RollerAndBtmIntake.spin(forward, 100, percent);

  wait(5, sec);

  RollerAndBtmIntake.stop();
  UpperIntake.stop();
  Flywheel.stop();
}

void skillsAuton(){
  // Get the first roller
  drive(-0.18, 75);
  UpperIntake.setVelocity(100, percent);
  UpperIntake.spinFor(-125, deg);

  // Get the second roller
  drive(0.97, 75);
  turn(-102, 75);
  drive(-0.90, 75);
  UpperIntake.spinFor(-115, deg);

  // Shoot our two preloads
  drive(0.5, 75);
  Flywheel.spin(forward, 6.2, volt);
  turn(96);
  drive(2.15, 75);

  turn(24, 75); // Align with goal
  RollerAndBtmIntake.spin(forward, 25, percent);
  UpperIntake.spin(forward, 25, percent);

  wait(10, seconds);
  UpperIntake.stop(); RollerAndBtmIntake.stop(); Flywheel.stop();

  // Drive to the other rollers
  turn(-115, 75);
  drive(3.7, 75);
  turn(-110, 75);

  // Get da rollers
  drive(-1.59, 75);
  UpperIntake.setVelocity(100, percent);
  UpperIntake.spinFor(-115, deg);

  // Get da third roller
  drive(0.75, 75);
  turn(-90, 75);
  drive(-0.45);
  UpperIntake.spinFor(-115, deg);
}

void driver(){
  flywheelPid.motor = &Flywheel;
  flywheelPid.targetRpm = flywheelVoltage;

  if (tableMode){
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Warning Table Mode!!");
    Controller1.Screen.setCursor(2, 1);
    Controller1.Screen.print("Press A + Up to exit");
    Controller1.Screen.setCursor(3, 1);
    Controller1.Screen.print("X + B = Auton");
  }

  while (true){
    long startTime = Brain.timer(msec);
    if (!tableMode){
      tank(Controller1.Axis3.position(), Controller1.Axis2.position());
      // Print flywheel voltage
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Flywheel: ");
      Controller1.Screen.print((int) flywheelVoltage);
    } else if (Controller1.ButtonA.pressing() && Controller1.ButtonUp.pressing() && tableMode){
      tableMode = false;
      Controller1.rumble("-..");
      Controller1.Screen.clearScreen();
    } else if (Controller1.ButtonX.pressing() && Controller1.ButtonB.pressing() && tableMode){
      Controller1.rumble("-..");
      rollerLowGoal();
      wait(1, seconds);
    }

    // flywheelPid.targetRpm = flywheelRPM;

    if (Controller1.ButtonR2.pressing()){
      // flywheelPid.setTargetRpm(flywheelRPM);
      flywheelOn = !flywheelOn;
      if (!flywheelOn){
        Flywheel.stop();
      } else {
        Flywheel.spin(forward, flywheelVoltage, volt);
      }

      wait(200, msec);
    }

    if (flywheelOn){
      // flywheelPid.doTick();
      Flywheel.spin(forward, flywheelVoltage, volt);
    }

    // Live RPM Control
    if (Controller1.ButtonX.pressing()){
      flywheelVoltage += 1;
      wait(200, msec);
    } else if (Controller1.ButtonB.pressing()){
      flywheelVoltage -= 1;
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
      BigNut.set(true);
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
  BigNut.set(false);

  // competition comp;
  // comp.drivercontrol(driver);
  // comp.autonomous(skillsAuton);

  // if (!comp.isCompetitionSwitch()){
  //   tableMode = true;
  // }

  // driver();

  skillsAuton();
}
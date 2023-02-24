// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Flywheel             motor_group   1, 2            
// EndGame              digital_out   G               
// RollerAndBtmIntake   motor         3               
// UpperIntake          motor         4               
// DiskShootDetector    digital_in    F               
// EndgameBlocker       digital_out   H               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "sys/_stdint.h"
#include "v5_apitypes.h"
#include "vex.h"
#include <sys/types.h>

using namespace vex;

bool flywheelOn            = false;
bool flywheelButtonEnabled = true;

double defaultFlywheelVoltage = 7;
double flywheelVoltage        = defaultFlywheelVoltage;
int flywheelModifier          = 0;

void getRoller(){
  UpperIntake.setVelocity(100, percent);
  drive(-0.17);
  UpperIntake.spinFor(-90, deg, true);
  wait(0.5, sec);
}

void shootFromMiddle(){
  getRoller();
  // Flywheel.spin(forward, 9, volt); // Spin up early
  
  // Drive to middle
  drive(0.25, 70);
  turn(-72, 70);
  drive(3.05, 70);

  // Align with goal
  turn(57, 70);

  //Shoot disks
  UpperIntake.setVelocity(40, percent);
  RollerAndBtmIntake.setVelocity(40, percent);

  RollerAndBtmIntake.spin(forward);
  UpperIntake.spin(forward);

  wait(5, sec);
  Flywheel.stop();
  UpperIntake.stop();
  RollerAndBtmIntake.stop();
  // Flywheel.spin(forward, 7.5, volt);  
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
  shootFromMiddle(); // Has getRoller()
  
  // Get the far roller
  turn(-60, 70);
  drive(2.45, 70);
}

void rollerLowGoal(){
  getRoller();
  drive(0.2);
  flywheelVoltage = 5.2;
  flywheelOn = true;
  // Flywheel.spin(forward, 5.2, volt);
  turn(-90);
  drive(2.20);

  wait(2.2, seconds);
  UpperIntake.spin(forward, 40, percent);
  RollerAndBtmIntake.spin(forward, 40, percent);

  wait(5, sec);

  RollerAndBtmIntake.stop();
  UpperIntake.stop();
  // Flywheel.stop();
  flywheelOn = false;
}

void safeDrive(double distance){driveAsync(distance);}
void safeDrive(double distance, double speed){driveAsync(distance, speed);}
void safeDrive(double distance, double speed, double timeout){driveAsync(distance, speed); wait(timeout, msec);}

void slamRamAss(){
  driveAsync(-0.5, 75); // Thrust forward
  wait(1, sec);
  driveAsync(0.1, 75); // Pull out
}

void skillsAuton(){
  UpperIntake.setVelocity(100, percent);
  // safeDrive(-0.175);
  slamRamAss();
  UpperIntake.spinFor(-120, deg, false);
  wait(2, sec);
  
  // Drive backwards to get the second roller
  // drive(0.225, 70);
  safeDrive(0.215, 50, 1000);
  turn(40, 70);
  drive(0.70);
  turn(-125, 70);

  drive(-0.60); //Drive up to the second roller
  slamRamAss();
  UpperIntake.spinFor(155, deg, false);
  wait(1, seconds);

  // Drive and get the second rollers
  drive(0.65);
  
  // turn(246, 70); // Align with diagonal line
  turn(70, 70);

  wait(1, sec);
  calibrateInertial();
  updateRotation();

  wait(1, sec); EndGame.set(true);
  return;
  drive(-5.52, 70);
  turn(57);

  slamRamAss();
  UpperIntake.spinFor(-125, deg, true);
  wait(0.5, sec);

  drive(0.8);
  turn(-97);
  drive(-0.45);
  slamRamAss();
  UpperIntake.spinFor(-125, deg, false);
  wait(1, sec);
  
  // End game
  drive(0.5);
  turn(60);
  // EndGame.set(true);
}

int lFlywheelState = 0;
void driver(){
  // In case I forget to clean up shtuff at the end of auton.
  flywheelOn      = false;
  flywheelVoltage = defaultFlywheelVoltage;

  while (true){
    tank(Controller1.Axis3.position(), Controller1.Axis2.position());

    // Just store the flywheel state as a int
    if (lFlywheelState != flywheelVoltage + round(flywheelModifier) + int(flywheelButtonEnabled)){
      lFlywheelState = flywheelVoltage + round(flywheelModifier) + int(flywheelButtonEnabled);
      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("Flywheel: ");
      Controller1.Screen.print((int) flywheelVoltage);
      Controller1.Screen.print(" (+");
      Controller1.Screen.print((int) flywheelModifier);
      Controller1.Screen.print(")");

      if (!flywheelButtonEnabled){
        Controller1.Screen.print(" [OFF]");
      } else {
        Controller1.Screen.print(" [ON]");
      }
    }

    if (Controller1.ButtonR2.pressing()){
      flywheelOn = !flywheelOn;
      wait(200, msec);
    }

    // Live RPM Control
    if (Controller1.ButtonX.pressing()){
      flywheelVoltage += 1;
      wait(200, msec);
    } else if (Controller1.ButtonB.pressing()){
      flywheelVoltage -= 1;
      wait(200, msec);
    }

    // RPM Button Toggle
    if (Controller1.ButtonLeft.pressing() && Controller1.ButtonRight.pressing()){
      Controller1.rumble("..");
      flywheelButtonEnabled = !flywheelButtonEnabled;
      wait(200, msec);
    }
    
    // Intake ============================================================
    if (Controller1.ButtonL1.pressing()){
      UpperIntake.spin(forward, 12, volt);
    } else if (!Controller1.ButtonUp.pressing()) {UpperIntake.stop();}

    if (Controller1.ButtonL2.pressing()){
      RollerAndBtmIntake.spin(forward, 12, volt);
    } else if (!Controller1.ButtonDown.pressing()){RollerAndBtmIntake.stop();}

    if (Controller1.ButtonUp.pressing()){
      UpperIntake.spin(reverse, 12, volt);
    } else if (!Controller1.ButtonL1.pressing()){UpperIntake.stop();}

    if (Controller1.ButtonDown.pressing()){
      RollerAndBtmIntake.spin(reverse, 12, volt);
    } else if (!Controller1.ButtonL2.pressing()){RollerAndBtmIntake.stop();}

    // Endgame &/Blocker  =============================================
    if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing()
        && Controller1.ButtonX.pressing() && Controller1.ButtonY.pressing()){
      EndGame.set(true);
      wait(500, msec);
      EndGame.set(false);
    }

    if (Controller1.ButtonLeft.pressing() && Controller1.ButtonRight.pressing()
        && Controller1.ButtonUp.pressing() && Controller1.ButtonDown.pressing()){
          EndgameBlocker.set(true);
          wait(500, msec);
          EndgameBlocker.set(false);
    }
  }
}

int flywheel_task(){
  while (true){
    if (DiskShootDetector.value() == 0 && flywheelButtonEnabled){
      flywheelModifier = flywheelVoltage; // Double the voltage for a bit
    }

    if (flywheelModifier > 0 && flywheelModifier != 0){
      flywheelModifier -= 0.002;
    } else {
      flywheelModifier = 0;
    }

    if (flywheelOn){Flywheel.spin(forward, flywheelVoltage + flywheelModifier, volt);
    } else {Flywheel.stop();}
    wait(25, msec);
  }

  return 0;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  initDrive();

  task flywheelTask(flywheel_task); // I guess we dont actually have to start it?

  //Main =========================================

  Flywheel.setStopping(coast);

  EndGame.set(false);
  EndgameBlocker.set(false);

  competition comp;
  comp.drivercontrol(driver);

  AutonSelector selector = AutonSelector(&Controller1);
  selector.addAuton("Driver", driver);
  selector.addAuton("Low Goal", rollerLowGoal);
  selector.addAuton("Skills", skillsAuton);
  selector.addAuton("WP", soloWinpoint);

  comp.autonomous(selector.runMenu());
}
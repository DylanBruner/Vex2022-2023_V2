#include "math.h"
#include "vex.h"
#include "vex_imu.h"
using namespace vex;

//https://github.com/Marsgate/Greenhat2

/**************************************************/
//user editable constants

//motor ports
const int left_front  = PORT7;
const int left_rear   = PORT8;
const int right_front = PORT5;
const int right_rear  = PORT6;
gearSetting gear_ratio = ratio18_1;


bool noPid = false;
//gyro port (set to 0 if not using)
const int gyro_port = 9;
inertial inert = inertial(PORT9);

//distance constants
const int distance_constant = 685; //ticks per tile
const double degree_constant = 2.76;//2.75; //ticks per degree

// I hate this...
// const double left_modifier  = 1.15;
// const double right_modifier = 1.0;

double targetRotation = 0;
double start_encoder_left  = 0;
double start_encoder_right = 0;
bool isTurning        = false;

/**************************************************/
//advanced tuning (PID and slew)

//slew control (autonomous only)
const int accel_step = 8; //smaller number = more slew
const int deccel_step = 200; //200 = no slew
const int arc_step = 2; // acceleration for arcs

//straight driving constants
const double driveKP = .3;
const double driveKD = .5; // default was .5, seems to help with preventing turning when driving straight

//turning constants
const double turnKP = .8; // Default 0.8
const double turnKD = 3;

//arc constants
const double arcKP = .05;


/**************************************************/
//edit below with caution!!!
static int driveMode = 0;
static int driveTarget = 0;
static int turnTarget = 0;
static int maxSpeed = 100;

//gyro
inertial iSens(gyro_port);

//motors
motor left1(left_front, gear_ratio, 0);
motor left2(left_rear, gear_ratio, 0);
motor right1(right_front, gear_ratio, 1);
motor right2(right_rear, gear_ratio, 1);

motor_group leftMotors = {left1, left2};
motor_group rightMotors = {right1, right2};

/**************************************************/
void spinLeft(int amount){leftMotors.spinFor(amount, deg);}
void spinRight(int amount){rightMotors.spinFor(amount, deg, true);}
void disablePid(){noPid = true;}
void enablePid(){noPid = false;}

void updateRotation(){
  targetRotation = inert.rotation(deg);
}

void calibrateInertial(){
  inert.calibrate();
}

//task::sleep shorthand
void delay(int sleepTime){
  task::sleep(sleepTime);
}

/**************************************************/
//basic control
void left_drive(int vel){
  vel *= 120;
  leftMotors.spin(fwd, vel, voltageUnits::mV);
}

void right_drive(int vel){
  vel *= 120;
  rightMotors.spin(fwd, vel, voltageUnits::mV);
}

void setBrakeMode(brakeType b){
  leftMotors.setStopping(b);
  rightMotors.setStopping(b);
  leftMotors.stop();
  rightMotors.stop();
}

void reset(){
  leftMotors.resetRotation();
  rightMotors.resetRotation();
  setBrakeMode(hold);
}

int drivePos(){
  return (right1.rotation(deg) + left1.rotation(deg)*driveMode)/2;
}

/**************************************************/
//slew control
static int lastSpeed = 0;
int slew(int speed){
  int step;

  if(abs(lastSpeed) < abs(speed))
    if(driveMode == 0)
      step = arc_step;
    else
      step = accel_step;
  else
    step = deccel_step;

  if(speed > lastSpeed + step)
    lastSpeed += step;
  else if(speed < lastSpeed - step)
    lastSpeed -= step;
  else{
    lastSpeed = speed;
  }

  return lastSpeed;
}

/**************************************************/
//drive settling
bool isDriving(){
  static int count = 0;
  static int last = 0;
  static int lastTarget = 0;

  int curr = drivePos();

  int target = turnTarget;
  if(driveMode == 1)
    target = driveTarget;

  if(abs(last-curr) < 3)
    count++;
  else
    count = 0;

  if(target != lastTarget)
    count = 0;

  lastTarget = target;
  last = curr;

  //not driving if we haven't moved
  if(count > 4)
    return false;
  else
    return true;
}

void waitUntilSettled(){
  while(isDriving()) delay(10);
}

/**************************************************/
//autonomous functions
void driveAsync(double sp, int max){
  sp *= distance_constant;
  reset();
  maxSpeed = max;
  driveTarget = sp;
  driveMode = 1;
}

void turnAsync(double sp, int max){
  sp *= degree_constant;
  reset();
  maxSpeed = max;
  turnTarget = sp;
  driveMode = -1;
}

void drive(double sp, int max){
  targetRotation      = inert.rotation();
  start_encoder_left  = left1.rotation(deg);
  start_encoder_right = right1.rotation(deg);

  driveAsync(sp, max);
  delay(450);
  waitUntilSettled();
}

void turn(double sp, int max){
  isTurning = true;
  turnAsync(sp, max);
  delay(450);
  waitUntilSettled();
  isTurning = false;
}

void fastDrive(double sp, int max){
  if(sp < 0) max = -max;
  reset();
  lastSpeed = max;
  driveMode = 0;
  left_drive(max);
  right_drive(max);

  if(sp > 0)
    while(drivePos() < sp * distance_constant) delay(20);
  else
    while(drivePos() > sp * distance_constant) delay(20);
}

void timeDrive(int t, int left, int right){
  left_drive(left);
  right_drive(right == 0 ? left : right);
  delay(t);
}

void velocityDrive(int t, int max){
  leftMotors.spin(fwd, max, pct);
  rightMotors.spin(fwd, max, pct);
  delay(t);
}

void arc(bool mirror, int arc_length, double rad, int max, int type){
  reset();
  int time_step = 0;
  driveMode = 0;
  bool reversed = false;

  //reverse the movement if the length is negative
  if(arc_length < 0){
    reversed = true;
    arc_length = -arc_length;
  }

  //fix jerk bug between velocity movements
  if(type < 2){
    leftMotors.stop();
    rightMotors.stop();
    delay(10);
  }

  while(time_step < arc_length){

    //speed
    int error = arc_length-time_step;
    int speed = error*arcKP;

    if(type == 1 || type == 2)
      speed = max;

    //speed limiting
    if(speed > max)
      speed = max;
    if(speed < -max)
      speed = -max;

    //prevent backtracking
    if(speed < 0)
      speed = 0;

    speed = slew(speed); //slew

    if(reversed)
      speed = -speed;

    double scaled_speed = speed*rad;

    if(type == 1)
      scaled_speed *= (double)time_step/arc_length;
    else if(type == 2)
      scaled_speed *= fabs(2*(.5-(double)time_step/arc_length));
    else if(type == 3)
      scaled_speed *= (1-(double)time_step/arc_length);

    //assign drive motor speeds
    leftMotors.spin(fwd, mirror ? speed : scaled_speed, pct);
    rightMotors.spin(fwd, mirror ? scaled_speed : speed, pct);

    //increment time step
    time_step += 10;
    delay(10);
  }

  if(type != 1 && type != 2){
    leftMotors.stop();
    rightMotors.stop();
  }
}

void arcLeft(int arc_length, double rad, int max, int type){
  arc(false, arc_length, rad, max, type);
}

void arcRight(int arc_length, double rad, int max, int type){
  arc(true, arc_length, rad, max, type);
}

void scurve(bool mirror, int arc1, int mid, int arc2, int max){

  //first arc
  arc(mirror, arc1, 1, max, 1);
 
  //middle movement
  velocityDrive(mid, max);

  //final arc
  arc(!mirror, arc2, 1, max, 2);

}

void sLeft(int arc1, int mid, int arc2, int max){
  scurve(false, arc1, mid, arc2, max);
}

void sRight(int arc1, int mid, int arc2, int max){
  scurve(true, arc1, mid, arc2, max);
}

void _sLeft(int arc1, int mid, int arc2, int max){
  scurve(true, -arc1, mid, -arc2, -max);
}

void _sRight(int arc1, int mid, int arc2, int max){
  scurve(false, -arc1, -mid, -arc2, max);
}

/**************************************************/
//task control
int driveTask(){
  int prevError = 0;
  double kp;
  double kd;
  int sp;

  while(1){
    delay(20);
    if (noPid) continue;

    if(driveMode == 1){
      sp = driveTarget;
      kp = driveKP;
      kd = driveKD;
    }else if(driveMode == -1){
      sp = turnTarget;
      kp = turnKP;
      kd = turnKD;
    }else{
      continue;
    }

    //read sensors
    int sv = drivePos();
    // if(gyro_port != 0 && driveMode == -1){
    //   // sv = -iSens.rotation(deg);
    //   sv = -inert.rotation(deg);
    //   //print out sv
    //   // printf("sv: %d", sv);
    // }

    //speed
    int error = sp-sv;
    int derivative = error - prevError;
    prevError = error;
    int speed = error*kp + derivative*kd;

    //speed limiting
    if(speed > maxSpeed)
      speed = maxSpeed;
    if(speed < -maxSpeed)
      speed = -maxSpeed;


    double inert_error_left  = inert.rotation(deg) - targetRotation;
    double inert_error_right = -inert.rotation(deg) - targetRotation;
    speed = slew(speed); //slew

    // Drive straight because jayson isn't
    if ((driveMode != -1 && !isTurning) && (fabs(inert_error_left) > 1 || fabs(inert_error_right) > 1) && (abs(error) > 20)){
      // use inertial to correct turning
      double l_speed = speed + inert_error_left * 0.1;
      double r_speed = speed + inert_error_right * 0.1;

      double encoder_difference = fabs(start_encoder_left - left1.rotation(deg)) - fabs(start_encoder_right - right1.rotation(deg));
      // if the encoders are not equal, slow down the motor that is going faster
      if (fabs(encoder_difference) > 10 && false){//Disable this shit for now
        // printf("encoder difference: %f\n", encoder_difference);
        if (encoder_difference > 0){
          l_speed = l_speed * 0.6;
        } else {
          r_speed = r_speed * 0.6;
        }
      }

      left_drive(l_speed);
      right_drive(r_speed);

    } else {
      left_drive(speed*driveMode);
      right_drive(speed);
    }
  }
}

void initDrive(){
  task drive_task(driveTask);
  if(gyro_port != 0){
    iSens.calibrate();
    while(iSens.isCalibrating()) delay(20);
  }

  // inert.calibrate();
  // while(inert.isCalibrating()) delay(20);
}

/**************************************************/
//operator control
double l_pos = 0; double r_pos = 0;
bool holdStarted = false;

void betterHold(){
  // We don't want holding to kick in untill were basically standstill
  double overall_speed = abs(Controller1.Axis3.position()) + abs(Controller1.Axis2.position()) +
                         leftMotors.velocity(percent) + rightMotors.velocity(percent);
  
  double controller_speed = abs(Controller1.Axis3.position() + abs(Controller1.Axis2.position()));

  if (controller_speed > 2 && holdStarted){
    holdStarted = false;
  }

  if (overall_speed < 10){
    // Accounting for a little drift
    if (!holdStarted){
      holdStarted = true;
      l_pos = leftMotors.position(deg);
      r_pos = rightMotors.position(deg);
    }

    double l_error = l_pos - leftMotors.position(deg);
    double r_error = r_pos - rightMotors.position(deg);

    rightMotors.setVelocity(100, percent);
    leftMotors.setVelocity(100, percent);

    leftMotors.spinFor(l_error, deg);
    rightMotors.spinFor(r_error, deg);
  } else {
    holdStarted = false;
  }
}

double currentMode    = 0;
double lastChangeTime = 0;

void tank(int left, int right){
  driveMode = 0; //turns off autonomous tasks

  // Attempt to stop the robot from jerking..
  // When at higher speeds we coast and when standing still hold
  if ((int) Brain.Timer.time(msec) % 5 == 0 && currentMode == 2){
    betterHold();
  }
  
  if (Brain.Timer.time(msec) - lastChangeTime > 500){
    lastChangeTime = Brain.Timer.time(msec);
   
    if (((fabs(leftMotors.velocity(percent)) > 20 || 
        fabs(rightMotors.velocity(percent)) > 20)) && currentMode != 1){
      currentMode = 1;
      rightMotors.setStopping(coast);
      leftMotors.setStopping(coast);
    } else if (currentMode != 2) {
      currentMode = 2;
      // rightMotors.setStopping(hold);
      // leftMotors.setStopping(hold);
      setBrakeMode(hold);
    }
  }

  // if (currentMode == 2){
  //   betterHold();
  // }

  left_drive(left);
  right_drive(right);
}

void arcade(int vertical, int horizontal){
  driveMode = 0; //turns off autonomous task

  left_drive(vertical + horizontal);
  right_drive(vertical - horizontal);
}
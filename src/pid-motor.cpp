#include "pid-motor.h"
#include "vex_thread.h"

using namespace vex;

void PidMotor::setTarget(double target) {
  this->target = target;
}

void PidMotor::goToTarget(){
    //If bool wait == false, run this in a separate thread
    bool locationReached = false;

    m->setVelocity(0, percent);

    while (!locationReached){
      double motorPosition = m->position(degrees);
      double error = target - motorPosition;
      double velocity = error * 0.2;
      m->setVelocity(velocity, percent);
      m->spin(forward);
      if (fabs(error) < maxError){
        locationReached = true;
        m->stop();
      }
    }
}

void PidMotor::tickToTarget(){
    //If bool wait == true, run this in the main thread
    double motorPosition = m->position(degrees);
    double error = target - motorPosition;
    double velocity = error * 0.2;
    m->setVelocity(velocity, percent);
    m->spin(forward);
    if (fabs(error) < maxError){
      m->stop();
    }
}

bool PidMotor::isAtTarget(){
  double motorPosition = m->position(degrees);
  double error = target - motorPosition;

  return fabs(error) < maxError;
}
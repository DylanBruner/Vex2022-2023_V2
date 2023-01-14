#include "vex.h"

using namespace vex;

void MotorPid::doTick() {
  tickCounter++;
  
  if (tickCounter >= 15){
    tickCounter = 0;
    
    double rpmDiff = targetRpm - motor->velocity(rpm);
    if (rpmDiff > maxThreshold) {
        currentVoltage += rpmDiff * 0.02;
    } else if (rpmDiff < -maxThreshold) {
        currentVoltage += rpmDiff * 0.01;
    }

    if (lastVoltage != currentVoltage){
      lastVoltage = currentVoltage;
      motor->spin(forward, currentVoltage, volt);
    }
  }
}

void MotorPid::setTargetRpm(int rpm) {targetRpm = rpm;}
void MotorPid::setMaxThreshold(double threshold) {maxThreshold = threshold;}
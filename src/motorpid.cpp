#include "vex.h"

using namespace vex;

void MotorPid::doTick() {
    // Get the current RPM of the motor
    int currentRpm = motor->velocity(vex::velocityUnits::rpm);

    // Calculate the error
    double error = targetRpm - currentRpm;

    // Calculate the integral
    integral += error * sampleTime;

    // Calculate the derivative
    double derivative = (error - lastError) / sampleTime;

    // Calculate the output
    double output = kP * error + kI * integral + kD * derivative;

    // Limit the output to a maximum threshold
    output = fmin(output, maxThreshold);

    // Set the motor's speed based on the output
    motor->spin(vex::directionType::fwd, output, vex::velocityUnits::rpm);

    // Update lastError for the next iteration
    lastError = error;

    // Increment tickCounter
    tickCounter++;

    // If sample time has not been reached, return early
    if(tickCounter < sampleTime) {
        return;
    }

    // Reset tickCounter
    tickCounter = 0;
}


void MotorPid::setTargetRpm(int rpm) {targetRpm = rpm; maxThreshold = targetRpm;}
void MotorPid::setMaxThreshold(double threshold) {maxThreshold = threshold;}
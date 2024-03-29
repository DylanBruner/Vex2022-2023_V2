#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor FlywheelMotorA = motor(PORT1, ratio6_1, true);
motor FlywheelMotorB = motor(PORT2, ratio6_1, false);
motor_group Flywheel = motor_group(FlywheelMotorA, FlywheelMotorB);
digital_out EndGame = digital_out(Brain.ThreeWirePort.G);
motor RollerAndBtmIntake = motor(PORT3, ratio18_1, true);
motor UpperIntake = motor(PORT4, ratio18_1, false);
digital_in DiskShootDetector = digital_in(Brain.ThreeWirePort.F);
digital_out EndgameBlocker = digital_out(Brain.ThreeWirePort.H);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}
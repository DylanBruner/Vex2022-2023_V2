using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor_group Flywheel;
extern digital_out LittleNut;
extern digital_out BigNut;
extern motor RollerAndBtmIntake;
extern motor UpperIntake;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
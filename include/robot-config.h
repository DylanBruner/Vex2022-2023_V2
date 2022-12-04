using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LeftFrontMotor;
extern motor LeftBackMotor;
extern motor RightBackMotor;
extern motor RightFrontMotor;
extern inertial Inert20;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
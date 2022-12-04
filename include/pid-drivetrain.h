#include "vex.h"
#include "pid-motor.h"
#include "vex_imu.h"

using namespace vex;

class PidDriveTrain {
  public:
    int port;

    PidMotor *LeftBack;
    PidMotor *LeftFront;
    PidMotor *RightBack;
    PidMotor *RightFront;
    inertial *Inert20;

    //Functions
    void goTo(double target, bool wait = false);
    void setMaxError(double maxError);
    void turn(double degrees);

    //Config
    int maxError = 4; //It gets stuck when it's too low right now, possibly because of the encoders
    
    //Constructor
    PidDriveTrain(motor *LeftBack, motor *LeftFront, 
                  motor *RightBack, motor *RightFront, inertial *Inert20){
        this->LeftBack   = new PidMotor(LeftBack);
        this->LeftFront  = new PidMotor(LeftFront);
        this->RightBack  = new PidMotor(RightBack);
        this->RightFront = new PidMotor(RightFront);
        this->Inert20    = Inert20;

        this->Inert20->calibrate();
    }
};
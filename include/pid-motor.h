#include "vex.h"

using namespace vex;

class PidMotor {
  public:
    int port;
    motor *m;

    //Config
    int maxError = 4; //It gets stuck when it's too low right now, possibly because of the encoders

    //Functions
    void setTarget(double target);
    void goToTarget();
    void tickToTarget();
    bool isAtTarget();

    //Constructor
    PidMotor(motor *motor) {
      this->m = motor;
    }

  private:
    double target;
};
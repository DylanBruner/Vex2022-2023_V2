#include "pid-drivetrain.h"
#include "vex.h"

using namespace vex;

void PidDriveTrain::goTo(double target, bool wait){
    LeftBack->setTarget(target);
    LeftFront->setTarget(target);
    RightBack->setTarget(target);
    RightFront->setTarget(target);

    while (!LeftBack->isAtTarget() || !LeftFront->isAtTarget() || !RightBack->isAtTarget() || !RightFront->isAtTarget()){
      LeftBack->tickToTarget();
      LeftFront->tickToTarget();
      RightBack->tickToTarget();
      RightFront->tickToTarget();
    }
}

void PidDriveTrain::setMaxError(double maxError){
  LeftBack->maxError   = maxError;
  LeftFront->maxError  = maxError;
  RightBack->maxError  = maxError;
  RightFront->maxError = maxError;
}

void PidDriveTrain::turn(double degrees){
    double startRotation = Inert20->rotation();

    LeftBack->setTarget(startRotation + degrees);
    LeftFront->setTarget(startRotation + degrees);
    RightBack->setTarget(startRotation - degrees);
    RightFront->setTarget(startRotation - degrees);

    while (!LeftBack->isAtTarget() || !LeftFront->isAtTarget() || !RightBack->isAtTarget() || !RightFront->isAtTarget()){
      LeftBack->tickToTarget();
      LeftFront->tickToTarget();
      RightBack->tickToTarget();
      RightFront->tickToTarget();
    }
}
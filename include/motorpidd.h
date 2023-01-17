
#include "vex_motorgroup.h"
class MotorPid {
    public:
        vex::motor_group *motor;
        int targetRpm = 0;
        void doTick();
        void setTargetRpm(int rpm);
        void setMaxThreshold(double threshold);

        //PID stuffs
        double kP = 0.2;
        double kI = 0.4;
        double kD = 0.5;
        double integral = 0;
        double lastError = 0;
        double sampleTime = 20;
        double maxThreshold = 350;

    private:
      int tickCounter = 0;
};
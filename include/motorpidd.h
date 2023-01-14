
#include "vex_motorgroup.h"
class MotorPid {
    public:
        vex::motor_group *motor;
        int targetRpm = 0;
        double maxThreshold = 25;
        void doTick();
        void setTargetRpm(int rpm);
        void setMaxThreshold(double threshold);
        double currentVoltage = 0;
        double lastVoltage    = -1;

    private:
      int tickCounter = 0;
};
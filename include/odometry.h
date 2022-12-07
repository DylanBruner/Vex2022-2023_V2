//Java uses this and i like it
#include "vex_triport.h"
class Point2D{
    public:
        double x;
        double y;
        double getDistance(Point2D target);
};

class Odometry{
    public:
        double getHeading();
        Point2D getPosition();
        vex::encoder *L_ENCODER;
        vex::encoder *R_ENCODER;
        vex::encoder *M_ENCODER;
        double *d1; //This is the distance between the left and right wheels

        double x0 = 0;
        double y0 = 0;

        Odometry(vex::encoder l_encoder, vex::encoder r_encoder,
                 vex::encoder m_encoder, double d1);
};
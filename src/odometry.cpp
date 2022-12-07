#include "vex.h"
#include <math.h>

Point2D Odometry::getPosition(){
    double dx = R_ENCODER->position(vex::rotationUnits::deg) + L_ENCODER->position(vex::rotationUnits::deg);
    double dy = M_ENCODER->position(vex::rotationUnits::deg);

    double theta = getHeading();

    Point2D point;

    point.x = x0 + dx * cos(theta) - dy * sin(theta);
    point.y = y0 + dx * sin(theta) + dy * cos(theta);
    return point;
}

double Odometry::getHeading(){
    //heading = arctan((distance right - distance left) / wheelbase)
    return atan((R_ENCODER->position(vex::rotationUnits::deg) - L_ENCODER->position(vex::rotationUnits::deg)) / *d1);
}

Odometry::Odometry(vex::encoder l_encoder, vex::encoder r_encoder,
                   vex::encoder m_encoder, double d1){
    this->L_ENCODER = &l_encoder;
    this->R_ENCODER = &r_encoder;
    this->M_ENCODER = &m_encoder;
    this->d1 = &d1;
}

//Point2D Code
//=============================================================================
double Point2D::getDistance(Point2D target){
    //Calculate the distance between two points
    double x = target.x - this->x;
    double y = target.y - this->y;
    return sqrt(x*x + y*y);
}
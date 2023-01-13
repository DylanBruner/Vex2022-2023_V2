#include "vex.h"
#include <math.h>

Point2D Odometry::getPosition(){
    double distanceLeft   = L_ENCODER->position(vex::rotationUnits::deg);
    double distanceRight  = R_ENCODER->position(vex::rotationUnits::deg);
    double distanceMiddle = M_ENCODER->position(vex::rotationUnits::deg);
    //Flip if needed
    if(FLIP_LEFT)   distanceLeft   *= -1;
    if(FLIP_RIGHT)  distanceRight  *= -1;
    if(FLIP_MIDDLE) distanceMiddle *= -1;

    double dx = distanceRight + distanceLeft;
    double dy = distanceMiddle;

    double theta = getHeading();

    Point2D point;

    point.x = x0 + dx * cos(theta) - dy * sin(theta);
    point.y = y0 + dx * sin(theta) + dy * cos(theta);
    return point;
}

double Odometry::getHeading(){
    //heading = arctan((distance right - distance left) / wheelbase)
    //Don't forget to flip the signs if you need to
    double distance_left  = L_ENCODER->position(vex::rotationUnits::deg);
    double distance_right = R_ENCODER->position(vex::rotationUnits::deg);
    //Flip the signs if needed
    if(FLIP_LEFT) distance_left   *= -1;
    if(FLIP_RIGHT) distance_right *= -1;

    double heading = atan((distance_right - distance_left) / *d1);
    //Convert to degrees
    return heading;
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
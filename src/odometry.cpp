#include "vex.h"
#include <math.h>

Point2D Odometry::getPosition(){
    Point2D position;
    position.x = 0;
    position.y = 0;
    return position;
}

double Odometry::getHeading(){
    return 0;
}

Odometry::Odometry(vex::encoder l_encoder, vex::encoder r_encoder,
                   vex::encoder m_encoder){
    this->L_ENCODER = &l_encoder;
    this->R_ENCODER = &r_encoder;
    this->M_ENCODER = &m_encoder;
}

//Point2D Code
//=============================================================================
double Point2D::getDistance(Point2D target){
    //Calculate the distance between two points
    double x = target.x - this->x;
    double y = target.y - this->y;
    return sqrt(x*x + y*y);
}
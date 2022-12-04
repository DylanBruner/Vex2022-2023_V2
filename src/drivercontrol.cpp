#include "vex.h"

void opcontrol(){
  while (true){
    tank(Controller1.Axis3.value(), Controller1.Axis2.value());
  }
}
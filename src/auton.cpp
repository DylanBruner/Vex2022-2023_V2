#include "vex.h"
using namespace vex;

void testAuton(){
  drive(2);
  wait(1, seconds);
  turn(90);
  wait(1, seconds);
  turn(90);
  wait(1, seconds);
  drive(2);
  wait(1, seconds);
}
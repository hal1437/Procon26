
#pragma once
#include "../Structure/Point.h"

#define CLOCKWISE_FOR(V_NAME) \
for(Point V_NAME : {Point(0,1),Point(1,0),Point(0,-1),Point(-1,0)})

#define RCLOCKWISE_FOR(V_NAME) \
for(Point V_NAME : {Point(0,1),Point(-1,0),Point(0,-1),Point(1,0)})



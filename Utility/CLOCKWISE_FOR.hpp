
#pragma once
#include "../Structure/Point.hpp"

#define CLOCKWISE_FOR(V_NAME) \
for(Point V_NAME : {Point(0,1),Point(1,0),Point(0,-1),Point(-1,0)})

#define RCLOCKWISE_FOR(V_NAME) \
for(Point V_NAME : {Point(0,1),Point(-1,0),Point(0,-1),Point(1,0)})

#define CLOCKWISE_8FOR(V_NAME) \
for(Point V_NAME : {Point(0,1),Point(1,1),Point(1,0),Point(1,-1),Point(0,-1),Point(-1,-1),Point(-1,0),Point(-1,1)})

#define RCLOCKWISE_8FOR(V_NAME) \
for(Point V_NAME : {Point(0,1),Point(-1,1),Point(-1,0),Point(-1,-1),Point(0,-1),Point(1,-1),Point(1,0),Point(1,1)})


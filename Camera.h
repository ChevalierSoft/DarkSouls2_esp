#pragma once

#include "vec.h"

struct Camera
{
public:
	vec4		__rotation;		// vision angle
	vec4		__dunnovec;		// left
	vec4	lookAt;
	vec4	position;

	vec4		__position2;	// target position (player's head)
	float	fovx;
};
#pragma once

#include "vec.h"

struct Camera
{
public:
	vec4	rotation;
	vec4	dunno;
	vec4	lookAt;
	vec4	position;

	vec4	target_pos;	// target position (player's head)
	float	fovx;
};
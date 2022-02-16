#pragma once

#include "vec.h"

struct Camera
{
public:
	vec4		__rotation;
	vec4		__dunnovec;
	vec4	lookAt;
	vec4	position;
	vec4		__position2;
	float	fovx;
};
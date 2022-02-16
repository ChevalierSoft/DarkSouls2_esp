#pragma once

#include "Camera.h"
#include "GameData.h"
#include <d3dx9math.h>

class GameData;

class CameraEx
{
public:
	float	windowWidth = 0;
	float	windowHeight = 0;
	Camera  camera;

	CameraEx() = default;

	void	update(GameData *gd);
	vec3	WorldToScreen(vec3 worldPosition);
};


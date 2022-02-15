#pragma once

#include "CasualLibrary1.0/CasualLibrary.hpp"
#pragma comment(lib, "CasualLibrary.lib")

#include "GameData.h"

struct vec3
{
	float x, y, z;
};

class GameData;

class Enemy
{
	GameData	*g_data;
public:
	uintptr_t	addr = 0;
	int32_t		hp = 0;
	int32_t		hp_max = 0;

	vec3		pos = {0., 0., 0.};

private:
	Enemy() = default;
public:
	Enemy(GameData* gd, uintptr_t a);
	~Enemy() = default;

	void	update();
	void	update(GameData* gd, uintptr_t a);
};

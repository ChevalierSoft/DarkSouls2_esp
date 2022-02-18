#pragma once

#pragma comment (lib,"Gdiplus.lib")
#include "CasualLibrary1.0/CasualLibrary.hpp"
#pragma comment(lib, "CasualLibrary.lib")

#include <vector>
#include "Enemy.h"
#include "CameraEx.h"

class Enemy;
class CameraEx;

class GameData
{
public:
    Memory::External*   memory = NULL;
    Address		    	baseAddr = (uintptr_t)0;
    Address			    a_entity_list = (uintptr_t)0;
    ptrdiff_t		    offset = 0x08;
    uintptr_t		    first_enemy = NULL;
    std::vector<Enemy>  enemies;
    bool                visible = true;
    CameraEx            *cam = NULL;
    int                 width = 0, height = 0;

    GameData();
    ~GameData();

    void    init();
    void    update();
};

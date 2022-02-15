#pragma once

#pragma comment (lib,"Gdiplus.lib")
#include "CasualLibrary1.0/CasualLibrary.hpp"
#pragma comment(lib, "CasualLibrary.lib")

#include <vector>
#include "Enemy.h"

class Enemy;

class GameData
{
public:
    Memory::External*   memory = NULL;
    Address		    	baseAddr = (uintptr_t)0;
    Address			    a_entity_list = (uintptr_t)0;
    ptrdiff_t		    offset = 0x08;
    uintptr_t		    first_enemy = NULL;
    std::vector<Enemy>  enemies;

    GameData();
    ~GameData();

    void    init();
    void    update();
};


#include "GameData.h"

GameData::GameData()
: memory(0), baseAddr((uintptr_t)0), a_entity_list((uintptr_t)0),
	offset(0x08), first_enemy(0)
{}

GameData::~GameData()
{
	delete this->memory;
}

void    GameData::init()
{
    this->memory = new Memory::External("DarkSoulsII.exe");

    this->baseAddr = this->memory->getModule("DarkSoulsII.exe");
    std::cout << "module addr : " << std::hex << this->baseAddr << std::dec << std::endl;

    this->a_entity_list = this->memory->getAddress((this->baseAddr.get() + 0x0160B8D0), { 0x18, 0x80 + 0 * 8 });
    std::cout << "a_entity_list : " << std::hex << this->a_entity_list << std::dec << std::endl;

    cam = new CameraEx();
    //this->cam->init(this);
}

void	GameData::update()
{
    
}
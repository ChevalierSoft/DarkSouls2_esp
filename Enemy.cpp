#include "Enemy.h"

Enemy::Enemy(GameData *gd, uintptr_t a) : g_data(gd), addr(a), hp(0), hp_max(0)
{}

void	Enemy::update()
{
	this->update(this->g_data, this->addr);
}

void	Enemy::update(GameData* gd, uintptr_t a)
{
	if (g_data != gd)
		g_data = gd;
	if (addr != a)
		addr = a;
	if (!g_data || !addr)
		return ;

	uintptr_t	tmp_addr;
	char		hex_str[32];

	////tmp_addr = g_data.memory->getAddress(g_data.enemies[i].addr + 0x168, {});
	this->hp = this->g_data->memory->read<int>(addr + 0x168);
	this->hp_max = this->g_data->memory->read<int>(addr + 0x168 + 0x8);

}
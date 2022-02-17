#include "Paint.h"
#include "Enemy.h"

int Paint::d3D9Init(HWND hWnd) {

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject))) {
		exit(1);
	}

	ZeroMemory(&d3dparams, sizeof(d3dparams));

	d3dparams.BackBufferWidth = width;
	d3dparams.BackBufferHeight = height;
	d3dparams.Windowed = TRUE;
	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.hDeviceWindow = hWnd;
	d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dparams.EnableAutoDepthStencil = TRUE;
	d3dparams.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT res = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);

	if (FAILED(res)) {
		std::wstring ws(DXGetErrorString(res));
		std::string str(ws.begin(), ws.end());
		std::wstring ws2(DXGetErrorDescription(res));
		std::string str2(ws2.begin(), ws2.end());
		std::string error = "Error: " + str + " error description: " + str2;
		exit(1);
	}

	D3DXCreateFont(d3dDevice, 20, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Consolas", &d3dFont);

	return 0;

}

Paint::Paint() {};

Paint::Paint(HWND hWnd, HWND targetWnd, int width, int height) {
	this->width = width;
	this->height = height;
	this->targetWnd = targetWnd;
	d3D9Init(hWnd);
}

int Paint::render(GameData &g_data)
{
	char		hex_str[32];
	uintptr_t	prev = 0;

	if (d3dDevice == nullptr)
		return 1;
	d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	d3dDevice->BeginScene();

	if (targetWnd == GetForegroundWindow() && g_data.visible)
	{

		g_data.cam->update(&g_data);

		sprintf_s(hex_str, "%p", g_data.a_entity_list);
		std::string s_buf = "entity list : " + std::string(hex_str);
		drawText((char *)s_buf.c_str(), width / 10, height / 10, 255, 171, 0, 182);

		g_data.enemies.clear();

		for (int n = 0; n <= 65; ++n)
		{
			uintptr_t tmp_enemy = g_data.memory->read<uintptr_t>((g_data.a_entity_list + (n * 0x8)));

			if (tmp_enemy == 0)
				continue ;
			g_data.enemies.push_back(Enemy(&g_data, tmp_enemy));

			//sprintf_s(hex_str, "%p", tmp_enemy);
			//drawText((char*)("enemy " + std::to_string(n) + " " + std::string(hex_str)).c_str(), width / 10, height / 7 + (n * 15), 255, 0, 255, 182);
		}

		for (int i = 0; i < g_data.enemies.size(); ++i)
		{
			g_data.enemies[i].update();
			drawText((char *)std::to_string(g_data.enemies[i].hp).c_str(), width / 20, height / 7 + (i * 12), 255, 0, 200, 255);
			drawText((char*)std::to_string(g_data.enemies[i].hp_max).c_str(), width / 20 + width / 18, height / 7 + (i * 12), 255, 0, 255, 182);

			drawText((char*)std::to_string((float)g_data.enemies[i].pos.x).c_str(), width / 5, height / 7 + (i * 12), 255, 255, 0, 0);
			drawText((char*)std::to_string((float)g_data.enemies[i].pos.y).c_str(), width / 5 + 130, height / 7 + (i * 12), 255, 0, 255, 0);
			drawText((char*)std::to_string((float)g_data.enemies[i].pos.z).c_str(), width / 5 + 260, height / 7 + (i * 12), 255, 0, 0, 255);
		}


		//drawRectangle(100, 200, 200, 200, D3DCOLOR_ARGB(255, 255, 255, 255));

		for (Enemy &en : g_data.enemies)
		{
			vec3 feet_position(g_data.cam->WorldToScreen(en.pos));
			if (feet_position.z < 100)
			{
				//vec3 torso_position(feet_position);		// TODO : search for the head or torso
				//torso_position.y += 30;
				//float en_height = abs(feet_position.y - torso_position.y);
				std::string str = "hp : " + std::to_string((int)en.hp);
				drawText((char*)str.c_str(), feet_position.x, feet_position.y, 255, 255, 255, 255); //D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}

	d3dDevice->EndScene();
	d3dDevice->PresentEx(0, 0, 0, 0, 0);

	return 0;
}


void Paint::drawText(char* str, int x, int y, int a, int r, int g, int b)
{
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	d3dFont->DrawTextA(0, str, strlen(str), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

void	Paint::drawFilledRectangle(float x, float y, float width, float height, D3DCOLOR color)
{
	D3DXVECTOR2 vertices[2] = { D3DXVECTOR2(x + width / 2, y),D3DXVECTOR2(x + width / 2, y + height) };
	if (!line)
		D3DXCreateLine(d3dDevice, &line);
	line->SetWidth(width);
	line->Draw(vertices, 2, color);
}

void Paint::drawRectangle(float x, float y, float width, float height, D3DCOLOR color)
{
	D3DXVECTOR2 Rect[5];
	Rect[0] = D3DXVECTOR2(x, y);
	Rect[1] = D3DXVECTOR2(x + width, y);
	Rect[2] = D3DXVECTOR2(x + width, y + height);
	Rect[3] = D3DXVECTOR2(x, y + height);
	Rect[4] = D3DXVECTOR2(x, y);
	if (!line)
		D3DXCreateLine(d3dDevice, &line);
	line->SetWidth(1);
	line->Draw(Rect, 5, color);
}
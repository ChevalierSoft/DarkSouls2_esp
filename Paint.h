#pragma once

#include <string> //save error
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <DxErr.h> //get error from error code
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include "GameData.h"

class Paint
{
private:
	IDirect3D9Ex*			d3dObject = NULL;	// used to create device
	IDirect3DDevice9Ex*		d3dDevice = NULL;	// contains functions like begin and end scene 
	D3DPRESENT_PARAMETERS	d3dparams;			// parameters for creating device
	ID3DXFont*				d3dFont = NULL;		// font used when displaying text
	ID3DXLine*				line = NULL;
	HWND					targetWnd;
	int						width;
	int						height;

public:
	Paint();
	Paint(HWND hWnd, HWND targetWnd, int width, int height);
	~Paint() = default;

	int		render(GameData &g_data);

private:
	int		d3D9Init(HWND hWnd);
	void	drawText(char* String, int x, int y, int a, int r, int g, int b);
	void	drawFilledRectangle(float x, float y, float width, float height, D3DCOLOR color);
	void	drawRectangle(float x, float y, float width, float height, D3DCOLOR color);
};
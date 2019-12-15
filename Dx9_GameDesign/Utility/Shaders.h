#pragma once
#include <d3dx9.h>
#include "MyCamera.h"

namespace shaders
{

struct Terrain_VS
{
	Terrain_VS()
	{
		TerShader = 0;
		ConstTab  = 0;

	}
	IDirect3DVertexShader9* TerShader;

	ID3DXConstantTable*		ConstTab;

	D3DXHANDLE				h_ViewProjMatrix;
	D3DXHANDLE				h_PosOffset;
	D3DXHANDLE				h_SunDir;
	
	bool Create(IDirect3DDevice9* d3d);
	void Release();
};


}
using namespace shaders;
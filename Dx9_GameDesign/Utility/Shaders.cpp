#include "Shaders.h"
#include "MyUTIL.h"
bool Terrain_VS::Create(IDirect3DDevice9*	 pDevice)
{

	ID3DXBuffer* shader      = 0;
	ID3DXBuffer* errorBuffer = 0;

	HRESULT hr = 0;

	hr = D3DXCompileShaderFromFile(
		"Shaders\\Terrain_VS.txt",
		0,
		0,
		"VS", // entry point function name
		"vs_1_1",
		D3DXSHADER_DEBUG, 
		&shader,
		&errorBuffer,
		&ConstTab);

	// output any error messages
	if( errorBuffer )
	{
		::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		SAFE_RELEASE(errorBuffer);
	}

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);
		return false;
	}

	//
	// Create shader
	//

	hr = pDevice->CreateVertexShader(
		(DWORD*)shader->GetBufferPointer(),
		&TerShader);

	if(FAILED(hr))
	{
		::MessageBox(0, "CreateTerrain_VS - FAILED", 0, 0);
		return false;
	}

	SAFE_RELEASE(shader);


	// 
	// Get Handles
	//

	h_ViewProjMatrix	= ConstTab->GetConstantByName(0, "ViewProj");
	h_PosOffset			= ConstTab->GetConstantByName(0, "posOffset");
	h_SunDir			= ConstTab->GetConstantByName(0, "sunDir");

	return true;
}
void Terrain_VS::Release()
{
	SAFE_RELEASE(TerShader);
	SAFE_RELEASE(ConstTab);

}


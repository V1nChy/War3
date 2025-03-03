#ifndef _GE_FORMAT_H_
#define _GE_FORMAT_H_
#include "d3dx9.h"

enum GEFormat
{
	GEFormat_UNKNOWN,
	GEFormat_Index16,
	GEFormat_Index32,
	/*
	// Unsigned Formats in D3D
	D3DFMT_R8G8B8
	D3DFMT_A8R8G8B8
	D3DFMT_X8R8G8B8
	D3DFMT_R5G6B5
	D3DFMT_X1R5G5B5
	D3DFMT_A1R5G5B5
	D3DFMT_A4R4G4B4
	D3DFMT_R3G3B2
	D3DFMT_A8
	D3DFMT_A8R3G3B2
	D3DFMT_X4R4G4B4
	D3DFMT_A2B10G10R10
	D3DFMT_A8B8G8R8
	D3DFMT_X8B8G8R8
	D3DFMT_G16R16
	D3DFMT_A2R10G10B10
	D3DFMT_A16B16G16R16
	D3DFMT_A8P8
	D3DFMT_P8
	D3DFMT_L8
	D3DFMT_L16
	D3DFMT_A8L8
	D3DFMT_A4L4
	*/
};

inline D3DFORMAT TranslateToD3DFormat(GEFormat f)
{
	switch(f)
	{
	case GEFormat_Index16:
		return D3DFMT_INDEX16;
		break;
	case GEFormat_Index32:
		return D3DFMT_INDEX32;
		break;
	}
	return D3DFMT_UNKNOWN;

}




#endif
// MyTexture.cpp: implementation of the MyTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "MyTexture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyTexture::MyTexture()
{
	m_uHeight = D3DX_DEFAULT;
	m_uWidth = D3DX_DEFAULT;
	m_iNumMipmaps = D3DX_DEFAULT;	// 表示每次都生成完整数量的Mipmap
	m_uDepth = D3DX_DEFAULT;

	m_eType = TEX_TYPE_2D;   

	m_dwUsage = 0;
	m_eFormat = D3DFMT_UNKNOWN;
	m_ePool = D3DPOOL_MANAGED;
	m_eFilter = D3DX_DEFAULT;
	m_eMipFilter = D3DX_DEFAULT;
	m_cColorKey = MyColor::NoColor;
	

	Enable32Bit(false);
}

MyTexture::~MyTexture()
{

}


/** 如何得到资源文件的大小,重载 */
size_t MyTexture::CalculateSize(void) const
{
	return m_uHeight * m_uWidth * m_uDepth * m_uFinalBpp;
}


//--------------------------------------------------------------------------
size_t MyTexture::GetNumFaces(void) const
{
	return GetTextureType() == TEX_TYPE_CUBE_MAP ? 6 : 1;
}



// MyTexture.h: interface for the MyTexture class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_TEXTURE_H_
#define _MY_TEXTURE_H_

#pragma warning(disable:4786)

#include "MyResource.h"
#include "d3dx9tex.h"
#include "MyColor.h"

class MyTextureManager;

/** Enum identifying the texture type    */
enum TextureType
{
	///< 1D texture, used in combination with 1D texture coordinates
	TEX_TYPE_1D = 1,
	///< 2D texture, used in combination with 2D texture coordinates (default)
	TEX_TYPE_2D = 2,
	///< 3D volume texture, used in combination with 3D texture coordinates
	TEX_TYPE_3D = 3,
	///< 3D cube map, used in combination with 3D texture coordinates
	TEX_TYPE_CUBE_MAP = 4
};


/**
 * 此类从 MyResource 继承而来, 但其本身也是一个基类, 而且是个抽象类
 * 
 * 
 */
class MyTexture : public MyResource  
{
public:
	MyTexture();
	virtual ~MyTexture();
				
	/** Sets the type of texture; can only be changed before load() 	*/
	void SetTextureType(TextureType ttype ) { m_eType = ttype; }

	/** Gets the type of texture 	*/
	TextureType GetTextureType(void) const { return m_eType; }

	/** Gets the number of mipmaps to be used for this texture.	*/
	size_t GetNumMipmaps(void) const {return m_iNumMipmaps;}

	/** Sets the number of mipmaps to be used for this texture.
	    @note
		Must be set before calling any 'load' method.
	*/
	void SetNumMipmaps(size_t num) {m_iNumMipmaps = num;}


	/** Returns the height of the texture.	*/
	unsigned int GetHeight(void) const { return m_uHeight; }

	/** Returns the width of the texture.	*/
	unsigned int GetWidth(void) const { return m_uWidth; }

	/** Returns the depth of the texture (only applicable for 3D textures).	*/
	unsigned int GetDepth(void) const { return m_uDepth; }

	/** Set the height of the texture; can only do this before load();	*/
	void SetHeight(unsigned int h) { m_uHeight = h; }

	/** Set the width of the texture; can only do this before load();	*/
	void SetWidth(unsigned int w) { m_uWidth = w; }

	/** Set the depth of the texture (only applicable for 3D textures);
	    @note
		Can only do this before load();
	*/
	void SetDepth(unsigned int d)  { m_uDepth = d; }


	
	/** Returns the TextureUsage indentifier for this Texture	*/
	DWORD GetUsage() const
	{
	    return m_dwUsage;
	}

	
	/** Sets the TextureUsage indentifier for this Texture; only useful before load()
			
		@param u is a combination of TU_STATIC, TU_DYNAMIC, TU_WRITE_ONLY 
		TU_AUTOMIPMAP and TU_RENDERTARGET (see TextureUsage enum). You are
		strongly advised to use HBU_STATIC_WRITE_ONLY wherever possible, if you need to 
		update regularly, consider HBU_DYNAMIC_WRITE_ONLY.
	*/
	void SetUsage(DWORD u) { m_dwUsage = u; }


	void Enable32Bit( bool setting = true ) 
	{
		setting ? m_uFinalBpp = 32 : m_uFinalBpp = 16;
	}
	
	const MyColor& GetColorKey() const
	{
		return m_cColorKey;
	}

	void SetColorKey(const MyColor& colorkey)
	{
		m_cColorKey = colorkey;
	}


	/** Return the number of faces this texture has. This will be 6 for a cubemap
		texture and 1 for a 1D, 2D or 3D one.
	*/
	virtual size_t GetNumFaces() const;



protected:
	TextureType	m_eType;					///< 纹理类型, 1D/2D/3D体纹理/3D方形纹理

	unsigned long m_uHeight;				///< 纹理高度			
	unsigned long m_uWidth;					///< 纹理宽度
	unsigned long m_uDepth;					///< 纹理深度

	unsigned short m_uFinalBpp;				///	颜色的色深
//	unsigned short m_uSrcBpp;				///< 屏幕色深
//	unsigned long m_uSrcWidth, m_uSrcHeight;///< 屏幕宽高	
//	bool m_bHasAlpha;						///< 是否有ALPHA透明

	size_t m_iNumMipmaps;					///< 多少个Mipmaps

	D3DFORMAT m_eFormat;					///< 图片的颜色格式
	DWORD m_dwUsage;						///< 资源的用法,位填充
	D3DPOOL m_ePool;						///< 存放形式
	DWORD  m_eFilter;						///< 过滤器
	DWORD  m_eMipFilter;
	MyColor m_cColorKey;					///< 颜色键, 0 则关闭
		
protected:
	/** 如何得到资源文件的大小,必须重载 */
	virtual size_t CalculateSize(void) const;

};

#endif

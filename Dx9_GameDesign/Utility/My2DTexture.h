// My2DTexture.h: interface for the My2DTexture class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY2DTEXTURE_H_
#define _MY2DTEXTURE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTexture.h"
#include "MyColor.h"
#include <vector>
using namespace std;

#define GERES_GET_2DTXTR(handleORname) ((My2DTexture *)MyTextureManager::GetSingleton()->GetResource(handleORname))

/** 2D 纹理类，继承自GETexture，属于Resource */
class My2DTexture : public MyTexture  
{
public:
	My2DTexture();
	virtual ~My2DTexture();

	/** 得到纹理指针 */
	LPDIRECT3DTEXTURE9 GetTexture();

	bool GetRGBAPerPix(vector<MyColor>& vRGBAPixs) const;

protected:
	LPDIRECT3DTEXTURE9 	m_lpTexture;			///< 纹理指针

protected:
	/** 如何加载资源,重载 */
	virtual bool LoadImpl(void);
	/** 如何卸载资源,重载 */
	virtual bool UnloadImpl(void);
	/** 从文件中创建纹理 */
	HRESULT CreateTextureFromFile();
	HRESULT CreateTextureFromFileEx();


};

#endif

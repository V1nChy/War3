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

/** 2D �����࣬�̳���GETexture������Resource */
class My2DTexture : public MyTexture  
{
public:
	My2DTexture();
	virtual ~My2DTexture();

	/** �õ�����ָ�� */
	LPDIRECT3DTEXTURE9 GetTexture();

	bool GetRGBAPerPix(vector<MyColor>& vRGBAPixs) const;

protected:
	LPDIRECT3DTEXTURE9 	m_lpTexture;			///< ����ָ��

protected:
	/** ��μ�����Դ,���� */
	virtual bool LoadImpl(void);
	/** ���ж����Դ,���� */
	virtual bool UnloadImpl(void);
	/** ���ļ��д������� */
	HRESULT CreateTextureFromFile();
	HRESULT CreateTextureFromFileEx();


};

#endif

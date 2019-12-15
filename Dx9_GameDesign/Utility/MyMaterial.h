// MyMaterial.h: interface for the MyMaterial class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_MATERIAL_H_
#define _MY_MATERIAL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)

#include "d3dx9.h"
#include "MyColor.h"
#include <string>
#include <vector>
using namespace std;

/*�������*/
enum SceneBlendType
{
	SBT_ADD = 1,
	SBT_MODULATE = 2,		//! GEGraphicLayer���ʵ���������Ҫ����һ�£���Ҫͨ������D3DRS_SRCBLEND��D3DRS_DESTBLEND��
				//  ֵ��ʵ�֡�OGRE�����ó�D3DBLEND_SRCCOLOR��D3DBLEND_INVSRCCOLOR�������Ҿ��ò�̫�ԣ�
				//  �ƺ�Ӧ�����ó�D3DBLEND_DESTCOLOR��D3DBLEND_ZERO
				
	SBT_ALPHA_BLEND = 3,
	SBT_CUSTOM = 4     //�ֶ�����SrcBlend��DestBlend��ֵ
};

/*������*/
typedef struct{
	string TextureName;
	D3DTEXTUREOP ColorOP;
	D3DTEXTUREOP AlphaOP;
}TextureLayer;

/*��Ӱ����*/
enum ShadowType
{
	ST_PLANAR,
	ST_SHADOWED,
	ST_DISABLE
};

/** ������ */
class MyMaterial
{
protected:
	MyColor m_AmbientColor;	//���ʶԻ����ⷴ������
	MyColor m_DiffuseColor;	//���ʶ�����ⷴ������
	MyColor m_SpecularColor;	//���ʵľ��淴������
	MyColor m_EmissiveColor;	//������Է���̶�
	float m_fPower;				// Sharpness if specular highlight 
								//alpha���Ե�reference������ΪDWORD��ȡֵ��Χ��0x00000000~0x000000ff��
								//����û������ֵ����0xff��������Ҫ�����ĳ�0xff��
								//����ʱҪ����255
	DWORD m_fAlphaReference;
	D3DCMPFUNC m_AlphaTestFunc;
	D3DCMPFUNC m_DepthWriteFunc;
	DWORD m_dwAlphaBlendFactor;	//���ʵ�alpha���ϵ��		D3DRS_BLENDFACTOR

	bool m_bDepthCheck;							//�Ƿ����Ȳ���
	bool m_bDepthWrite;							//�Ƿ��������Ȼ������д����
	bool m_bLighting;								//���ö�̬�����Ƿ�Ϊ�˲��ʴ򿪡����off����ʹ���ʱ�������е�ambient, 				 
													//diffuse,specular, emissive��shading������Ч�����������Ĺ����йء�
	bool m_bIsTransparent;					//�Ƿ�͸�������ڲ��ʷ���  
	bool m_bUseAlphaBlendFactor;    //����alpha�Ƿ���Ч��if true����ʵ�alpha���û�ָ��
	bool m_bAlphaTestEnabled;				//alpha�����Ƿ��

	SceneBlendType m_SceneBlend;	//������Ϸ�ʽ
	D3DCULL m_Culling;				//�޳�ģʽ
	D3DTEXTUREFILTERTYPE m_Filtering;		//������˷�ʽ
	D3DSHADEMODE m_Shading;			//����ģʽ

	ShadowType m_eShadowType;

	D3DBLEND m_SrcBlend; //����SceneBlendΪCustomʱ��Ч
	D3DBLEND m_DestBlend;


	vector<TextureLayer> m_Textures;	//�����

protected:
	D3DMATERIAL9 m_lpMaterial;

protected:
	DWORD _GetAlphaReference() const { return m_fAlphaReference; }
	D3DCMPFUNC _GetD3DAlphaTestFunc() const { return m_AlphaTestFunc; }	// ֻ�������ڲ�ʹ�õĹ��к���ǰ��ӡ�_��

public:
	static MyMaterial GUIMaterial;
	static MyMaterial GetGUIMaterial();
public:
	MyMaterial();
	virtual ~MyMaterial();

	//set color
	void SetAmbientColor(MyColor aColor){ m_AmbientColor=aColor; }
	void SetDiffuseColor(MyColor dColor){m_DiffuseColor=dColor;}
	void SetSpecularColor(MyColor sColor){ m_SpecularColor=sColor;}
	void SetEmissiveColor(MyColor eColor) { m_EmissiveColor=eColor;}
	void SetPower(float fPower){ m_fPower=fPower;}


	void SetSrcBlend(D3DBLEND srcBlend) {m_SrcBlend=srcBlend;}
	void SetDestBlend(D3DBLEND destBlend) { m_DestBlend=destBlend;}
	//get the properties of DX material
	D3DMATERIAL9 GetDXMaterial();


	//set the culling mode
	void SetCullingMode( D3DCULL cMode ) { m_Culling=cMode; }
	D3DCULL GetCullingMode() const {return m_Culling;}
		
	//set whether lighting is enabled for the material
	void SetLighting(bool bLightingEnabled)	{ m_bLighting=bLightingEnabled;}
	bool GetLighting() const { return	m_bLighting; }	// ��̬�����Ƿ��
		
	//��������Ϊ͸����͸��
	//�����־���ڻ���ʱ��������˳��ģ��Ȼ���͸�����壬�ڻ�͸�����壩
	void SetTransparent(bool bIsTrans) { m_bIsTransparent=bIsTrans; }
	bool GetTransparent() const{ return m_bIsTransparent;}

	void SetFilteringMode(D3DTEXTUREFILTERTYPE filter)  { m_Filtering=filter;}
	D3DTEXTUREFILTERTYPE GetFilteringMode() const { return m_Filtering;}
	
//	void SetName(string strName){m_strName=strName;}
	void SetDepthCheck(bool bDepthCheck){m_bDepthCheck=bDepthCheck;}
	bool GetDepthCheck() const { return m_bDepthCheck;}
	
	void SetDepthWrite(bool bDepthWrite){m_bDepthWrite=bDepthWrite;}
	bool GetDepthWrite() const { return m_bDepthWrite;}

	void SetDepthWriteFunc(D3DCMPFUNC func){m_DepthWriteFunc=func;}
	D3DCMPFUNC GetDepthWriteFunc () const { return m_DepthWriteFunc;}
	//alpha test,if true,the material will not be paint
	//bool AlphaTest(float fAReference,AlphaTestFunc ATFunction) const;	//! ����������ˣ�ȥ���������
		
	//set the reference of AlphaTest
	void SetAlphaReference(DWORD fAReference) { m_fAlphaReference=fAReference; }
	DWORD GetAlphaReference() const { return m_fAlphaReference;}
		

	void SetShadow(ShadowType shadow) { m_eShadowType = shadow; }
	ShadowType GetShadowType() { return m_eShadowType; }

	//set the alpha blend factor
	void SetAlphaBlendFactor(DWORD dwABFactor) { m_dwAlphaBlendFactor=dwABFactor; }

	void SetSceneBlend(SceneBlendType sceneblend){m_SceneBlend=sceneblend;	}
	SceneBlendType GetSceneBlend() const {return m_SceneBlend;}

	void SetAlphaTest(bool bNeedTest=false) { m_bAlphaTestEnabled=bNeedTest; }
	bool GetAlphaTest() const { return m_bAlphaTestEnabled; }	// AlphaTest�Ƿ��
		
	//ѡ��alpha test function
	void SetAlphaTestFunc(D3DCMPFUNC func) { m_AlphaTestFunc=func; }
	D3DCMPFUNC GetAlphaTestFunc() const {return m_AlphaTestFunc;}

	void SetUseAlphaBlendFactor(bool bUseAlpha) { m_bUseAlphaBlendFactor=bUseAlpha; }
	bool GetUseAlphaBlendFactor() const {return m_bUseAlphaBlendFactor;}

	void SetShading(D3DSHADEMODE SMode) {m_Shading=SMode;}
	D3DSHADEMODE GetShading() const { return m_Shading;}

	void AddTextureLayer();
	void AddTexture(string name);
	void SetTextureColorOP(D3DTEXTUREOP ColorOP);
	void SetTextureAlphaOP(D3DTEXTUREOP AlphaOP);

	MyColor GetAmbient() const { return m_AmbientColor; }
	MyColor GetDiffuse() const { return m_DiffuseColor; }
	MyColor GetSpecular() const { return m_SpecularColor; }
	MyColor GetEmissive() const { return m_EmissiveColor; }
	float GetPower() const { return m_fPower; }
	
	//! ��������ʱ��Ҫ�Ĳ���ҲҪ�ṩget��������getNumTextures();

	void SetFromMaterial();
public:
	static MyMaterial LastMaterial;
	static void RestoreLastMaterial();




};

#endif // !defined(AFX_GEMATERIAL_H__0A39BE48_E369_48A8_9A5C_93760308A66E__INCLUDED_)

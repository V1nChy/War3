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

/*混合类型*/
enum SceneBlendType
{
	SBT_ADD = 1,
	SBT_MODULATE = 2,		//! GEGraphicLayer如何实现这个还需要考虑一下，需要通过设置D3DRS_SRCBLEND和D3DRS_DESTBLEND的
				//  值来实现。OGRE是设置成D3DBLEND_SRCCOLOR和D3DBLEND_INVSRCCOLOR，不过我觉得不太对，
				//  似乎应该设置成D3DBLEND_DESTCOLOR和D3DBLEND_ZERO
				
	SBT_ALPHA_BLEND = 3,
	SBT_CUSTOM = 4     //手动设置SrcBlend和DestBlend的值
};

/*混合组合*/
typedef struct{
	string TextureName;
	D3DTEXTUREOP ColorOP;
	D3DTEXTUREOP AlphaOP;
}TextureLayer;

/*阴影类型*/
enum ShadowType
{
	ST_PLANAR,
	ST_SHADOWED,
	ST_DISABLE
};

/** 材质类 */
class MyMaterial
{
protected:
	MyColor m_AmbientColor;	//材质对环境光反射能力
	MyColor m_DiffuseColor;	//材质对漫射光反射属性
	MyColor m_SpecularColor;	//材质的镜面反向属性
	MyColor m_EmissiveColor;	//物体的自发光程度
	float m_fPower;				// Sharpness if specular highlight 
								//alpha测试的reference，类型为DWORD，取值范围是0x00000000~0x000000ff；
								//如果用户输入的值大于0xff，程序里要把它改成0xff。
								//计算时要除以255
	DWORD m_fAlphaReference;
	D3DCMPFUNC m_AlphaTestFunc;
	D3DCMPFUNC m_DepthWriteFunc;
	DWORD m_dwAlphaBlendFactor;	//材质的alpha混合系数		D3DRS_BLENDFACTOR

	bool m_bDepthCheck;							//是否打开深度测试
	bool m_bDepthWrite;							//是否允许对深度缓存进行写操作
	bool m_bLighting;								//设置动态光照是否为此材质打开。如果off，将使材质本身的所有的ambient, 				 
													//diffuse,specular, emissive和shading属性无效，仅仅与外界的光照有关。
	bool m_bIsTransparent;					//是否透明，便于材质分组  
	bool m_bUseAlphaBlendFactor;    //材质alpha是否生效，if true则材质的alpha由用户指定
	bool m_bAlphaTestEnabled;				//alpha测试是否打开

	SceneBlendType m_SceneBlend;	//场景混合方式
	D3DCULL m_Culling;				//剔除模式
	D3DTEXTUREFILTERTYPE m_Filtering;		//纹理过滤方式
	D3DSHADEMODE m_Shading;			//绘制模式

	ShadowType m_eShadowType;

	D3DBLEND m_SrcBlend; //仅当SceneBlend为Custom时有效
	D3DBLEND m_DestBlend;


	vector<TextureLayer> m_Textures;	//纹理层

protected:
	D3DMATERIAL9 m_lpMaterial;

protected:
	DWORD _GetAlphaReference() const { return m_fAlphaReference; }
	D3DCMPFUNC _GetD3DAlphaTestFunc() const { return m_AlphaTestFunc; }	// 只给引擎内部使用的公有函数前面加“_”

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
	bool GetLighting() const { return	m_bLighting; }	// 动态光照是否打开
		
	//设置物体为透明或不透名
	//这个标志用于绘制时决定绘制顺序的（先画不透明物体，在画透明物体）
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
	//bool AlphaTest(float fAReference,AlphaTestFunc ATFunction) const;	//! 这个函数错了，去掉这个函数
		
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
	bool GetAlphaTest() const { return m_bAlphaTestEnabled; }	// AlphaTest是否打开
		
	//选择alpha test function
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
	
	//! 其他绘制时需要的参数也要提供get函数，如getNumTextures();

	void SetFromMaterial();
public:
	static MyMaterial LastMaterial;
	static void RestoreLastMaterial();




};

#endif // !defined(AFX_GEMATERIAL_H__0A39BE48_E369_48A8_9A5C_93760308A66E__INCLUDED_)

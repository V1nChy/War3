 // GEMaterial.cpp: implementation of the GEMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "MyUtil.h"
#include "MyMaterial.h"
#include "DxGraphicLayer.h"
#include "MyTextureManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//static 变量的定义
MyMaterial MyMaterial::LastMaterial= MyMaterial();
MyMaterial MyMaterial::GUIMaterial= GetGUIMaterial();

MyMaterial::MyMaterial()
{
	m_AmbientColor=MyColor::White;
	m_DiffuseColor= MyColor::White;
	m_SpecularColor= MyColor::White;
	m_fPower=0.0f;
	m_EmissiveColor= MyColor::Black;
	m_fAlphaReference=0;
	m_dwAlphaBlendFactor=0;


	m_AlphaTestFunc=D3DCMP_GREATEREQUAL;
	m_DepthWriteFunc=D3DCMP_LESSEQUAL;
	m_Culling=D3DCULL_CCW;
	m_Filtering=D3DTEXF_POINT;
	m_Shading=D3DSHADE_GOURAUD;
	m_SceneBlend=SBT_ADD;

	m_bAlphaTestEnabled=FALSE;	
	m_bIsTransparent=FALSE;
	m_bLighting=TRUE;
	m_bDepthCheck=TRUE;
	m_bDepthWrite=TRUE;
	m_bUseAlphaBlendFactor=FALSE;

	m_eShadowType = ST_DISABLE;

}

MyMaterial::~MyMaterial()
{
	for (size_t i = 0; i < m_Textures.size(); ++i)
		if (m_Textures[i].TextureName != "")
			MyTextureManager::GetSingleton()->RemoveResource("m_Textures[i].TextureName");

	m_Textures.clear();
}

MyMaterial MyMaterial::GetGUIMaterial()
{
	MyMaterial temp= MyMaterial();

	temp.SetLighting(false);

	temp.SetAlphaTest(true);
	temp.SetAlphaTestFunc(D3DCMP_GREATER);
	temp.SetAlphaReference(0);

	temp.AddTextureLayer();
	temp.SetTextureColorOP(D3DTOP_MODULATE);
	temp.SetTextureAlphaOP(D3DTOP_MODULATE);

	temp.SetTransparent(true);
	temp.SetSceneBlend(SBT_ALPHA_BLEND);


	return temp;
}

D3DMATERIAL9 MyMaterial::GetDXMaterial()
{
	m_lpMaterial.Ambient=m_AmbientColor.GetAsD3DCOLOR();
	m_lpMaterial.Diffuse=m_DiffuseColor.GetAsD3DCOLOR();
	m_lpMaterial.Emissive=m_EmissiveColor.GetAsD3DCOLOR();
	m_lpMaterial.Specular=m_SpecularColor.GetAsD3DCOLOR();
	m_lpMaterial.Power=m_fPower;

	return m_lpMaterial;
}



void MyMaterial::AddTextureLayer()
{
	TextureLayer NewTexture;

	NewTexture.TextureName="";
	NewTexture.ColorOP=D3DTOP_ADD;
	NewTexture.AlphaOP=D3DTOP_DISABLE;

	m_Textures.push_back(NewTexture);
}


void MyMaterial::AddTexture(string name){
	MyTextureManager::GetSingleton()->Create2DTexture(name);
	m_Textures.back().TextureName=name;
}

void MyMaterial::SetTextureColorOP(D3DTEXTUREOP Color){
	m_Textures.back().ColorOP=Color;
}

void MyMaterial::SetTextureAlphaOP(D3DTEXTUREOP Alpha){
	m_Textures.back().AlphaOP=Alpha;
}

void MyMaterial::SetFromMaterial()
{	
	static bool bFirstTime = true;

	DxGraphicLayer *lpgraphiclayer= DxGraphicLayer::GetSingleton();

	lpgraphiclayer->GetD3DDevice()->SetMaterial(&(this->GetDXMaterial()));
	
	if(bFirstTime || LastMaterial.GetDepthCheck()!=m_bDepthCheck)
		lpgraphiclayer->SetRenderState(D3DRS_ZENABLE,m_bDepthCheck);
	if(bFirstTime || LastMaterial.GetDepthWrite()!=m_bDepthWrite)
		lpgraphiclayer->SetRenderState(D3DRS_ZWRITEENABLE,m_bDepthWrite);
	if(bFirstTime || LastMaterial.GetDepthWriteFunc()!=m_DepthWriteFunc)
		lpgraphiclayer->SetRenderState(D3DRS_ZFUNC,m_DepthWriteFunc);

	if(bFirstTime || LastMaterial.GetAlphaTest()!=m_bAlphaTestEnabled)
		lpgraphiclayer->SetRenderState(D3DRS_ALPHATESTENABLE,m_bAlphaTestEnabled);
	if(bFirstTime || LastMaterial.GetAlphaReference()!=m_fAlphaReference)
		lpgraphiclayer->SetRenderState(D3DRS_ALPHAREF,m_fAlphaReference);
	if(bFirstTime || LastMaterial.GetAlphaTestFunc()!=m_AlphaTestFunc)
		lpgraphiclayer->SetRenderState(D3DRS_ALPHAFUNC,m_AlphaTestFunc);

	if(bFirstTime || LastMaterial.GetCullingMode()!=m_Culling)
		lpgraphiclayer->SetRenderState(D3DRS_CULLMODE,m_Culling);
	if(bFirstTime || LastMaterial.GetLighting()!=m_bLighting)
		lpgraphiclayer->SetRenderState(D3DRS_LIGHTING,m_bLighting);
	if(bFirstTime || LastMaterial.GetShading()!=m_Shading)
		lpgraphiclayer->SetRenderState(D3DRS_SHADEMODE,m_Shading);

	vector<TextureLayer>::iterator i;
	int j=0;
	for(i = m_Textures.begin(); i != m_Textures.end(); ++i,++j)
	{
		lpgraphiclayer->GetD3DDevice()->SetSamplerState(j,D3DSAMP_MINFILTER,m_Filtering);
		lpgraphiclayer->GetD3DDevice()->SetSamplerState(j,D3DSAMP_MAGFILTER,m_Filtering);

		if(j==0){
			if(i->TextureName!="")
				lpgraphiclayer->SetTexture(j,i->TextureName);

			lpgraphiclayer->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			lpgraphiclayer->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			lpgraphiclayer->SetTextureStageState(0,D3DTSS_COLOROP,i->ColorOP);
			lpgraphiclayer->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			lpgraphiclayer->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			lpgraphiclayer->SetTextureStageState(0,D3DTSS_ALPHAOP,i->AlphaOP);
			
		}

		else{
			if(i->TextureName!="")
				lpgraphiclayer->SetTexture(j,i->TextureName);

			lpgraphiclayer->SetTextureStageState(j,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			lpgraphiclayer->SetTextureStageState(j,D3DTSS_COLORARG2,D3DTA_CURRENT);
			lpgraphiclayer->SetTextureStageState(j,D3DTSS_COLOROP,i->ColorOP);
			lpgraphiclayer->SetTextureStageState(j,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			lpgraphiclayer->SetTextureStageState(j,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
			lpgraphiclayer->SetTextureStageState(j,D3DTSS_ALPHAOP,i->AlphaOP);
		}
	}

	if (bFirstTime || (m_Textures.empty() && !LastMaterial.m_Textures.empty()))
		lpgraphiclayer->GetD3DDevice()->SetTexture(0, NULL);


	if(m_bIsTransparent){
		if (bFirstTime || LastMaterial.GetTransparent()!=m_bIsTransparent)
			lpgraphiclayer->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);

		if(m_SceneBlend==SBT_CUSTOM){
			if(bFirstTime || LastMaterial.m_SrcBlend!=m_SrcBlend)
				lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,m_SrcBlend);
			if(bFirstTime || LastMaterial.m_SrcBlend!=m_DestBlend)
				lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,m_DestBlend);
		}
		else if(bFirstTime || LastMaterial.GetSceneBlend()!=m_SceneBlend){
			switch(m_SceneBlend){
				case SBT_ADD:		
					lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
					lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
					break;
				case SBT_MODULATE:
					lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR); 
					lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);					
					break;
				case SBT_ALPHA_BLEND:
	//				if(LastMaterial.GetUseAlphaBlendFactor()!=m_bUseAlphaBlendFactor){
						if(m_bUseAlphaBlendFactor){
							lpgraphiclayer->SetRenderState(D3DRS_BLENDFACTOR,m_dwAlphaBlendFactor);
							lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BLENDFACTOR);
							lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVBLENDFACTOR);
						}
						else{
							lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
							lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
						}
	//				}
			}
		}
	}
	else
	{
		if (bFirstTime || LastMaterial.GetTransparent()!=m_bIsTransparent)
			lpgraphiclayer->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	}

	
	LastMaterial=*this;

	bFirstTime = false;
}


void MyMaterial::RestoreLastMaterial()
{
	DxGraphicLayer *lpgraphiclayer=DxGraphicLayer::GetSingleton();

	lpgraphiclayer->GetD3DDevice()->SetMaterial(&(LastMaterial.GetDXMaterial()));

	lpgraphiclayer->SetRenderState(D3DRS_ZENABLE,LastMaterial.m_bDepthCheck);
	lpgraphiclayer->SetRenderState(D3DRS_ZWRITEENABLE,LastMaterial.m_bDepthWrite);
	lpgraphiclayer->SetRenderState(D3DRS_ZFUNC,LastMaterial.m_DepthWriteFunc);


	lpgraphiclayer->SetRenderState(D3DRS_ALPHATESTENABLE,LastMaterial.LastMaterial.m_bAlphaTestEnabled);
	lpgraphiclayer->SetRenderState(D3DRS_ALPHAREF,LastMaterial.m_fAlphaReference);
	lpgraphiclayer->SetRenderState(D3DRS_ALPHAFUNC,LastMaterial.m_AlphaTestFunc);

	lpgraphiclayer->SetRenderState(D3DRS_CULLMODE,LastMaterial.m_Culling);
	lpgraphiclayer->SetRenderState(D3DRS_LIGHTING,LastMaterial.m_bLighting);
	lpgraphiclayer->SetRenderState(D3DRS_SHADEMODE,LastMaterial.m_Shading);

	vector<TextureLayer>::iterator i;
	int j=0;
	for(i = LastMaterial.m_Textures.begin(); i !=LastMaterial.m_Textures.end(); ++i,++j)
	{
		lpgraphiclayer->GetD3DDevice()->SetSamplerState(j,D3DSAMP_MINFILTER,LastMaterial.m_Filtering);
		lpgraphiclayer->GetD3DDevice()->SetSamplerState(j,D3DSAMP_MAGFILTER,LastMaterial.m_Filtering);


	if(j==0){
		if(i->TextureName!="")
			lpgraphiclayer->SetTexture(j,i->TextureName);
		lpgraphiclayer->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		lpgraphiclayer->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
		lpgraphiclayer->SetTextureStageState(0,D3DTSS_COLOROP,i->ColorOP);
		lpgraphiclayer->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		lpgraphiclayer->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
		lpgraphiclayer->SetTextureStageState(0,D3DTSS_ALPHAOP,i->AlphaOP);

	}

	else{
		if(i->TextureName!="")
			lpgraphiclayer->SetTexture(j,i->TextureName);
		lpgraphiclayer->SetTextureStageState(j,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		lpgraphiclayer->SetTextureStageState(j,D3DTSS_COLORARG2,D3DTA_CURRENT);
		lpgraphiclayer->SetTextureStageState(j,D3DTSS_COLOROP,i->ColorOP);
		lpgraphiclayer->SetTextureStageState(j,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		lpgraphiclayer->SetTextureStageState(j,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
		lpgraphiclayer->SetTextureStageState(j,D3DTSS_ALPHAOP,i->AlphaOP);
	}
}

	if(LastMaterial.m_bIsTransparent){
		lpgraphiclayer->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		switch(LastMaterial.m_SceneBlend){
				case SBT_ADD:		
					lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
					lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
					break;
				case SBT_MODULATE:
					lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR); 
					lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);					
					break;
				case SBT_ALPHA_BLEND:
					if(LastMaterial.m_bUseAlphaBlendFactor){
						lpgraphiclayer->SetRenderState(D3DRS_BLENDFACTOR,LastMaterial.m_dwAlphaBlendFactor);
						lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BLENDFACTOR);
						lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVBLENDFACTOR);
					}
					else{
						lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
						lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
					}
					break;
				case SBT_CUSTOM:
					lpgraphiclayer->SetRenderState(D3DRS_SRCBLEND,LastMaterial.m_SrcBlend);
					lpgraphiclayer->SetRenderState(D3DRS_DESTBLEND,LastMaterial.m_DestBlend);
			}
	}
	else
		lpgraphiclayer->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);

}
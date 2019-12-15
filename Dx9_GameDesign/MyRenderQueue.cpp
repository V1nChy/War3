// GERenderQueue.cpp: implementation of the GERenderQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "MyRenderQueue.h"
#include "DxGraphicLayer.h"

//#include "MyParticleSystem.h"
//#include "MySkybox.h"

#pragma warning(disable:4267)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyRenderQueue* MyRenderQueue::m_pRenderQueue = new MyRenderQueue;

MyRenderQueue::MyRenderQueue():
	m_pSkybox(NULL)
{

}

MyRenderQueue::~MyRenderQueue()
{

}

void MyRenderQueue::ClearRenderQueue()
{
	m_NormalRQ.clear();
	m_TransRQ.clear();
	m_PSRQ.clear();

	m_ShadowSrcRQ.clear();
	m_ShadowDestRQ.clear();

	m_pSkybox = NULL;
}

HRESULT MyRenderQueue::Render()
{
	if (m_pSkybox != NULL)
		/*m_pSkybox->Render();*/

	// 普通物体
	int i = m_NormalRQ.size();
	/*while ( --i >= 0 )
	{
		MyObject* obj = m_NormalRQ[i];
		obj->Render();
	}*/
	
	// 半透明物体
	/*i = m_TransRQ.size();
	while ( --i >= 0 )
		m_TransRQ[i]->Render();*/

	// 准备画阴影, 设置被投影物的stencil buffer的状态
	//DxGraphicLayer::GetSingleton()->PrepareForPlanarShadowDest();

	// 平面阴影的投射面
	/*i = m_ShadowDestRQ.size();
	while ( --i >= 0 )
		m_ShadowDestRQ[i]->RenderInStencil();*/

	// 准备画阴影, 设置投影物的stencil buffer的状态
	//DxGraphicLayer::GetSingleton()->PrepareForPlanarShadowSrc();

	// 被投影物
	/*i = m_ShadowSrcRQ.size();
	while ( --i >= 0 )
		m_ShadowSrcRQ[i]->RenderInStencil();*/

	// 结束平面阴影绘制
	//GEGraphicLayer::GetSingleton()->EndPlanarShadow();

	// 粒子系统
	/*i = m_PSRQ.size();
	while ( --i >= 0 )
		m_PSRQ[i]->Render();*/

	// 清空绘制序列
	ClearRenderQueue();

	return S_OK;
}
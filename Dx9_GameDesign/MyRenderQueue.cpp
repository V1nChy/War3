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

	// ��ͨ����
	int i = m_NormalRQ.size();
	/*while ( --i >= 0 )
	{
		MyObject* obj = m_NormalRQ[i];
		obj->Render();
	}*/
	
	// ��͸������
	/*i = m_TransRQ.size();
	while ( --i >= 0 )
		m_TransRQ[i]->Render();*/

	// ׼������Ӱ, ���ñ�ͶӰ���stencil buffer��״̬
	//DxGraphicLayer::GetSingleton()->PrepareForPlanarShadowDest();

	// ƽ����Ӱ��Ͷ����
	/*i = m_ShadowDestRQ.size();
	while ( --i >= 0 )
		m_ShadowDestRQ[i]->RenderInStencil();*/

	// ׼������Ӱ, ����ͶӰ���stencil buffer��״̬
	//DxGraphicLayer::GetSingleton()->PrepareForPlanarShadowSrc();

	// ��ͶӰ��
	/*i = m_ShadowSrcRQ.size();
	while ( --i >= 0 )
		m_ShadowSrcRQ[i]->RenderInStencil();*/

	// ����ƽ����Ӱ����
	//GEGraphicLayer::GetSingleton()->EndPlanarShadow();

	// ����ϵͳ
	/*i = m_PSRQ.size();
	while ( --i >= 0 )
		m_PSRQ[i]->Render();*/

	// ��ջ�������
	ClearRenderQueue();

	return S_OK;
}
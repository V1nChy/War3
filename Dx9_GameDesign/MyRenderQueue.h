// MyRenderQueue.h: interface for the MyRenderQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GERENDERQUEUE_H__6928E383_6087_407E_8946_A8CA58DB92AF__INCLUDED_)
#define AFX_GERENDERQUEUE_H__6928E383_6087_407E_8946_A8CA58DB92AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"

class MyParticleSystem;
class MySkybox;


/** 渲染序列 */
class MyRenderQueue  
{
protected:
	//单实例模式
	static MyRenderQueue* m_pRenderQueue;
	MyRenderQueue();
	virtual ~MyRenderQueue();

	//粒子系统节点
	vector<MyParticleSystem * > m_PSRQ;
	//透明节点
	vector<MyObject * > m_TransRQ;
	//不透明普通节点
	vector<MyObject * > m_NormalRQ;
	//阴影投射物节点
	vector<MyObject * > m_ShadowSrcRQ;
	//阴影投射面节点
	vector<MyObject * > m_ShadowDestRQ;
	//天空盒节点
	MySkybox * m_pSkybox;

public:
	static inline MyRenderQueue* GetSingleton() { return m_pRenderQueue; }

	HRESULT Render();

	void ClearRenderQueue();

	inline size_t PushToTransRQ(MyObject * obj) 
	{ m_TransRQ.push_back(obj); return m_TransRQ.size(); }

	inline size_t PushToNormalRQ(MyObject * obj)
	{ m_NormalRQ.push_back(obj); return m_NormalRQ.size(); }

	inline size_t PushToShadowSrcRQ(MyObject * obj)
	{ m_ShadowSrcRQ.push_back(obj); return m_ShadowSrcRQ.size(); }

	inline size_t PushToShadowDestRQ(MyObject * obj)
	{ m_ShadowDestRQ.push_back(obj); return m_ShadowDestRQ.size(); }

	inline size_t PushToPSRQ(MyParticleSystem* ps)
	{ m_PSRQ.push_back(ps); return m_PSRQ.size(); }

	inline void SetSkybox(MySkybox * skybox)
	{ m_pSkybox = skybox; }

};

#endif // !defined(AFX_GERENDERQUEUE_H__6928E383_6087_407E_8946_A8CA58DB92AF__INCLUDED_)

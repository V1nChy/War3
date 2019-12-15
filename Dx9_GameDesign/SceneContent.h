#ifndef _SCENE_CONTENT_H_
#define _SCENE_CONTENT_H_
#pragma once
#include <windows.h>

class MyGUIManager;
class MySceneManager;
//�������ݶ������
class SceneContent
{
protected:
	MyGUIManager* m_GUI;
	MySceneManager* m_SM;
public:
	SceneContent() :m_GUI(NULL), m_SM(NULL) {}

	virtual HRESULT OneTimeSceneInit() = 0;

	// ��ǰ3D��������ɶ����������
	virtual HRESULT LoadContent()=0;

	virtual void UnloadContent()=0;

	virtual void Update(float dt) = 0;

	virtual void Render() = 0;

	MyGUIManager* GetGUIManager() { return m_GUI; };
	MySceneManager* GetSceneManager() { return m_SM; };
};

#endif
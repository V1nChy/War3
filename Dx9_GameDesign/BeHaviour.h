#ifndef _BEHAVIOUR_H_
#define _BEHAVIOUR_H_

#include "Vector.h"

class MyGUIElementBase;
class MyGUILabel;
class MyGUIImage;
class MyGUIButton;
class MyGUIContainer;

class MyBeHaviour
{
public:
	MyGUIElementBase* pHandleObj;

	virtual void OneTimeInit() = 0;

	virtual void Update() {}

	virtual void Execute1() {}

	virtual void Execute2() {}

	virtual void Execute3() {}
};

class LabelShowFPS :public MyBeHaviour
{
	MyGUILabel* pLabel;
public:

	void Update();

	void OneTimeInit();
};

class UVAni :public MyBeHaviour
{
	MyGUIImage* pImage;

	int m_FPS;
	int m_TotalFPS;

	float m_TargetW;
	float m_TargetH;
	float m_SrcW;
	float m_SrcH;
public:
	UVAni();// pImage->SetUVMap(currentFPS*(m_TargetW / m_SrcW), 0, (currentFPS + 1)*currentFPS*(m_TargetW / m_SrcW), m_TargetH / m_SrcH);

	/** fps:每秒帧数
		n:总共的帧数
	*/
	UVAni(int fps, int n, float tw, float th,
		float sw, float sh);

	void Update();

	void OneTimeInit();

	inline void SetAni(int fps, int n) {
		m_FPS = fps;
		m_TotalFPS = n;
	};
};

class ContainerMoveAni :public MyBeHaviour
{
	MyGUIContainer* pCont;
	bool m_AniPlay;
	float lastTime;
public:
	/* bp:开始位置
	// d：持续时间
	*/
	ContainerMoveAni(float x,float y, float d) :duration(d)
	{
		beginPos.x = x;
		beginPos.y = y;
	}
	Vector3 beginPos;
	Vector3 endPos;
	float duration;
	void OneTimeInit();
	void Update();

	void SetAniPlay();
};

class ClickButton :public MyBeHaviour
{
protected:
	MyGUIButton* pButton;

public:
	virtual void OneTimeInit();
	virtual void Execute1();
	virtual void Execute2();
};

class ClickButtonShow1 :public ClickButton
{
	bool m_AniPlay;
	bool show;
	float lastTime;
public:
	ClickButtonShow1() {}
	ClickButtonShow1(float bx, float by, float ex, float ey, float d) :duration(d)
	{
		beginPos.x = bx;
		beginPos.y = by;

		endPos.x = ex;
		endPos.y = ey;
	}
	MyGUIContainer* pCont;

	Vector3 beginPos;
	Vector3 endPos;

	Vector3 begin;
	Vector3 end;

	float duration;

	void OneTimeInit();
	void Update();
	void Execute1();
	void Execute2();
	void Execute3();
};

class ClickButtonShow2 :public ClickButton
{
	bool m_AniPlay;
	bool show;
	float lastTime;
public:
	ClickButtonShow2() {}
	ClickButtonShow2(float bx, float by, float ex, float ey, float d) :duration(d)
	{
		beginPos.x = bx;
		beginPos.y = by;

		endPos.x = ex;
		endPos.y = ey;
	}
	MyGUIContainer* pCont;

	Vector3 beginPos;
	Vector3 endPos;

	Vector3 begin;
	Vector3 end;

	float duration;

	void OneTimeInit();
	void Update();
	void Execute1();
	void Execute2();
	void Execute3();
};

class ClickButtonChangeScene :public ClickButton
{

public:
	void OneTimeInit();
	void Execute1();
	void Execute2();
};

class ClickButtonQuit :public ClickButton
{
protected:
	MyGUIButton* pButton;

public:
	void Execute1();
	void Execute2();
};

#endif

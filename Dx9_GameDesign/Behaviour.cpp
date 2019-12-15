#include "Behaviour.h"
#include "MyUtil.h"
#include "MyApp.h"

void LabelShowFPS::OneTimeInit()
{
	pLabel = dynamic_cast<MyGUILabel*>(pHandleObj);
}
void LabelShowFPS::Update()
{
	// Keep track of the frame count
	static FLOAT fLastTime = 0.0f;
	static DWORD dwFrames = 0;
	static string strFrameStats = "";
	FLOAT fTime = MyTime::Timer(TIMER_GETABSOLUTETIME);
	++dwFrames;

	// Update the scene stats once per second
	if (fTime - fLastTime > 1.0f)
	{
		float m_fFPS = dwFrames / (fTime - fLastTime);
		fLastTime = fTime;
		dwFrames = 0;

		if (m_fFPS < .1f)
			strFrameStats = "FPS: 0.0";
		else
			strFrameStats = "FPS: " + MyUTIL::FloatToStr(m_fFPS, 1);
	}
	pLabel->SetText(strFrameStats);
}

UVAni::UVAni():m_FPS(1), m_TotalFPS(1), m_TargetW(1),
m_TargetH(1), m_SrcW(1), m_SrcH(1)
{}

UVAni::UVAni(int fps, int n, float tw, float th,
	float sw, float sh) :m_FPS(fps), m_TotalFPS(n),m_TargetW(tw),
	m_TargetH(th),m_SrcW(sw),m_SrcH(sh)
{
	
}

void UVAni::OneTimeInit()
{
	pImage = dynamic_cast<MyGUIImage*>(pHandleObj);
}
void UVAni::Update()
{
	static float second = 1.0f / m_FPS;
	static int currentFPS = 0;

	static float fLastTime = 0.0f;
	float fTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

	// Update the scene stats once per second
	if (fTime - fLastTime > second)
	{
		fLastTime = fTime;
		currentFPS++;
		if (currentFPS == m_TotalFPS)
			currentFPS = 0;
		pImage->SetUVMap(currentFPS*(m_TargetW/m_SrcW), 0, (currentFPS + 1)*(m_TargetW / m_SrcW), m_TargetH/m_SrcH);
	}
}

void ContainerMoveAni::OneTimeInit()
{
	pCont = dynamic_cast<MyGUIContainer*>(pHandleObj);
	m_AniPlay = true;
	lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

	endPos.x = pCont->GetPosition().x;
	endPos.y = pCont->GetPosition().y;
}
void ContainerMoveAni::Update()
{
	if (!m_AniPlay)
		return;

	float time = MyTime::Timer(TIMER_GETABSOLUTETIME);
	if (time - lastTime < duration)
	{
		Vector3 temp = beginPos.Lerp(endPos, (time - lastTime) / duration);
		pCont->SetPosition(temp.x, temp.y);
	}
	else
	{
		m_AniPlay = false;
		pCont->SetPosition(endPos.x, endPos.y);
	}
}
void ContainerMoveAni::SetAniPlay()
{
	m_AniPlay = true;
	lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);
}

void ClickButton::OneTimeInit()
{
	pButton = dynamic_cast<MyGUIButton*>(pHandleObj);
}
void ClickButton::Execute1()
{
	pButton->Move(-2, 5);
}
void ClickButton::Execute2()
{
	pButton->Move(2, -5);
}

void ClickButtonShow1::OneTimeInit()
{
	ClickButton::OneTimeInit();
	m_AniPlay = false;
	show = false;
}
void ClickButtonShow1::Execute1()
{
	ClickButton::Execute1();
}
void ClickButtonShow1::Execute2()
{
	if(MyGUIManager::GetCurrentGUIManager()->GetContainer("Help")->IsVisible())
		MyGUIManager::GetCurrentGUIManager()->GetContainer("Menu")->GetElement("HelpBtn")->GetBehaviour()->Execute3();
	
	if (pCont != NULL && show)
	{
		show = false;
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = endPos;
		end = beginPos;
	}
	else if (pCont != NULL && !show)
	{
		show = true;
		pCont->SetVisible(true);
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = beginPos;
		end = endPos;
	}
	ClickButton::Execute2();
}
void ClickButtonShow1::Execute3()
{
	
	if (pCont != NULL && show)
	{
		show = false;
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = endPos;
		end = beginPos;
	}
	else if (pCont != NULL && !show)
	{
		show = true;
		pCont->SetVisible(true);
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = beginPos;
		end = endPos;
	}
}
void ClickButtonShow1::Update()
{
	if (!m_AniPlay)
		return;

	float time = MyTime::Timer(TIMER_GETABSOLUTETIME);
	if (time - lastTime < duration)
	{
		Vector3 temp = begin.Lerp(end, (time - lastTime) / duration);
		pCont->SetPosition(temp.x, temp.y);
	}
	else
	{
		m_AniPlay = false;
		pCont->SetPosition(end.x, end.y);

		if (!show)
			pCont->SetVisible(false);
	}
}

void ClickButtonShow2::OneTimeInit()
{
	ClickButton::OneTimeInit();
	m_AniPlay = false;
	show = false;
}
void ClickButtonShow2::Execute1()
{
	ClickButton::Execute1();
}
void ClickButtonShow2::Execute2()
{
	if(MyGUIManager::GetCurrentGUIManager()->GetContainer("Option")->IsVisible())
		MyGUIManager::GetCurrentGUIManager()->GetContainer("Menu")->GetElement("SelectBtn")->GetBehaviour()->Execute3();
	
	if (pCont != NULL && show)
	{
		show = false;
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = endPos;
		end = beginPos;
	}
	else if (pCont != NULL && !show)
	{
		show = true;
		pCont->SetVisible(true);
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = beginPos;
		end = endPos;
	}
	ClickButton::Execute2();
}
void ClickButtonShow2::Execute3()
{

	if (pCont != NULL && show)
	{
		show = false;
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = endPos;
		end = beginPos;
	}
	else if (pCont != NULL && !show)
	{
		show = true;
		pCont->SetVisible(true);
		m_AniPlay = true;
		lastTime = MyTime::Timer(TIMER_GETABSOLUTETIME);

		begin = beginPos;
		end = endPos;
	}
}
void ClickButtonShow2::Update()
{
	if (!m_AniPlay)
		return;

	float time = MyTime::Timer(TIMER_GETABSOLUTETIME);
	if (time - lastTime < duration)
	{
		Vector3 temp = begin.Lerp(end, (time - lastTime) / duration);
		pCont->SetPosition(temp.x, temp.y);
	}
	else
	{
		m_AniPlay = false;
		pCont->SetPosition(end.x, end.y);

		if (!show)
			pCont->SetVisible(false);
	}
}

void ClickButtonChangeScene::OneTimeInit()
{
	ClickButton::OneTimeInit();
	
}
void ClickButtonChangeScene::Execute1()
{
	ClickButton::Execute1();
}
void ClickButtonChangeScene::Execute2()
{
	MyApp* pApp = (MyApp*)MyApp::GetSingleton();
	pApp->change = true;

	ClickButton::Execute2();
}

void ClickButtonQuit::Execute1()
{
	ClickButton::Execute1();
}
void ClickButtonQuit::Execute2()
{
	ClickButton::Execute2();
	PostQuitMessage(0);
}
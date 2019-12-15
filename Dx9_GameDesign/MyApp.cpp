#include "MyApp.h"
#include "DxGraphicLayer.h"
#include "MyTextureManager.h"
#include "MyMouse.h"

MyApp::MyApp():
	m_CurretGUI(NULL),
	m_CurretSM(NULL),
	m_CurCont(NULL)
{}
MyApp::~MyApp(){}

HRESULT MyApp::OneTimeSceneInit()
{
	MyGUIContainer* pCtr;
	MyGUIImage* pImage;

	m_Load = new MyGUIManager();
	MyGUIManager::SetCurrentGUIManager(m_Load);

	//新建一个容器，为背景UI容器
	pCtr = m_Load->CreateContainer("Load");
	//新建一个图片
	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/loadBg.jpg");
	pImage->SetSize(EngineFrame::GetSingleton()->GetWindowedWidth(), EngineFrame::GetSingleton()->GetWindowedHeight());

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/loadBar.png");
	pImage->SetPosition(160,720);
	pImage->SetSize(1200,30);

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetName("londingBar");
	pImage->ResetImage("Texture/loadBarFill.PNG");
	pImage->SetPosition(160, 720);
	/*pImage->SetSize(950, 15);*/
	pImage->SetSize(0, 30);

	m_Load->OneTimeInit();

	MyFontManager::GetSingleton()->OneTimeSceneInit();
	MyTextureManager::GetSingleton()->OneTimeInit();
	MyMouse::GetSingleton()->OnCreateMouse(m_hWnd, EngineFrame::GetSingleton()->GetWindowBounds());

	m_MainMenu.OneTimeSceneInit();
	m_GameScene.OneTimeSceneInit();

	m_CurretGUI = m_MainMenu.GetGUIManager();
	m_CurretSM = m_MainMenu.GetSceneManager();
	m_CurCont = &m_MainMenu;
	MyGUIManager::SetCurrentGUIManager(m_CurretGUI);
	return S_OK;
}
HRESULT MyApp::InitDeviceObjects()
{
	RenderLoading();
	MyFontManager::GetSingleton()->InitDeviceObjects();
	RenderLoading();
	MyTextureManager::GetSingleton()->InitDeviceObjects();
	RenderLoading();
	m_MainMenu.LoadContent();
	RenderLoading();
	m_GameScene.LoadContent();
	RenderLoading(true);

	return S_OK;
}
HRESULT MyApp::RestoreDeviceObjects()
{
	
	return S_OK;
}
HRESULT MyApp::FrameMove()
{
	if(change)
	{
		m_CurretGUI = m_GameScene.GetGUIManager();
		m_CurretSM = m_GameScene.GetSceneManager();
		m_CurCont = &m_GameScene;
		change = false;
	}
	MyMouse::GetSingleton()->Update(m_hWnd);

	m_CurretGUI->FrameMove(m_fElapsedTime);
	m_CurretSM->FrameMove(m_fElapsedTime);
	m_CurCont->Update(m_fElapsedTime);
	return S_OK;
}
void MyApp::RenderLoading(bool isEnd)
{
	static MSG msg = { 0,0,0,0,0,{ 0,0 } };

	static POINT pt = { -1,-1 };


	if (msg.message == WM_QUIT)
	{
		::PostQuitMessage(0);
		return;
	}

	if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	static int barWidth = 0;
	static float lastTime = (float)timeGetTime();
	static float beginTime = (float)timeGetTime();
	float currTime = 0.0f;
	float timeDelta = 0.0f;

	if (!isEnd)
	{
		currTime = (float)timeGetTime();

		timeDelta = (currTime - lastTime);

		if (timeDelta < 15)	return;

		lastTime = currTime;

		barWidth += 1;
		if (barWidth > 1200)
			barWidth = 1200;
	}
	else
	{
		currTime = (float)timeGetTime();

		timeDelta = (currTime - beginTime);

		barWidth = 1200;
	}

	MyGUIImage* pImage = (MyGUIImage*)m_Load->GetContainer("Load")->GetElement("londingBar");
	pImage->SetSize(barWidth, 30);
	pImage->SetUVMap(0, 0, barWidth / 1200.0f, 1);

	pImage = (MyGUIImage*)m_Load->GetContainer("Load")->GetElement("londingBarShine");

	static MyColor color(0.3f, 0.3f, 0.3f);
	DxGraphicLayer::GetSingleton()->Clear(true, true, color, 1);
	DxGraphicLayer::GetSingleton()->ClearZBuffer(1.f);
	// Begin the scene
	if (DxGraphicLayer::GetSingleton()->BeginScene())
	{
		m_Load->RenderBackgroundGUI();
		m_Load->RenderForwardGUI();

		DxGraphicLayer::GetSingleton()->EndScene();
	}
	DxGraphicLayer::GetSingleton()->Present();

	if (isEnd)
		Sleep(timeDelta>2000 ? 1 : 2000 - timeDelta);
}
HRESULT MyApp::Render()
{
	// Clear the viewport
	static MyColor color(0.3f, 0.3f, 0.3f);
	DxGraphicLayer::GetSingleton()->Clear(true, true, color, 1);

	// Begin the scene
	if (DxGraphicLayer::GetSingleton()->BeginScene())
	{
		//将对象加入到渲染队列（剔除相机外的物体）
		//m_CurretSM->Render();
		RenderAll();

		DxGraphicLayer::GetSingleton()->EndScene();
	}
	return S_OK;
}
void MyApp::RenderAll()
{
	m_CurretGUI->RenderBackgroundGUI();
	DxGraphicLayer::GetSingleton()->ClearZBuffer(1.f);

	//MyRenderQueue::GetSingleton().Render();

	m_CurCont->Render();
	m_CurretGUI->RenderForwardGUI();
	MyMouse::GetSingleton()->ShowMouse();

	//显示帧率
	MyFontManager::GetSingleton()->m_pSystemFont->DrawText(m_strFrameStats, MyColor::White, 0, 0);
}

HRESULT MyApp::InvalidateDeviceObjects()
{
	
	return S_OK;
}
HRESULT MyApp::DeleteDeviceObjects()
{
	
	return S_OK;
}
void MyApp::FinalCleanup()
{
	m_MainMenu.UnloadContent();
	m_GameScene.UnloadContent();
	MyFontManager::GetSingleton()->FinalCleanup();
	MyTextureManager::GetSingleton()->FinalCleanup();
	MySceneManager::GetSingleton()->FinalCleanup();
}
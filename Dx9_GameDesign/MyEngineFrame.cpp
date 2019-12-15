// GEApp.cpp: implementation of the GEApp class.
//
//////////////////////////////////////////////////////////////////////
#define IDM_EXIT                        40002
#include "MyEngineFrame.h"
#include "DxGraphicLayer.h"
#include "MyUTIL.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EngineFrame* EngineFrame::m_pApp = NULL;

//让普通的消息回调函数指向我们引擎框架的处理函数
LRESULT CALLBACK WndProc (HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return EngineFrame::GetSingleton()->MsgProc(hWindow, iMessage, wParam, lParam);
}

/* Our message handler */
LRESULT EngineFrame::MsgProc(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	case WM_NCMOUSEMOVE:	//鼠标若是在非客户区上
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	case WM_SETCURSOR:		//鼠标位于窗体上
		SetCursor(NULL);	//屏蔽鼠标
		break;
	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		Pause(true);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
			// Recieved key/menu command to exit app
			SendMessage(hWindow, WM_CLOSE, 0, 0);
			return 0;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWindow);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWindow, iMessage, wParam, lParam);
	}
	return 0;
}

EngineFrame::EngineFrame() : m_strClassName("My Engine")
{
	m_pApp = this;

    m_hWnd              = NULL;
    m_hWndFocus         = NULL;
    m_bWindowed         = true;
    m_bActive           = false;
    m_bDeviceLost       = false;
    m_bMinimized        = false;
    m_bMaximized        = false;
    m_bIgnoreSizeChange = false;
    m_bDeviceObjectsInited = false;
    m_bDeviceObjectsRestored = false;
    m_dwCreateFlags     = 0;
	m_bStartFullScreen	= false;

    m_strWindowTitle    = "My Engine";
    m_dwCreationWidth   = 640;
    m_dwCreationHeight  = 480;

    m_fTime             = 0.0f;
    m_fElapsedTime      = 0.0f;
    m_fFPS              = 0.0f;
    m_strFrameStats     = "";

	Pause( true ); // 进行资源加载
}

//-----------------------------------------------------------------------------
// Name: Create()
// Desc: 创建窗口和图形设备
// width: 客户区宽
// height: 客户区高
//-----------------------------------------------------------------------------
HRESULT EngineFrame::Create (HINSTANCE hInstance, int width, int height, LPCSTR lpszTitleName)
{
	// 窗口类的结构填充
	WNDCLASSEX	WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	/* System properties */
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = m_strClassName.c_str();
	/* Extra properties */
	WndClass.lpszMenuName = NULL;
	WndClass.cbClsExtra = NULL;
	WndClass.cbWndExtra = NULL;
	WndClass.style = NULL;

	// 向操作系统注册这个窗口类
	if (!RegisterClassEx(&WndClass))
	{
		LOGINFO("你需要 Windows NT 以上的操作系统!");
		return S_FALSE;
	}

	m_strWindowTitle = lpszTitleName;			//设置窗口标题
	m_dwCreationWidth = width;					//设置窗口宽
	m_dwCreationHeight = height;				//设置窗口高


	// 窗口模式的一个样式
	m_dwWindowStyle = WS_POPUP | WS_VISIBLE;

	RECT r =
	{
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2 + width,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2 + height
	};
	AdjustWindowRect(&r, m_dwWindowStyle, FALSE);

	//创建窗口
	m_hWnd = CreateWindowA(m_strClassName.c_str(), m_strWindowTitle.c_str(), m_dwWindowStyle, 
		r.left, r.top, r.right - r.left, r.bottom - r.top,
		NULL, NULL, hInstance, NULL);

	if (!m_hWnd)
	{
		// 窗口创建失败
		LOGINFO("窗口创建失败!");
		return S_FALSE;
	}
	
    // The focus window can be a specified to be a different window than the
    // device window.  If not, use the device window as the focus window. 
    if( m_hWndFocus == NULL )
        m_hWndFocus = m_hWnd;

    // Save window properties
    m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );
	
	// Initialize the application timer
	MyTime::Timer(TIMER_RESET);

	// Initialize the app's custom scene stuff
	if (FAILED(OneTimeSceneInit()))
	{
		DxGraphicLayer::GetSingleton()->ReleaseD3D9();
		LOGINFO("全局初始化出错，必须退出！")
			return E_FAIL;
	}

    // Initialize the 3D environment for the app
    if( FAILED(_Initialize3DEnvironment()) )
    {
		DxGraphicLayer::GetSingleton()->FinalCleanup();
		LOGINFO(0, "3D设备初始化出错，必须退出！", 0, 0);
        return E_FAIL;
    }

	return S_OK;
}

/* Real time message loop */
void EngineFrame::Run (void)
{
	// The app is ready to go
	Pause(false);
	bool bGotMsg = false;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
	
	while (WM_QUIT != msg.message)
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if( m_bActive )
			bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
		else
			bGotMsg = ( GetMessage( &msg, NULL, 0U, 0U ) != 0 );

		if( bGotMsg )
		{
              TranslateMessage( &msg );
              DispatchMessage( &msg );	
		}
		else
		{
//			/* No message, do frame */
			if( m_bDeviceLost )
			{
				// Yield some CPU time to other processes
				Sleep( 100 ); // 100 milliseconds
			}
			// Render a frame during idle time (no messages are waiting)
			if( m_bActive )
			{
				_Render3DEnvironment();
			}
		}
	}
	Shutdown();
}


//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
void EngineFrame::Pause( bool bPause )
{
    static DWORD dwAppPausedCount = 0;

    dwAppPausedCount += ( bPause ? +1 : -1 );
    m_bActive         = ( dwAppPausedCount ? false : true );

    // Handle the first pause request (of many, nestable pause requests)
    if( bPause && ( 1 == dwAppPausedCount ) )
    {
        // Stop the scene from animating
		MyTime::Timer( TIMER_STOP );
    }

    if( 0 == dwAppPausedCount )
    {
        // Restart the timers
		MyTime::Timer( TIMER_START );
    }
}
void EngineFrame::Shutdown()
{
	FinalCleanup();
	DxGraphicLayer::GetSingleton()->FinalCleanup();
}

bool EngineFrame::ResetWindowPos()
{
	if(m_bWindowed)
	{
        return SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                      m_rcWindowBounds.left, m_rcWindowBounds.top,
                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                      SWP_SHOWWINDOW );
	}

	return true;
}

HRESULT EngineFrame::_Render3DEnvironment()
{
	HRESULT hr;

	// Get the app's time, in seconds. Skip rendering if no time elapsed
	FLOAT fAppTime = MyTime::Timer(TIMER_GETAPPTIME);
	FLOAT fElapsedAppTime = MyTime::Timer(TIMER_GETELAPSEDTIME);
	if (0.0f == fElapsedAppTime)
		return S_OK;

	// FrameMove (animate) the scene
	// Store the time for the app
	m_fTime = fAppTime;
	m_fElapsedTime = fElapsedAppTime;

	// Frame move the scene
	if (FAILED(hr = FrameMove()))
		return hr;

	// Render the scene as normal
	if (FAILED(hr = Render()))
		return hr;

	_UpdateStats();
	// Show the frame on the primary surface.
	hr = DxGraphicLayer::GetSingleton()->Present();
	if (D3DERR_DEVICELOST == hr)
		m_bDeviceLost = true;

	return S_OK;
}

HRESULT EngineFrame::_AdjustWindowForChange()
{
	if (m_bWindowed)
	{
		// Set windowed-mode style
		SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle);
	}
	else
	{
		// Set fullscreen-mode style
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
	}
	return S_OK;
}

HRESULT EngineFrame::_Initialize3DEnvironment()
{
	HRESULT hr = E_FAIL;

	//peter 从外部得到改变后的是否是窗口, 赋给m_bWindowed

	// 根据 windowed/fullscreen 调整窗口属性
	_AdjustWindowForChange();

	// 设置D3D的环境, 并创建设备
	hr = DxGraphicLayer::GetSingleton()->Initialize3DEnvironment(m_hWnd);

	if (SUCCEEDED(hr))
	{
		// Store device Caps & render target surface desc
		// PETER: m_dwCreateFlags = behaviorFlags;

		// Initialize the app's device-dependent objects
		if (FAILED(InitDeviceObjects()))
		{
			DeleteDeviceObjects();
		}
		else
		{
			m_bDeviceObjectsInited = true;
			hr = RestoreDeviceObjects();
			if (FAILED(hr))
			{
				InvalidateDeviceObjects();
			}
			else
			{
				m_bDeviceObjectsRestored = true;
				return S_OK;
			}
		}

		// Cleanup before we try again
		_Cleanup3DEnvironment();
	}

	return hr;
}

HRESULT EngineFrame::_Cleanup3DEnvironment()
{
	if (DxGraphicLayer::GetSingleton()->GetD3DDevice())
	{
		if (m_bDeviceObjectsRestored)
		{
			m_bDeviceObjectsRestored = false;
			InvalidateDeviceObjects();
		}
		if (m_bDeviceObjectsInited)
		{
			m_bDeviceObjectsInited = false;
			DeleteDeviceObjects();
		}

		DxGraphicLayer::GetSingleton()->ReleaseD3D9Device();
	}

	return S_OK;
}

void EngineFrame::_UpdateStats()
{
	// Keep track of the frame count
	static FLOAT fLastTime = 0.0f;
	static DWORD dwFrames = 0;
	FLOAT fTime = MyTime::Timer(TIMER_GETABSOLUTETIME);
	++dwFrames;

	// Update the scene stats once per second
	if (fTime - fLastTime > 1.0f)
	{
		m_fFPS = dwFrames / (fTime - fLastTime);
		fLastTime = fTime;
		dwFrames = 0;

		if (m_fFPS < .1f)
			m_strFrameStats = "FPS: 0.0";
		else
			m_strFrameStats = "FPS: " + MyUTIL::FloatToStr(m_fFPS, 1);
	}
}

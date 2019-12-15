#include"Dx9DemoBase.h"
#include "MyMouse.h"

IDirect3DDevice9*		g_pDevice;
Dx9DemoBase* Dx9DemoBase::m_pInstance = NULL;
Dx9DemoBase::Dx9DemoBase() :
	m_hwnd(NULL),
	m_hInstance(NULL),
	m_pD3D(NULL),
	m_pDevice(NULL),
	m_FPS(0),
	m_DurationTime(0)
{

}

Dx9DemoBase::~Dx9DemoBase()
{
	Shutdown();
}

LRESULT CALLBACK Dx9DemoBase::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_pInstance->MsgProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Dx9DemoBase::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

bool Dx9DemoBase::Create(HINSTANCE hInstance, int width, int height, LPCSTR lpszWndName)
{

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "DX9WindowClass";

	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(0, "注册窗口失败", 0, 0);
		return false;
	}


	LONG cpSzie = GetSystemMetrics(SM_CYCAPTION);

	LONG xdlgSzie = GetSystemMetrics(SM_CXDLGFRAME);

	LONG ydlgSzie = GetSystemMetrics(SM_CXDLGFRAME);

	LONG exW = xdlgSzie * 2 + width;

	LONG exH = ydlgSzie * 2 + cpSzie + height;

	LONG scSzieW = GetSystemMetrics(SM_CXSCREEN);

	LONG scSzieH = GetSystemMetrics(SM_CYSCREEN);

	LONG PosX = (scSzieW - exW) / 2;

	LONG PosY = (scSzieH - exH) / 2 - cpSzie;




	m_hwnd = CreateWindowA(
		"DX9WindowClass",
		"aaa",
		WS_POPUP | WS_VISIBLE,
		0, 0, exW, exH,
		NULL, NULL, hInstance, NULL);

	if (!m_hwnd)
	{
		MessageBox(0, "创建窗口失败", 0, 0);
		return false;
	}


	m_width = width;
	m_height = height;

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);


	//初始化设备
	bool result = InitDevice(hInstance, m_hwnd);
	if (result == false)
	{
		MessageBox(0, "设备初始化失败", 0, 0);
		return false;
	}

	//载入内容
	result = LoadContent();
	if (result == false)
	{
		MessageBox(0, "内容载入失败", 0, 0);
		return false;
	}

	return true;
}

WPARAM Dx9DemoBase::Run()
{
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mStartCount);

	static LARGE_INTEGER lastCounter = mStartCount;
	LARGE_INTEGER currentCounter;
	float timeDelta = 0.0f;

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));


	//m_Mouse.Acquire();
	MyMouse::GetSingleton()->OnCreateMouse(m_hwnd,0,0,m_width,m_height);

	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//m_MousePT = msg.pt;
			//ScreenToClient(m_hwnd, &m_MousePT);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//计算帧率
		QueryPerformanceCounter(&currentCounter);
		timeDelta = (float)(currentCounter.LowPart - lastCounter.LowPart) / mFrequency.LowPart;
		m_FPS = 1.0f / timeDelta;
		lastCounter = currentCounter;
		m_DurationTime += timeDelta;

		MyMouse::GetSingleton()->Update(m_hwnd);

		Update(timeDelta);
		Render();

	}
	Shutdown();
	return int(msg.wParam);
}

bool Dx9DemoBase::InitDevice(HINSTANCE hInstance, HWND hwnd)
{
	m_hInstance = hInstance;
	m_hwnd = hwnd;

	//创建Direct3D对象，并获取接口IDirect3D9的指针
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	//判断显卡是否支持硬件顶点处理
	D3DCAPS9 caps;

	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	//取得目标桌面显示模式
	D3DDISPLAYMODE d3ddm;
	if (m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm) != D3D_OK)
		return false;

	//建立绘图设备
	//D3DPRESENT_PARAMETERS d3dpp;
	//memset(&d3dpp, 0, sizeof(d3dpp));
	//d3dpp.Windowed = TRUE; //创建窗口模式的Direct3D程序
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.BackBufferFormat = d3ddm.Format;

	////d3dpp.MultiSampleType = multiType;//多重采样

	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//创建32位深度/模板缓存，24位深度，8为模板
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));
	d3dpp.Windowed = TRUE; //创建窗口模式的Direct3D程序
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//创建设备对象，并获取接口IDirect3DDevice9的指针
	if (m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, //使用缺省的显卡
		D3DDEVTYPE_HAL,
		m_hwnd,
		vp,
		&d3dpp,
		&m_pDevice) != D3D_OK)
		return false;
	g_pDevice = m_pDevice;
	return true;
}

void Dx9DemoBase::Shutdown()
{
	UnloadContent();

	if (m_pDevice)
		m_pDevice->Release();
	if (m_pD3D)
		m_pD3D->Release();

	m_pDevice = NULL;
	m_pD3D = NULL;
}

bool Dx9DemoBase::LoadContent()
{
	return true;
}

void Dx9DemoBase::UnloadContent()
{}

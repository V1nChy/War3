#ifndef _DEMO_BASE_H_
#define _DEMO_BASE_H_

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

class Dx9DemoBase
{
protected:
	static Dx9DemoBase* m_pInstance;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int					m_width;	//窗口宽				
	int					m_height;	//窗口高

	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStartCount;
	float				m_FPS;
	float				m_DurationTime;

	HWND m_hwnd;
	HINSTANCE m_hInstance;
	IDirect3D9* m_pD3D;				//Direct3D对象的接口指针
	IDirect3DDevice9* m_pDevice;		//设备对象的接口指针
	LPDIRECT3DDEVICE9 pDevice;
	
public:
	Dx9DemoBase();
	virtual ~Dx9DemoBase();

	//创建程序
	bool Create(HINSTANCE hInstance, int width, int height, LPCSTR lpszWndName);

	//初始化Dx9
	bool InitDevice(HINSTANCE hInstance, HWND hwnd);

	//程序循环
	WPARAM Run();

	//释放资源
	void Shutdown();

	//消息处理函数
	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual bool LoadContent();

	virtual void UnloadContent();

	virtual void Update(float dt) = 0;

	virtual void Render() = 0;

	inline IDirect3DDevice9* GetD3DDevice()const { return m_pDevice; };
};

#endif

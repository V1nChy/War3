// MyEngineFrame.h: interface for the MyEngineFrame class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable:4786)
#include <windows.h>
#include <string>

using namespace std;

/** 此类封装了引擎的底层应用框架 */
class EngineFrame  
{
private:
	const string m_strClassName;				//窗口类名
protected:
	static EngineFrame* m_pApp;

	HWND		m_hWnd;
	HWND		m_hWndFocus;

	// 应用程序的内部变量
	bool              m_bWindowed;				// 是否为窗口模式
	bool              m_bActive;				// 窗口是否被激活
	bool              m_bDeviceLost;			// 如果设备丢失，使用Restore device	
	bool              m_bMinimized;				// 是否最小化
	bool              m_bMaximized;				// 是否最大化
	bool              m_bIgnoreSizeChange;		// 是否不理睬用户改变窗口
	bool              m_bDeviceObjectsInited;	// 是否设备被初始化
	bool              m_bDeviceObjectsRestored;	// 设备是否还原

	DWORD             m_dwCreateFlags;     // Indicate sw or hw vertex processing
	DWORD             m_dwWindowStyle;     // Saved window style for mode switches
	RECT              m_rcWindowBounds;    // Saved window bounds for mode switches
	RECT              m_rcWindowClient;    // Saved client area size for mode switches

	string			  m_strWindowTitle;    // Title for the app's window
	UINT			  m_dwCreationWidth;   // Width used to create window
	UINT              m_dwCreationHeight;  // Height used to create window

										   // Variables for timing
	FLOAT             m_fTime;             // Current time in seconds
	FLOAT             m_fElapsedTime;      // Time elapsed since last frame
	FLOAT             m_fFPS;              // Instanteous frame rate
	string			  m_strFrameStats;	   // String to hold frame stats

	bool			  m_bStartFullScreen;	//全屏

	HRESULT _Render3DEnvironment();
	HRESULT _AdjustWindowForChange();
	HRESULT _Initialize3DEnvironment();
	HRESULT _Cleanup3DEnvironment();
	void _UpdateStats();
public:
	EngineFrame();
	virtual ~EngineFrame()	{}
	inline static EngineFrame* GetSingleton() { return m_pApp; }

	HRESULT Create(HINSTANCE hInstance, const int width=640, const int height=480,const LPCSTR lpszTitleName="My Engine");			//创建窗口
	void Run();									//运行程序
	void Pause(bool bPause);						//暂停程序
	void Shutdown();

	bool ResetWindowPos();							//重置窗口位置
                                   
	virtual LRESULT MsgProc(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam);

    // Overridable functions for the 3D scene created by the app
    virtual HRESULT OneTimeSceneInit()									= 0;
    virtual HRESULT InitDeviceObjects()									= 0;
    virtual HRESULT RestoreDeviceObjects()								= 0;
    virtual HRESULT FrameMove()											= 0;
    virtual HRESULT Render()											= 0;
    virtual HRESULT InvalidateDeviceObjects()							= 0;
    virtual HRESULT DeleteDeviceObjects()								= 0;
    virtual void FinalCleanup()											= 0;

	inline HWND GetHWND() const { return m_hWnd; }
	inline HWND GetHWNDFocus() const { return m_hWndFocus; }

	/** 得到窗口大小 */
	inline int GetWidth() { if (m_bWindowed) return GetWindowedWidth(); else return GetFullScreenWidth(); }
	inline int GetHeight() { if (m_bWindowed) return GetWindowedHeight(); else return GetFullScreenHeight(); }
	inline int GetWindowedWidth() { return m_rcWindowClient.right - m_rcWindowClient.left; }
	inline int GetWindowedHeight() { return m_rcWindowClient.bottom - m_rcWindowClient.top; }
	inline int GetFullScreenWidth() { return m_dwCreationWidth; }
	inline int GetFullScreenHeight() { return m_dwCreationHeight; }
	inline RECT GetWindowBounds() { return m_rcWindowBounds; }

};




// MyEngineFrame.h: interface for the MyEngineFrame class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning(disable:4786)
#include <windows.h>
#include <string>

using namespace std;

/** �����װ������ĵײ�Ӧ�ÿ�� */
class EngineFrame  
{
private:
	const string m_strClassName;				//��������
protected:
	static EngineFrame* m_pApp;

	HWND		m_hWnd;
	HWND		m_hWndFocus;

	// Ӧ�ó�����ڲ�����
	bool              m_bWindowed;				// �Ƿ�Ϊ����ģʽ
	bool              m_bActive;				// �����Ƿ񱻼���
	bool              m_bDeviceLost;			// ����豸��ʧ��ʹ��Restore device	
	bool              m_bMinimized;				// �Ƿ���С��
	bool              m_bMaximized;				// �Ƿ����
	bool              m_bIgnoreSizeChange;		// �Ƿ�����û��ı䴰��
	bool              m_bDeviceObjectsInited;	// �Ƿ��豸����ʼ��
	bool              m_bDeviceObjectsRestored;	// �豸�Ƿ�ԭ

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

	bool			  m_bStartFullScreen;	//ȫ��

	HRESULT _Render3DEnvironment();
	HRESULT _AdjustWindowForChange();
	HRESULT _Initialize3DEnvironment();
	HRESULT _Cleanup3DEnvironment();
	void _UpdateStats();
public:
	EngineFrame();
	virtual ~EngineFrame()	{}
	inline static EngineFrame* GetSingleton() { return m_pApp; }

	HRESULT Create(HINSTANCE hInstance, const int width=640, const int height=480,const LPCSTR lpszTitleName="My Engine");			//��������
	void Run();									//���г���
	void Pause(bool bPause);						//��ͣ����
	void Shutdown();

	bool ResetWindowPos();							//���ô���λ��
                                   
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

	/** �õ����ڴ�С */
	inline int GetWidth() { if (m_bWindowed) return GetWindowedWidth(); else return GetFullScreenWidth(); }
	inline int GetHeight() { if (m_bWindowed) return GetWindowedHeight(); else return GetFullScreenHeight(); }
	inline int GetWindowedWidth() { return m_rcWindowClient.right - m_rcWindowClient.left; }
	inline int GetWindowedHeight() { return m_rcWindowClient.bottom - m_rcWindowClient.top; }
	inline int GetFullScreenWidth() { return m_dwCreationWidth; }
	inline int GetFullScreenHeight() { return m_dwCreationHeight; }
	inline RECT GetWindowBounds() { return m_rcWindowBounds; }

};




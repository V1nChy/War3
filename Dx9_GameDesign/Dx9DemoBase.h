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

	int					m_width;	//���ڿ�				
	int					m_height;	//���ڸ�

	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStartCount;
	float				m_FPS;
	float				m_DurationTime;

	HWND m_hwnd;
	HINSTANCE m_hInstance;
	IDirect3D9* m_pD3D;				//Direct3D����Ľӿ�ָ��
	IDirect3DDevice9* m_pDevice;		//�豸����Ľӿ�ָ��
	LPDIRECT3DDEVICE9 pDevice;
	
public:
	Dx9DemoBase();
	virtual ~Dx9DemoBase();

	//��������
	bool Create(HINSTANCE hInstance, int width, int height, LPCSTR lpszWndName);

	//��ʼ��Dx9
	bool InitDevice(HINSTANCE hInstance, HWND hwnd);

	//����ѭ��
	WPARAM Run();

	//�ͷ���Դ
	void Shutdown();

	//��Ϣ������
	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual bool LoadContent();

	virtual void UnloadContent();

	virtual void Update(float dt) = 0;

	virtual void Render() = 0;

	inline IDirect3DDevice9* GetD3DDevice()const { return m_pDevice; };
};

#endif

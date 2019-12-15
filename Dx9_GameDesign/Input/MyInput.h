// MyInput.h: interface for the MyInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEINPUT_H__22EFB5FB_E9DA_436B_85B3_1AB46A961564__INCLUDED_)
#define AFX_GEINPUT_H__22EFB5FB_E9DA_436B_85B3_1AB46A961564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




//#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
//#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
//#define DIPROPMODE int
//#define Dword 0
//#define Range 1


#include "dinput.h"
#pragma comment( lib, "dinput8.lib")   
#pragma comment( lib, "dxguid.lib") 

/**
 * ���������豸��Ļ��� 
 */
class MyInput  
{
public:
	MyInput();
	virtual ~MyInput();
	// ���������̵������豸
	bool OnCreateDevice(HWND hwnd, GUID guid, struct _DIDATAFORMAT const* format, int size);
	// �ͷŲ�ɾ���豸
	void FreeDevice();
	// ȡ�������豸
	HRESULT AcquireDevice();
	// �ͷ������豸
	HRESULT UnacquireDevice();
	
protected:
	// �ж������豸��Ӧ���Ƿ񱻰���
	bool IsKeyDown(const BYTE storage[], const BYTE oldStorage[], int type, bool lastSensitive = false);
	// �ж������豸��Ӧ���Ƿ��ɿ�
	bool IsKeyUp  (const BYTE storage[], const BYTE oldStorage[], int type, bool lastSensitive = false);
	// ����ǰ�����豸״̬��Ϣ����
	bool _SaveState2Old(const BYTE storage[], BYTE oldStorage[], int size);
	//�����º�Ľ�����
	bool _CheckState(HRESULT hr);
	
protected:
	static LPDIRECTINPUT8	m_pDI;		///< ���� DirectInput ����ָ��
	LPDIRECTINPUTDEVICE8	m_pDevice;	///< ��������װ�ö���ָ��
};



#endif // !defined(AFX_GEINPUT_H__22EFB5FB_E9DA_436B_85B3_1AB46A961564__INCLUDED_)

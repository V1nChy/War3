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
 * 所有输入设备类的基类 
 */
class MyInput  
{
public:
	MyInput();
	virtual ~MyInput();
	// 创建鼠标键盘等输入设备
	bool OnCreateDevice(HWND hwnd, GUID guid, struct _DIDATAFORMAT const* format, int size);
	// 释放并删除设备
	void FreeDevice();
	// 取得输入设备
	HRESULT AcquireDevice();
	// 释放输入设备
	HRESULT UnacquireDevice();
	
protected:
	// 判断输入设备对应键是否被按下
	bool IsKeyDown(const BYTE storage[], const BYTE oldStorage[], int type, bool lastSensitive = false);
	// 判断输入设备对应键是否被松开
	bool IsKeyUp  (const BYTE storage[], const BYTE oldStorage[], int type, bool lastSensitive = false);
	// 将当前输入设备状态信息保存
	bool _SaveState2Old(const BYTE storage[], BYTE oldStorage[], int size);
	//检查更新后的结果句柄
	bool _CheckState(HRESULT hr);
	
protected:
	static LPDIRECTINPUT8	m_pDI;		///< 声明 DirectInput 对象指针
	LPDIRECTINPUTDEVICE8	m_pDevice;	///< 声明输入装置对象指针
};



#endif // !defined(AFX_GEINPUT_H__22EFB5FB_E9DA_436B_85B3_1AB46A961564__INCLUDED_)

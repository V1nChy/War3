// GEInput.cpp: implementation of the GEInput class.
//
//////////////////////////////////////////////////////////////////////

#include "MyInput.h"
#include "MyUTIL.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPDIRECTINPUT8 MyInput::m_pDI = NULL;

/**
 * GEInput的构造函数，将Direct Input的指针和Device的指针都清空，设Device未被创建
 */
MyInput::MyInput()
		: m_pDevice(NULL)
{
	 // 声明 DirectInput 对象指针
	 // 声明输入装置对象指针
}
/**
 * GEInput的析构函数，调用FreeDevice来清空输入设备
 */
MyInput::~MyInput()
{
	FreeDevice();
}
/**
 * 创建鼠标键盘等输入设备
 *
 * @param hwnd 受输入设备控制的窗口的句柄
 * @param guid 输入设备的标识符
 * @param format 输入设备的格式
 * @param size 输入设备的buffer大小
 *
 * @return 创建输入设备成功与否的bool值
 */
bool MyInput::OnCreateDevice(HWND hwnd, GUID guid, struct _DIDATAFORMAT const* format, int size){

	// 创建输入设备
	HINSTANCE hInst = (HINSTANCE) GetWindowLong( hwnd, GWL_HINSTANCE );

	if( m_pDevice )	
		FreeDevice();

	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *)&m_pDI, NULL);
	if( FAILED( hr ))	
		return FALSE;

	// Obtain an interface to the system device.
    if( FAILED( hr = m_pDI->CreateDevice( guid, &m_pDevice, NULL ) ) )
        return FALSE;

	if( FAILED( hr = m_pDevice->SetDataFormat( format ) ) )
		return FALSE;

	DWORD dwCoopFlags = DISCL_NONEXCLUSIVE|DISCL_FOREGROUND;//Device Model !!
	if( FAILED( m_pDevice->SetCooperativeLevel(hwnd, dwCoopFlags) ) )
		return FALSE;

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize			= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize		= sizeof(DIPROPHEADER);
	dipdw.diph.dwObj			= 0;
	dipdw.diph.dwHow			= DIPH_DEVICE;
	dipdw.dwData				= size; // Arbitary buffer size

	if( FAILED( hr = m_pDevice->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
		return FALSE;

	AcquireDevice();
    return true;
}


/**
 * 清空输入设备
 */
void MyInput::FreeDevice(){
	// Unacquire the device one last time just in case
	// the app tried to exit while the device is still acquired.
	if( m_pDevice ) 
		m_pDevice->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( m_pDevice );
    SAFE_RELEASE( m_pDI );
}

bool MyInput::IsKeyDown(const BYTE storage[], const BYTE oldStorage[], int type, bool lastSensitive){
	// 判断输入设备对应键是否被按下
	if(storage[type] & 0x80){
		if(lastSensitive && (oldStorage[type] & 0x80))
			return false;
		else 
			return true;
	}
	else
		return false;
}

bool MyInput::IsKeyUp(const BYTE storage[], const BYTE oldStorage[], int type, bool lastSensitive){
	// 判断输入设备对应键是否被松开
	if(storage[type] & 0x80)
		return false;
	else 
	{
		if (lastSensitive && !(oldStorage[type] & 0x80))
			return false;
		else
			return true;
	}
}

bool MyInput::_SaveState2Old(const BYTE storage[], BYTE oldStorage[], int size){
	// 将当前输入设备状态信息保存    
	if( NULL == m_pDevice ){
		return false;//write: No Device. STOP
	}
	// 把上次的按键情况记录到OldStorage里
	memcpy(oldStorage, storage, size);
	return true;
}

bool MyInput::_CheckState(HRESULT hr){
	//检查更新后的结果句柄
    if( hr != DI_OK ) 
    {
        hr = m_pDevice->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pDevice->Acquire();

        if( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ){ 
            return false;//write: Device Not Aquired. STOP
		}
    }	
	if( FAILED(hr) ){
       return false;//write: Device Not Aquired. STOP
	}
	return true;
}
/**
 * 取得输入设备
 *
 * @return 所得的结果句柄
 */
HRESULT MyInput::AcquireDevice(){
	return m_pDevice->Acquire();
}
/**
 * 释放输入设备
 *
 * @return 所得的结果句柄
 */
HRESULT MyInput::UnacquireDevice(){
	return m_pDevice->Unacquire();
}

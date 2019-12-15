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
 * GEInput�Ĺ��캯������Direct Input��ָ���Device��ָ�붼��գ���Deviceδ������
 */
MyInput::MyInput()
		: m_pDevice(NULL)
{
	 // ���� DirectInput ����ָ��
	 // ��������װ�ö���ָ��
}
/**
 * GEInput����������������FreeDevice����������豸
 */
MyInput::~MyInput()
{
	FreeDevice();
}
/**
 * ���������̵������豸
 *
 * @param hwnd �������豸���ƵĴ��ڵľ��
 * @param guid �����豸�ı�ʶ��
 * @param format �����豸�ĸ�ʽ
 * @param size �����豸��buffer��С
 *
 * @return ���������豸�ɹ�����boolֵ
 */
bool MyInput::OnCreateDevice(HWND hwnd, GUID guid, struct _DIDATAFORMAT const* format, int size){

	// ���������豸
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
 * ��������豸
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
	// �ж������豸��Ӧ���Ƿ񱻰���
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
	// �ж������豸��Ӧ���Ƿ��ɿ�
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
	// ����ǰ�����豸״̬��Ϣ����    
	if( NULL == m_pDevice ){
		return false;//write: No Device. STOP
	}
	// ���ϴεİ��������¼��OldStorage��
	memcpy(oldStorage, storage, size);
	return true;
}

bool MyInput::_CheckState(HRESULT hr){
	//�����º�Ľ�����
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
 * ȡ�������豸
 *
 * @return ���õĽ�����
 */
HRESULT MyInput::AcquireDevice(){
	return m_pDevice->Acquire();
}
/**
 * �ͷ������豸
 *
 * @return ���õĽ�����
 */
HRESULT MyInput::UnacquireDevice(){
	return m_pDevice->Unacquire();
}

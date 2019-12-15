// GEMouse.cpp: implementation of the GEMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "MyMouse.h"
#include <Windows.h>
#include "MyGUIImage.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyMouse MyMouse::c_Mouse =MyMouse();

MyMouse::MyMouse():
	m_bShowCursor(true),
	m_pCursorNormal(NULL),
	m_pCursorSelect(NULL),
	m_pCurrentCursor(NULL),
	m_offsetImgX(0),
	m_offsetImgY(0)
{
}

MyMouse::~MyMouse(void)
{
	delete m_pCursorNormal;
	delete m_pCursorSelect;
}

/**
* 创建鼠标输入设备
*
* @param hwnd 鼠标作用的窗口的句柄
*
* @return 创建鼠标成功与否的标识符
*/
bool MyMouse::OnCreateMouse(HWND hwnd, int left, int top, int right, int bottom, const string& src)
{
	m_pCursorNormal = new MyGUIImage(NULL, 0, "DEFAULT_CURSOR", src, true, MyColor::Black,
		0, 0, 30, 30, MyColor::White, true);
	m_pCurrentCursor = m_pCursorNormal;
	m_pCursorNormal->SetUVMap(0, 0, 1, 1);
	m_pCursorNormal->OneTimeInit();

	m_Pos.x = (left + right)/2;
	m_Pos.y = (top + bottom)/2;
	m_Scare.left = left;
	m_Scare.right = right;
	m_Scare.top = top;
	m_Scare.bottom = bottom;
	m_dOldClickedTime = -1;
	m_Sensitivity = 1;

	return OnCreateDevice( hwnd, GUID_SysMouse, &c_dfDIMouse2, 1 );
}
bool MyMouse::OnCreateMouse(HWND hwnd, RECT scare, const string& src)
{
	m_pCursorNormal = new MyGUIImage(NULL, 0, "DEFAULT_CURSOR", src, true, MyColor::Black,
		0, 0, 30, 30, MyColor::White, true);
	m_pCurrentCursor = m_pCursorNormal;
	m_pCursorNormal->SetUVMap(0, 0, 1, 1);
	m_pCursorNormal->OneTimeInit();

	m_Pos.x = (scare.left + scare.right) / 2;
	m_Pos.y = (scare.top + scare.bottom) / 2;
	m_Scare = scare;

	m_dOldClickedTime = -1;
	m_Sensitivity = 1;

	return OnCreateDevice(hwnd, GUID_SysMouse, &c_dfDIMouse2, 1);
}
void MyMouse::SetCursor(CursorType type, MyGUIImage* img)
{
	switch (type)
	{
	case Normal:
		if (m_pCursorNormal)
			delete m_pCursorNormal;
		m_pCursorNormal = img;
		break;
	case Select:
		if (m_pCursorSelect)
			delete m_pCursorSelect;
		m_pCursorSelect = img;
		break;
	default:
		break;
	}
	m_pCurrentCursor = m_pCursorNormal;
}
void MyMouse::ShowMouse()
{
	if (m_bShowCursor)
	{
		m_pCurrentCursor->SetPosition(m_ClinetPos.x + m_offsetImgX, m_ClinetPos.y + m_offsetImgY);
		m_pCurrentCursor->Render();
	}
}

/**
* 更新鼠标状态信息
*
* @param hwnd 鼠标作用的窗口的句柄
*
* @return 更新鼠标状态成功与否的标识符
*/
bool MyMouse::Update(HWND hwnd) {
	HRESULT	hr;

	if (_SaveState2Old(m_State.rgbButtons, m_OldState.rgbButtons, MouseBuffer))
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_State);
	else
		return false;

	if (_CheckState(hr)) {
		if (0 != m_State.lY) {
			m_Pos.y += m_State.lY*m_Sensitivity;
			if (m_Scare.bottom<m_Pos.y)
				m_Pos.y = m_Scare.bottom;
			if (m_Scare.top>m_Pos.y)
				m_Pos.y = m_Scare.top;
		}

		if (0 != m_State.lX) {
			m_Pos.x += m_State.lX*m_Sensitivity;
			if (m_Scare.left>m_Pos.x)
				m_Pos.x = m_Scare.left;
			if (m_Scare.right<m_Pos.x)
				m_Pos.x = m_Scare.right;
		}
	}
	else
		return false;

	::SetCursorPos(m_Pos.x, m_Pos.y);


	static POINT p;
	p.x = m_Pos.x;
	p.y = m_Pos.y;
	ScreenToClient(hwnd, &p);
	m_ClinetPos.x = p.x;
	m_ClinetPos.y = p.y;

	m_pCurrentCursor->Update(0);

	return true;
}

/**
* 判断鼠标对应键是否被按下
*
* @param type 鼠标按键标识符
* @param lastSensitive 对上次按键敏感与否的bool量
*
* @return 按下与否的bool值
*/
bool MyMouse::IsMouseKeyDown(int type, bool lastSensitive){
	return IsKeyDown(m_State.rgbButtons,m_OldState.rgbButtons,type,lastSensitive);
}

/**
* 判断鼠标对应键是否被松开
*
* @param type 鼠标按键标识符
* @param lastSensitive 对上次按键敏感与否的bool量
*
* @return 松开与否的bool值
*/
bool MyMouse::IsMouseKeyUp(int type, bool lastSensitive){
	return IsKeyUp(m_State.rgbButtons,m_OldState.rgbButtons,type,lastSensitive);
}

/** 判断鼠标左键是否双击 \n
* @param duringTime [float]: 满足双击时，两次单击鼠标的最大间隔时间
* @return [bool] 是否双击
*/
bool MyMouse::IsMouseDoubleClicked(float duringTime)
{

	DWORD currentTime = timeGetTime();
	if(IsMouseKeyDown(0,true) )
	{
		DWORD timePassed = currentTime - m_dOldClickedTime;
		if(timePassed <= (DWORD)(duringTime * 1000) && (m_dOldClickedTime != -1))
		{
			m_dOldClickedTime = -1;
			return true;
		}
		m_dOldClickedTime = currentTime;
	}

	return false;
}

void MyMouse::SetSensitivity(float n)
{
	m_Sensitivity = n;
}

/**
 * 设置鼠标位置
 *
 * @param x 鼠标横坐标值
 * @param y 鼠标纵坐标值
 */
void MyMouse::SetPos(int x, int y){
	m_Pos.x = x;
	m_Pos.y = y;
}

/**
* 设置鼠标位置
*
* @param newMouse 新的鼠标坐标值
*/
void MyMouse::SetPos(Point2D<int> newMouse){
	m_Pos.x = newMouse.x;
	m_Pos.y = newMouse.y;
}

/**
 * 设置鼠标移动范围
 *
 * @param left   鼠标移动范围的左边界
 * @param top    鼠标移动范围的上边界
 * @param right  鼠标移动范围的右边界
 * @param bottom 鼠标移动范围的下边界
 */
void MyMouse::SetScare(int left, int top, int right, int bottom){
	m_Scare.left = left;
	m_Scare.top = top;
	m_Scare.right = right;
	m_Scare.bottom = bottom;
}

/**
 * 设置鼠标移动范围
 *
 * @param LeftTop     鼠标移动范围的左上角
 * @param RightBottom 鼠标移动范围的右下角
 *
 * @return 
 */
void MyMouse::SetScare(Point2D<int> LeftTop, Point2D<int> RightBottom){
	m_Scare.left = LeftTop.x;
	m_Scare.top = LeftTop.y;
	m_Scare.right = RightBottom.x;
	m_Scare.bottom =RightBottom.y;
}

/**
* 获取鼠标移动范围信息
*
* @return 鼠标移动范围
*/
RECT MyMouse::GetScare() {
	return m_Scare;
}

/**
* 获得滚轮信息
*
* @return 返回滚轮信息，每一格用120表示，向上为正，向下为负
*/
int MyMouse::GetScroll() {
	return m_State.lZ;
}

/**
* 获得鼠标横向坐标
*
* @return 返回鼠标横向坐标
*/
int MyMouse::GetX() { return m_ClinetPos.x; }

/**
* 获得鼠标纵向坐标
*
* @return 返回鼠标纵向坐标
*/
int MyMouse::GetY() { return m_ClinetPos.y; }

/**
* 获得鼠标坐标
*
* @return 返回鼠标坐标
*/
Point2D<int> MyMouse::GetPos() {
	return m_ClinetPos;
}
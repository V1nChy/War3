// GEMouse.h: interface for the GEMouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEMOUSE_H__507C5D90_5B35_47F8_9A3D_C1539B95315C__INCLUDED_)
#define AFX_GEMOUSE_H__507C5D90_5B35_47F8_9A3D_C1539B95315C__INCLUDED_

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#define DIRECTINPUT_VERSION 0x0800

#define LEFT 0
#define TOP 0
#define RIGHT 800
#define BOTTOM 600

#define MouseBuffer 8

#include <vector>
using namespace std;

#include "Point2D.h"
#include "MyInput.h"

class MyGUIImage;

enum CursorType
{
	Normal,
	Select
};
/**
* 鼠标设备类 
*/
class MyMouse : public MyInput  
{
private:
	bool m_bShowCursor;

	MyGUIImage* m_pCursorNormal;
	MyGUIImage* m_pCursorSelect;
	MyGUIImage* m_pCurrentCursor;

	float m_offsetImgX;
	float m_offsetImgY;

	Point2D<int>		m_Pos;				/// 鼠标位置
	Point2D<int>		m_ClinetPos;		/// 鼠标在客户区的相对位置
	RECT				m_Scare;			/// 鼠标移动范围
	DIMOUSESTATE2		m_State;			/// 鼠标状态结构
	DIMOUSESTATE2		m_OldState;			/// 鼠标上次状态结构
	DWORD				m_dOldClickedTime;	/// 鼠标上次单击时间,用于双击判断

	float				m_Sensitivity;
	static MyMouse		c_Mouse;			/// 保存唯一的鼠标类指针
	MyMouse();
public:
	
	virtual ~MyMouse();

	/** 创建鼠标输入设备 \n
	 * @param [HWND]: 窗口句柄
	 * @return [int] left: 鼠标行动限制的最左边
	 * @return [int] top: 鼠标行动限制的最上边
	 * @return [int] right: 鼠标行动限制的最右边
	 * @return [int] bottom: 鼠标行动限制的最下边
	 * @note: 鼠标不会超过限定的范围
	 */
	bool OnCreateMouse(HWND hwnd, int left=LEFT, int top=TOP, int right=RIGHT, int bottom=BOTTOM, const string& src = "Texture/cursor.bmp");
	bool OnCreateMouse(HWND hwnd, RECT scare, const string& src="Texture/cursor.bmp");

	/** 判断鼠标对应键是否被按下 \n
	 * @param [int]: 哪个键（0-左键；1－右键；2－滚轮 ）
	 * @return [bool] lastSensitive: 本次按键返回和上次按键是否敏感，如果true，则表示如果上次按下左键，且到本次未松开，则本次返回false
	 *								 如果不敏感，则本次返回和上次的结果没有关联	
	 */
	bool IsMouseKeyDown(int type, bool lastSensitive = false);
	
	/** 判断鼠标对应键是否被松开 \n
	 * @param [int]: 哪个键（0-左键；1－右键；2－滚轮 ）
	 * @return [bool] lastSensitive: 本次按键返回和上次按键是否敏感
	 */
	bool IsMouseKeyUp  (int type, bool lastSensitive = false);

	/** 判断鼠标左键是否双击 \n
	* @param duringTime [float]: 满足双击时，两次单击鼠标的最大间隔时间
	* @return [bool] 是否双击
	*/
	bool IsMouseDoubleClicked(float duringTime);


	/** 返回上帧到此帧的滚轮改变量 */
	int GetScroll();
	/** 获取鼠标位置信息-x */
	int GetX();
	/** 获取鼠标位置信息-y */
	int GetY();
	/** 获取鼠标位置信息 */
	Point2D<int> GetPos();

	/** 设置鼠标位置 */
	void SetPos(int x, int y);
	/** 设置鼠标位置 */
	void SetPos(Point2D<int> newMouse);

	/** 获取鼠标移动范围信息 */
	RECT GetScare();


	/** 设置鼠标移动范围 */
	void SetScare(int left, int top, int right, int bottom);
	void SetScare(Point2D<int> LeftTop, Point2D<int> RightBottom);

	inline void SetShowMouse(bool b) { m_bShowCursor = b; }
	inline void SetOffsetCursor(float x, float y) { m_offsetImgX = x; m_offsetImgY = y; }

	void SetCursor(CursorType type, MyGUIImage* img);

	/** 设置鼠标敏感度，默认为1 */
	void SetSensitivity(float n);

	void ShowMouse();

	/** 更新鼠标状态信息,每帧都要调用 */
	bool Update(HWND hwnd);

	/** 得唯一鼠标类指针,Singleton模式 */
	static MyMouse* GetSingleton(){
		return &c_Mouse;
	}
};

#endif // !defined(AFX_GEMOUSE_H__507C5D90_5B35_47F8_9A3D_C1539B95315C__INCLUDED_)

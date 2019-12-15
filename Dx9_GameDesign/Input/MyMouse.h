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
* ����豸�� 
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

	Point2D<int>		m_Pos;				/// ���λ��
	Point2D<int>		m_ClinetPos;		/// ����ڿͻ��������λ��
	RECT				m_Scare;			/// ����ƶ���Χ
	DIMOUSESTATE2		m_State;			/// ���״̬�ṹ
	DIMOUSESTATE2		m_OldState;			/// ����ϴ�״̬�ṹ
	DWORD				m_dOldClickedTime;	/// ����ϴε���ʱ��,����˫���ж�

	float				m_Sensitivity;
	static MyMouse		c_Mouse;			/// ����Ψһ�������ָ��
	MyMouse();
public:
	
	virtual ~MyMouse();

	/** ������������豸 \n
	 * @param [HWND]: ���ھ��
	 * @return [int] left: ����ж����Ƶ������
	 * @return [int] top: ����ж����Ƶ����ϱ�
	 * @return [int] right: ����ж����Ƶ����ұ�
	 * @return [int] bottom: ����ж����Ƶ����±�
	 * @note: ��겻�ᳬ���޶��ķ�Χ
	 */
	bool OnCreateMouse(HWND hwnd, int left=LEFT, int top=TOP, int right=RIGHT, int bottom=BOTTOM, const string& src = "Texture/cursor.bmp");
	bool OnCreateMouse(HWND hwnd, RECT scare, const string& src="Texture/cursor.bmp");

	/** �ж�����Ӧ���Ƿ񱻰��� \n
	 * @param [int]: �ĸ�����0-�����1���Ҽ���2������ ��
	 * @return [bool] lastSensitive: ���ΰ������غ��ϴΰ����Ƿ����У����true�����ʾ����ϴΰ���������ҵ�����δ�ɿ����򱾴η���false
	 *								 ��������У��򱾴η��غ��ϴεĽ��û�й���	
	 */
	bool IsMouseKeyDown(int type, bool lastSensitive = false);
	
	/** �ж�����Ӧ���Ƿ��ɿ� \n
	 * @param [int]: �ĸ�����0-�����1���Ҽ���2������ ��
	 * @return [bool] lastSensitive: ���ΰ������غ��ϴΰ����Ƿ�����
	 */
	bool IsMouseKeyUp  (int type, bool lastSensitive = false);

	/** �ж��������Ƿ�˫�� \n
	* @param duringTime [float]: ����˫��ʱ�����ε������������ʱ��
	* @return [bool] �Ƿ�˫��
	*/
	bool IsMouseDoubleClicked(float duringTime);


	/** ������֡����֡�Ĺ��ָı��� */
	int GetScroll();
	/** ��ȡ���λ����Ϣ-x */
	int GetX();
	/** ��ȡ���λ����Ϣ-y */
	int GetY();
	/** ��ȡ���λ����Ϣ */
	Point2D<int> GetPos();

	/** �������λ�� */
	void SetPos(int x, int y);
	/** �������λ�� */
	void SetPos(Point2D<int> newMouse);

	/** ��ȡ����ƶ���Χ��Ϣ */
	RECT GetScare();


	/** ��������ƶ���Χ */
	void SetScare(int left, int top, int right, int bottom);
	void SetScare(Point2D<int> LeftTop, Point2D<int> RightBottom);

	inline void SetShowMouse(bool b) { m_bShowCursor = b; }
	inline void SetOffsetCursor(float x, float y) { m_offsetImgX = x; m_offsetImgY = y; }

	void SetCursor(CursorType type, MyGUIImage* img);

	/** ����������жȣ�Ĭ��Ϊ1 */
	void SetSensitivity(float n);

	void ShowMouse();

	/** �������״̬��Ϣ,ÿ֡��Ҫ���� */
	bool Update(HWND hwnd);

	/** ��Ψһ�����ָ��,Singletonģʽ */
	static MyMouse* GetSingleton(){
		return &c_Mouse;
	}
};

#endif // !defined(AFX_GEMOUSE_H__507C5D90_5B35_47F8_9A3D_C1539B95315C__INCLUDED_)

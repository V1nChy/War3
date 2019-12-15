//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------

#if !defined(AFX_FRUSTUM_H__26C156AF_A4C2_4E57_9389_6190CE044F69__INCLUDED_)
#define AFX_FRUSTUM_H__26C156AF_A4C2_4E57_9389_6190CE044F69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Plane.h"


/** ƽ����������� */
enum FRUSTUM_PLANE
{
	PLANE_LEFT,
	PLANE_RIGHT,
	PLANE_TOP,
	PLANE_BOTTOM,
	PLANE_FRONT,
	PLANE_BACK
};



/** ƽ��׶�壬һ�������� */
class Frustum  
{
public:
	/** �õ�ƽ��const���� \n
	 * @param [FRUSTUM_PLANE]: PLANE_LEFT��PLANE_RIGHT��PLANE_TOP��PLANE_BOTTOM��PLANE_FRONT��PLANE_BACK
	 * @return [const Plane *]: ��Ӧ��ƽ��ָ��
	 */
	const Plane* GetPlane(FRUSTUM_PLANE index) const;

	/** һ��ƽ��׶�������������\n
	 * ˳�����ң��ϣ��£�����Զ
	 */
	Plane m_Plane[6];

};

#endif // !defined(AFX_FRUSTUM_H__26C156AF_A4C2_4E57_9389_6190CE044F69__INCLUDED_)

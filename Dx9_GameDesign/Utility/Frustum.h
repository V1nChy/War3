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


/** 平面的六个方向 */
enum FRUSTUM_PLANE
{
	PLANE_LEFT,
	PLANE_RIGHT,
	PLANE_TOP,
	PLANE_BOTTOM,
	PLANE_FRONT,
	PLANE_BACK
};



/** 平截锥体，一个六面体 */
class Frustum  
{
public:
	/** 得到平面const引用 \n
	 * @param [FRUSTUM_PLANE]: PLANE_LEFT，PLANE_RIGHT，PLANE_TOP，PLANE_BOTTOM，PLANE_FRONT，PLANE_BACK
	 * @return [const Plane *]: 对应的平面指针
	 */
	const Plane* GetPlane(FRUSTUM_PLANE index) const;

	/** 一个平截锥体由六个面组成\n
	 * 顺序：左，右，上，下，近，远
	 */
	Plane m_Plane[6];

};

#endif // !defined(AFX_FRUSTUM_H__26C156AF_A4C2_4E57_9389_6190CE044F69__INCLUDED_)

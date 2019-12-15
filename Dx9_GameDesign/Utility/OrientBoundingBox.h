// OrientBoundingBox.h: interface for the OrientBoundingBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORIENTBOUNDINGBOX_H__88F776F1_FFF3_4A52_8C83_997671BB04AB__INCLUDED_)
#define AFX_ORIENTBOUNDINGBOX_H__88F776F1_FFF3_4A52_8C83_997671BB04AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)

#include "Vector.h"
#include "Frustum.h"

class GEObject;


/** 有向包围盒 \n
 * 最开始初始化的时候, 就是一个AABB (axis aligned bounding box)
 * 旋转, 平移, 缩放 都是通过它包围的OBJECT实现的
 * 实现最小计算，很多优化是针对GEObject的特殊性做的
 */
class OrientBoundingBox
{
public :
	OrientBoundingBox();
	~OrientBoundingBox();

	void Init(GEObject * object, float fRight, float fLeft, float fUp, float fDown, float fFront, float fBack);

	const Frustum& GetOBB() const { return m_OBB; }

	/** 得到包围球的半径（经过缩放） */
	float GetRadius() const{ return m_fRadius; }
	/** 得到包围盒的中心点（经过变换） */
	const Vector3& GetCenter() const { return m_vCenter; }
	
	/** 得到长、宽、高（经过变换） */
	float GetWidth () const;
	float GetHeight() const;
	float GetDepth () const;

	void UpdateOBB();

	void Render();

protected:						    
	GEObject * m_Object;	///< 有向包围盒的主体	

	Vector3 m_vCenter;		///< 包围盒的中心点
	float m_fRight;
	float m_fLeft;
	float m_fFront;
	float m_fBack;
	float m_fUp;
	float m_fDown;

	float m_fRadiusOriginal;///< 原始包围盒对角线的一半（未缩放）
	float m_fRadius;		///< 包围盒对角线的一半（经过缩放）

	Frustum m_OBB;			///< 六面体
};

#endif // !defined(AFX_ORIENTBOUNDINGBOX_H__88F776F1_FFF3_4A52_8C83_997671BB04AB__INCLUDED_)

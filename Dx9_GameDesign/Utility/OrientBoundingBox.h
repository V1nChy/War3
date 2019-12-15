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


/** �����Χ�� \n
 * �ʼ��ʼ����ʱ��, ����һ��AABB (axis aligned bounding box)
 * ��ת, ƽ��, ���� ����ͨ������Χ��OBJECTʵ�ֵ�
 * ʵ����С���㣬�ܶ��Ż������GEObject������������
 */
class OrientBoundingBox
{
public :
	OrientBoundingBox();
	~OrientBoundingBox();

	void Init(GEObject * object, float fRight, float fLeft, float fUp, float fDown, float fFront, float fBack);

	const Frustum& GetOBB() const { return m_OBB; }

	/** �õ���Χ��İ뾶���������ţ� */
	float GetRadius() const{ return m_fRadius; }
	/** �õ���Χ�е����ĵ㣨�����任�� */
	const Vector3& GetCenter() const { return m_vCenter; }
	
	/** �õ��������ߣ������任�� */
	float GetWidth () const;
	float GetHeight() const;
	float GetDepth () const;

	void UpdateOBB();

	void Render();

protected:						    
	GEObject * m_Object;	///< �����Χ�е�����	

	Vector3 m_vCenter;		///< ��Χ�е����ĵ�
	float m_fRight;
	float m_fLeft;
	float m_fFront;
	float m_fBack;
	float m_fUp;
	float m_fDown;

	float m_fRadiusOriginal;///< ԭʼ��Χ�жԽ��ߵ�һ�루δ���ţ�
	float m_fRadius;		///< ��Χ�жԽ��ߵ�һ�루�������ţ�

	Frustum m_OBB;			///< ������
};

#endif // !defined(AFX_ORIENTBOUNDINGBOX_H__88F776F1_FFF3_4A52_8C83_997671BB04AB__INCLUDED_)

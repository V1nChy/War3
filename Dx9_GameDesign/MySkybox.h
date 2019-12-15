// MySkybox.h: interface for the MySkybox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_SKYBOX_H_
#define _MY_SKYBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"
#include "MyVertexFormat.h"

class MyCamera;


/** 这个天空盒类并没有用到directx的硬件加速, 所以不能用DX的方法进行环境映射.
 *	PETER: 需要改进
 */
class MySkybox : public MyObject  
{
protected:
	My_NLVERTEX m_Vertex[36];

	int m_uSize;
	MyCamera* m_pCamera;
	int m_uOffsetFromGround;
public:
	MySkybox(const string& name, const string& cubeTexFilePath, size_t size, bool visible = true);

	/** 对应的照相机 */
	bool	SetCamera(MyCamera* camera, int height);
	/** 设置边长 */
	void	SetSize(int size);

	HRESULT Render(const MyCamera& camera);
	HRESULT Render();

	virtual ~MySkybox();
};

#endif // !defined(AFX_GESKYBOX_H__2D969A0D_C25C_4DB2_84CA_1D0695D1C106__INCLUDED_)

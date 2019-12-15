// GEFile.h: interface for the GEFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEFILE_H__3E91B423_55F2_454A_8ECE_03D258C1B933__INCLUDED_)
#define AFX_GEFILE_H__3E91B423_55F2_454A_8ECE_03D258C1B933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyUtil.h"


/** 文件类 */
class GEFile  
{
public:
	GEFile();
	virtual ~GEFile();
	
	/** 判断此文件是否存在 */
	static bool IsFileExist(string strPath);

};

#endif // !defined(AFX_GEFILE_H__3E91B423_55F2_454A_8ECE_03D258C1B933__INCLUDED_)

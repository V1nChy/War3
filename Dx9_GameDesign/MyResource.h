// MyResource.h: interface for the MyResource class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_RESOURCE_H_
#define _MY_RESOURCE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <cstddef>
using namespace std;

typedef unsigned int ResHandle;

class MyResourceManager;
class MyResourceLoader;


/** 资源类,加载的资源基类 */
class MyResource  
{
protected:
	bool m_bLocked;							//< 是否被锁定
	bool m_bIsLoaded;						//< 是否被加载
	size_t m_iSize;							//< 资源的大小
	string m_strName;						//< 资源的名称 (一定要唯一)
	string m_strOrigin;						//< 资源的路径
	MyResourceManager * m_pResManager;		//< 拥有资源的资源管理对象
	ResHandle m_hHandle;					//< 句柄,自动生成,一定唯一
	MyResourceLoader * m_pResLoader;		//< 该资源的加载器
	unsigned int m_iTimesRef;				//< 该资源被几个对象映射,即引用计数

public:
	MyResource();
	virtual ~MyResource();

	/** 初始化必要的数据 , 加载资源的第一步 \n
	 * strName : 资源的名字,必须唯一
	 * strOrigin: 资源源文件的位置
	 * pResManager: 资源管理器的指针
	 *
	 * return: 是否初始化成功,因为此函数只能在load之前调用	
	 */
	bool InitParms( ResHandle hHandle, string strName, string strOrigin, 
		MyResourceManager * pResManager, MyResourceLoader * pResLoader = 0 );

	/** 得到资源的大小 */
	size_t GetSize(void) const { return m_iSize; }

	/** 得到资源的名称(唯一的代号) */
	const string& GetName(void) const { return m_strName; }

	/** 得到资源的源文件的路径,名称 */
	const string& GetOrigin(void) const { return m_strOrigin; }

	/** 返回是否已经加载资源 */
	bool IsLoaded(void) const { return m_bIsLoaded; }

	/** 得到句柄 */
	ResHandle GetHandle(void) const {return m_hHandle;}

	/** 得到其对应的资源管理器 */
	MyResourceManager* GetResManager(void) const { return m_pResManager; }

	unsigned int AddTimesRef(){ return ++m_iTimesRef; }

	unsigned int MinusTimesRef(){ return --m_iTimesRef; }

	unsigned int GetTimesRef() const { return m_iTimesRef; }

	/** 卸载资源 */
	virtual bool Unload(void);
	/** 将已经加载的资源再重新加载一次 */
	virtual bool Reload(void);
	/** 加载资源 */
	virtual bool Load(void);
	/** 删除资源对象 */
	virtual bool Delete();


protected:
	/** 如何加载资源,必须重载 */
	virtual bool LoadImpl(void) = 0;
	/** 如何卸载资源,必须重载 */
	virtual bool UnloadImpl(void) = 0;
	/** 如何得到资源文件的大小,必须重载 */
	virtual size_t CalculateSize(void) const = 0;

};


/** 资源加载器,接口类 */
class MyResourceLoader
{
public:
	MyResourceLoader() {}
	virtual ~MyResourceLoader() {}

	virtual bool LoadResouce(MyResource * pRes) = 0;
};





#endif // !defined(AFX_GERESOURCE_H__3142C29E_CDFB_468C_BF6D_C128BA988121__INCLUDED_)

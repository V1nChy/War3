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


/** ��Դ��,���ص���Դ���� */
class MyResource  
{
protected:
	bool m_bLocked;							//< �Ƿ�����
	bool m_bIsLoaded;						//< �Ƿ񱻼���
	size_t m_iSize;							//< ��Դ�Ĵ�С
	string m_strName;						//< ��Դ������ (һ��ҪΨһ)
	string m_strOrigin;						//< ��Դ��·��
	MyResourceManager * m_pResManager;		//< ӵ����Դ����Դ�������
	ResHandle m_hHandle;					//< ���,�Զ�����,һ��Ψһ
	MyResourceLoader * m_pResLoader;		//< ����Դ�ļ�����
	unsigned int m_iTimesRef;				//< ����Դ����������ӳ��,�����ü���

public:
	MyResource();
	virtual ~MyResource();

	/** ��ʼ����Ҫ������ , ������Դ�ĵ�һ�� \n
	 * strName : ��Դ������,����Ψһ
	 * strOrigin: ��ԴԴ�ļ���λ��
	 * pResManager: ��Դ��������ָ��
	 *
	 * return: �Ƿ��ʼ���ɹ�,��Ϊ�˺���ֻ����load֮ǰ����	
	 */
	bool InitParms( ResHandle hHandle, string strName, string strOrigin, 
		MyResourceManager * pResManager, MyResourceLoader * pResLoader = 0 );

	/** �õ���Դ�Ĵ�С */
	size_t GetSize(void) const { return m_iSize; }

	/** �õ���Դ������(Ψһ�Ĵ���) */
	const string& GetName(void) const { return m_strName; }

	/** �õ���Դ��Դ�ļ���·��,���� */
	const string& GetOrigin(void) const { return m_strOrigin; }

	/** �����Ƿ��Ѿ�������Դ */
	bool IsLoaded(void) const { return m_bIsLoaded; }

	/** �õ���� */
	ResHandle GetHandle(void) const {return m_hHandle;}

	/** �õ����Ӧ����Դ������ */
	MyResourceManager* GetResManager(void) const { return m_pResManager; }

	unsigned int AddTimesRef(){ return ++m_iTimesRef; }

	unsigned int MinusTimesRef(){ return --m_iTimesRef; }

	unsigned int GetTimesRef() const { return m_iTimesRef; }

	/** ж����Դ */
	virtual bool Unload(void);
	/** ���Ѿ����ص���Դ�����¼���һ�� */
	virtual bool Reload(void);
	/** ������Դ */
	virtual bool Load(void);
	/** ɾ����Դ���� */
	virtual bool Delete();


protected:
	/** ��μ�����Դ,�������� */
	virtual bool LoadImpl(void) = 0;
	/** ���ж����Դ,�������� */
	virtual bool UnloadImpl(void) = 0;
	/** ��εõ���Դ�ļ��Ĵ�С,�������� */
	virtual size_t CalculateSize(void) const = 0;

};


/** ��Դ������,�ӿ��� */
class MyResourceLoader
{
public:
	MyResourceLoader() {}
	virtual ~MyResourceLoader() {}

	virtual bool LoadResouce(MyResource * pRes) = 0;
};





#endif // !defined(AFX_GERESOURCE_H__3142C29E_CDFB_468C_BF6D_C128BA988121__INCLUDED_)

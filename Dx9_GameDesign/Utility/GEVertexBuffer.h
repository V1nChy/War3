#ifndef _GE_VERTEX_BUFFER_H_
#define _GE_VERTEX_BUFFER_H_

#include "GEPrimitiveType.h"

class GEMesh;
class GESkeleton;
class GEMorphAnimationKey;
class GEBoneAssignment;

/**	 	 
* ���㻺��������װ�ײ㶥�㻺�����Ĵ�����������ݡ��豸��ʧ�Ĵ���ȡ�.
* ���㻺�����ṩ�˼��ִ�����ʽ��
* 1����managed��ʽ����
*   ������õķ�ʽ�����ݴ�����Դ��У��ײ�ͼ�ο��Զ���ϵͳ�ڴ��б��ݡ������豸��ʧ��
* 2����default��ʽ����
*   ���ַ�ʽ�������ڴ��б������ݱ��ݣ���Ҫ���豸��ʧʱ���´�Ӳ����װ�����ݡ���mesh�ܴ�ʱ����ϵͳ�ڴ��б������ݽ�ռ�ô����ռ䣬���豸��ʧ��������������������������¿�����default��ʽ��������������ʡ�ڴ�ռ䡣����������ȱ�����豸��ʧʱ���¶�ȡӲ�̡�����mesh�ļ����û�ͣ�ٸбȽ����ԣ����豸��ʧ����Ϸ���й����в���Ƶ����
* 3������dynamic��ʽ����
*   ����������ҪƵ������ʱ�������ڹ�������������dynamic��ʽ�������������߳�������Ч�ʡ������������dynamic��ʽ����������Ҫ��
*   a�� ������仺�����е����ݣ�
*     ��discard��ʽ��������������ʱ������Կ����ڶ�ȡ�û��棬DirectX�����̴������������أ���ָ��һ���µĻ�������ԭ���Ļ����������Կ���ȡ��Ϻ��ͷŵ�������������Ͳ���������ʱ����ֱ���Կ���ȡ�����ݡ�
*   b��ֻ����µ����ݣ�
*     ��nooverwrite��ʽ��������������ʱ������Կ�֡�ڶ�ȡ�û��棬DirectXͬ�������̴������������أ���ָ��û�������β���������������µ������ڻ�����ĩβ������Ӱ���Կ���ȡ������ǰ������ݡ�
*  \n\n
* �󲿷�����£��ͻ�����ԱӦ����GEVertexBuffer��managed��ʽ�����������������
* �ͻ�����Ա���ܲ�ϣ����managed��ʽ������
* 1���ö��㻺�������ڹ���������ҪƵ�����»��������ݣ���������·�managedЧ��
*    �ϸߣ��ڲ�ʹ��D3DPOOL_DEFAULT����
* 2��ģ�ͺܴ󣬲�ϣ�����ڴ��б��ݶ�ռ�ù����ڴ棨������RestoreDeviceObjects()ʱҪ��Ӳ��
*    �������ݣ�
*   ������������£����ڹ��캯����ָ��managedΪfalse����ʱ���Է�managed��ʽ������������
* ��������ͻ�����Աѡ����GEVertexBuffer��ϵͳ�ڴ��б��ݶ������ݡ��Զ�����restore����ô
* ��new��һ��GEVertexBuffer����֮��������PrepareSysBuffer()����֮������ͻ�����Ա����
* ĳЩԭ��ѡ����GEVertexBuffer��Ҫ��ϵͳ�ڴ��б������ݣ��磺��Ϊ�ö��㻺�����ܴ���
* ϵͳ�ڴ��б��ݽ��˷Ѽ���ռ䣩����ô���������PrepareSysBuffer()������Restoreʱ��Ҫ
* �ṩ����������Դ��������RestoreDeviceObjects(void* backSource, UINT sizeToFeed).
*   ������㻺������Ҫ�������²��Ҹ��·�ʽ����ȫ����ԭ���ݻ�ֻ��������ݣ����ڹ��캯��
* ָ��dynamicΪtrue��
* ע�⣺����������ҪƵ�����µĶ��㻺������������dynamic��ʽ������
*/	
class GEVertexBuffer
{
protected:		
	UINT m_vertexCount;	///< �������еĶ�������
	UINT m_stride;		///< ����Ĳ���
	UINT m_size;		///< ��������������
	UINT m_dataSize;	///< ��������ʵ�ʱ�������ݵ��ֽ���
	DWORD m_FVF;		///< D3D�������ʽ
	GEPrimitiveType m_primitiveType;	///< ͼԪ��ʽ	
	IDirect3DVertexBuffer9* m_DX9VB;	///< �ײ�D3D�Ķ��㻺����
	IDirect3DVertexBuffer9* m_ShadowVB; ///< ���ڹ���������Ӱ�ӻ��档���ʹ�øö��㻺������mesh�޹���������������
	BYTE* m_sysBuffer;	///< ��ϵͳ�ڴ��еı��ݣ��������豸�豸��ʧ��restoreʱ�ָ��Դ��е�IDirect3DIndexBuffer�����ݡ��������ã���������ʹ�ô˳�Ա��
	bool m_managed;		///< �Ƿ�����D3DPOOL_MANAGED������
	bool m_dynamic;		///< �Ƿ���ҪƵ�����µ�
	bool m_autoBackupForRestore; ///< �Ƿ��Զ���ϵͳ�ڴ汸�����ݵı�־
	
public:
	/**
	* ���캯��.
	* @param size Ҫ�����Ļ����С
	* @param FVF �������Ķ����ʽ
	* @param stride �����ڶ���Ĳ���
	* @param primitiveType ͼԪ���ͣ��������������߶��б�ȣ�	
	* @param managed ��managed��ʽ��������DirectX�ײ��Զ������豸��ʧ������������ϵͳ�ڴ���
	* �������ݡ�
	* @param dynamic ���������Ҫ��Ƶ�����£����Ҹ���ʱԭ��������ȫ��������ֻ��ԭ���ݺ���
	* ��ӣ���ʹ�ô˱�־���ǲ���Ч�ʸ��ߡ��������˵����
	* @param autoBackupForRestore ����Է�managed��ʽ������������ѡ��˱�־����ʹ������ϵͳ
	* �ڴ��Զ��������������豸��ʧ�Ĵ������ʹ��managed��ʽ����δѡ��˱�־������
	* Ҫ�ڴ����豸��ʧʱ�ṩ����Դ��
	*/
	GEVertexBuffer(UINT size, DWORD FVF, UINT stride, GEPrimitiveType primitiveType, 
		bool managed=true, bool dynamic=false, bool autoBackupForRestore=true);
	virtual ~GEVertexBuffer();

	/**
	* ׼��ϵͳ�ڴ��е����ݱ��ݣ���Ҫ�ṩ����Դ��
	* ���Է�managed��ʽ��������Ҫ��������ϵͳ�ڴ汸������ʱ��Ҫ�ڴ�������ô˺����� 
	*/
	void PrepareSysBuffer(void* sourceData);
	void PrepareSysBuffer(void* sourceData, UINT sizeToFeed);

	//< ��managed��vertex buffer���ô˰汾�������ڴ��ṩ��������
	void InitDeviceObjects();
	///< managed��vertex buffer���ô˰汾��������GEMesh��VB���������ṩ��������
	void InitDeviceObjects(void* pSource, UINT sizeToFeed);
	///< GEMesh��VB���ô˰汾�������lock���������������
	void InitDeviceObjectsFromMesh();
	///< managed��vertex buffer�Լ���m_autoBackupForRestoreΪ��ķ�managed��vertex buffer���ô˰汾
	void RestoreDeviceObjects();
	///< ��managed����δָ���Զ���������(m_autoBakcupForRestoreΪfalse)��vertex buffer���ô˺���
	void RestoreDeviceObjects(void* backSource, UINT sizeToFeed);		
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();

	///< ���D3D�����ʽ
	inline DWORD GetFVF() const	  { return m_FVF;	 }
	///< ��ö��㲽��
	inline UINT GetStride()	const { return m_stride; }		// get size of each vertex
	///< ��û�������С
	inline UINT GetSize() const	  { return m_size;	 }		// get total size of the buffer
	///< ��ö�������
	inline UINT GetVertexNum() const { return m_vertexCount; }
	///< ��û���ͼԪ������
	UINT   GetPrimitiveCount() const ;

	///< ����Ƿ񴴽���Ӱ�ӻ���
	inline bool hasShadowBuffer() { return m_ShadowVB?true:false; }
	///< ���ͼԪ����
	inline GEPrimitiveType GetPrimitiveType() { return m_primitiveType; }
	///< ���D3D�ײ㶥�㻺��
	inline IDirect3DVertexBuffer9* GetSource() 
	{  
		if(hasShadowBuffer())
		{
			return m_ShadowVB;			
		}
		else
		{
			return m_DX9VB;			
		}
		
	}

	///< �������
	void feed(void* pSource, UINT size);
	///< ����Ӱ�ӻ���
	bool createShadowBuffer();
	///< ���ݹǼܸ���Ӱ�ӻ�����
	bool updateFromSkeleton(GESkeleton* skeleton, GEBoneAssignment* assignment);

	bool updateFromMorphAnimation(GEMorphAnimationKey* currentPose);

	inline float GetXMin() const	{ return m_fXMin; }
	inline float GetXMax() const	{ return m_fXMax; }
	inline float GetYMin() const	{ return m_fYMin; }
	inline float GetYMax() const	{ return m_fYMax; }
	inline float GetZMin() const	{ return m_fZMin; }
	inline float GetZMax() const	{ return m_fZMax; }

protected:
	void _CreateDXVB();				///< ����m_DX9VB
	void _CopySysBufferToDXVB();	///< ��ϵͳ�ڴ��е����ݱ��ݿ���m_DX9VB
	void _FeedDXVB(void* sourceData);	///< ���D3D�ײ㻺����

	friend GEMesh;

	float m_fXMin;
	float m_fXMax;
	float m_fYMin;
	float m_fYMax;
	float m_fZMin;
	float m_fZMax;
};


#endif
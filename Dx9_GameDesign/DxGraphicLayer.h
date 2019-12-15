#ifndef _GRAPHIC_LAYER_H
#define _GRAPHIC_LAYER_H

#pragma once

#include <windows.h>
#include <D3D9.h>
#include <D3DX9.h>
#include <d3d9types.h>

#include "MyLight.h"
#include "MyCamera.h"
#include "MyTextureManager.h"
class DxGraphicLayer
{
private:
	static DxGraphicLayer* m_pGraphicLayer;
	IDirect3D9*       m_pD3D;              // D3D����
	IDirect3DDevice9* m_pd3dDevice;        // ͼ���豸�ӿ�
	D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer

	HWND              m_hWnd;              // ���ھ��
	int				  m_iWindowRCWidth;
	int				  m_iWindowRCHeight;
	int				  m_iFullScreenWidth;
	int				  m_iFullScreenHeight;

	float	m_fov;
	float	m_near;
	float	m_far;

	DxGraphicLayer();
protected:
	void _StoreBackSurfDesc();
public:
	~DxGraphicLayer();

	/** ����D3D */
	HRESULT CreateD3D9();
	void ReleaseD3D9();

	/** ����Setting����D3D�豸 */
	HRESULT CreateD3DDevice();
	void ReleaseD3D9Device();

	HRESULT Initialize3DEnvironment(HWND hwnd);

	void FinalCleanup();

	static inline DxGraphicLayer* GetSingleton() { return m_pGraphicLayer; }
	/** ��ʼ��������, �� EndScene() ���ʹ�� */
	inline HRESULT BeginScene() { return m_pd3dDevice->BeginScene(); }
	/** ������������ */
	inline HRESULT EndScene() { return m_pd3dDevice->EndScene(); }
	/** ��ҳ, ���󱸻������е�������ʾ����ҳ������ */
	inline HRESULT Present() { return m_pd3dDevice->Present(NULL, NULL, NULL, NULL); }

	/** ��ջ�����
	* @param[bool] bClearFrame: �Ƿ����ҳ������
	* @param[bool] bClearZ: �Ƿ���� Z ����
	* @param[DWORD] frameColor: �����ɫ
	* @param[float] zValue: z��������ֵ,Ĭ��Ϊ1
	*/
	void Clear(bool bClearFrame, bool bClearZ, const MyColor& frameColor, float zValue = 1.f);

	void ClearZBuffer(float zValue = 1.f);

	IDirect3DDevice9* GetD3DDevice() { return m_pd3dDevice; }


	/** ��Ļ����ʾ����  �� */
	inline int GetScreenWidth() const { return m_d3dsdBackBuffer.Width; }
	/** ��Ļ����ʾ����  �� */
	inline int GetScreenHeight() const { return m_d3dsdBackBuffer.Height; }

	/*------------------------------------------------------------------------------------------------*/
	//�������������
	/** ����ͶӰ��������	 */
	Matrix4 SetProjectionData(
		float inFov,
		float inNear,
		float inFar);

	/** �õ�Ŀǰ��ͶӰ�������� */
	void GetProjectionData(
		float* inFov,
		float* inNear,
		float* inFar);

	/** Ĭ������任 */
	void MakeViewport();

	/** �õ�����任���� */
	void GetViewMatrix(Matrix4* pMat);
	/** ��������任���� */
	void SetViewMatrix(const Matrix4& mat);

	/** �õ�ͶӰ�任���� */
	void GetProjectionMatrix(Matrix4* pMat);
	/** ����ͶӰ�任���� */
	void SetProjectionMatrix(const Matrix4& mat);

	/** �õ�����任���� */
	void GetWorldMatrix(Matrix4* pMat);
	/** ��������任���� */
	void SetWorldMatrix(const Matrix4& mat);
	/*------------------------------------------------------------------------------------------------*/

	/*------------------------------------------------------------------------------------------------*/
	//���ù�Դ
	HRESULT SetLight(DWORD LightIndex, MyLight * light);
	HRESULT GetLight(DWORD LightIndex, MyLight * light);
	HRESULT LightEnable(DWORD LightIndex, bool bEnable);

	/*------------------------------------------------------------------------------------------------*/
	//������Ⱦ״̬
	HRESULT SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	HRESULT SetFVF(DWORD FVF);
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

	/*------------------------------------------------------------------------------------------------*/
	//������ͼ
	bool SetTexture(int stage, const string& strName);
	bool SetTexture(int stage, ResHandle resHandle);
	bool SetTexture(int stage, My2DTexture * texture);

	/** ���ƻ���ͼԪ, ������ vertex buffer �����
	* @param[D3DPRIMITIVETYPE] PrimitiveType: ���Ʒ���--D3DPT_POINTLIST.	�����
	*													D3DPT_LINELIST.		�����б�
	*													D3DPT_LINESTRIP.	��������
	*													D3DPT_TRIANGLELIST.	�����б�
	*													D3DPT_TRIANGLESTRIP.��������
	*													D3DPT_TRIANGLEFAN.	��������
	*													D3DPT_FORCE_DWORD.	��û�õ�
	* @param[UINT] PrimitiveCount: ͼԪ����
	* @param[const void *] pVertexStreamZeroData: �ڴ���ʼλ��
	* @param[UINT] VertexStreamZeroStride: ÿ��������ڴ��С
	*/
	HRESULT DrawPrimitiveUp(D3DPRIMITIVETYPE PrimitiveType,
		UINT PrimitiveCount,
		const void *pVertexStreamZeroData,
		UINT VertexStreamZeroStride);
};


#endif
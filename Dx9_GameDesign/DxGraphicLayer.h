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
	IDirect3D9*       m_pD3D;              // D3D对象
	IDirect3DDevice9* m_pd3dDevice;        // 图形设备接口
	D3DSURFACE_DESC   m_d3dsdBackBuffer;   // Surface desc of the backbuffer

	HWND              m_hWnd;              // 窗口句柄
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

	/** 创建D3D */
	HRESULT CreateD3D9();
	void ReleaseD3D9();

	/** 根据Setting创建D3D设备 */
	HRESULT CreateD3DDevice();
	void ReleaseD3D9Device();

	HRESULT Initialize3DEnvironment(HWND hwnd);

	void FinalCleanup();

	static inline DxGraphicLayer* GetSingleton() { return m_pGraphicLayer; }
	/** 开始场景绘制, 和 EndScene() 配对使用 */
	inline HRESULT BeginScene() { return m_pd3dDevice->BeginScene(); }
	/** 结束场景绘制 */
	inline HRESULT EndScene() { return m_pd3dDevice->EndScene(); }
	/** 翻页, 将后备缓冲区中的数据显示到主页面上来 */
	inline HRESULT Present() { return m_pd3dDevice->Present(NULL, NULL, NULL, NULL); }

	/** 清空缓冲区
	* @param[bool] bClearFrame: 是否清空页面数据
	* @param[bool] bClearZ: 是否清空 Z 缓冲
	* @param[DWORD] frameColor: 填充颜色
	* @param[float] zValue: z缓冲的填充值,默认为1
	*/
	void Clear(bool bClearFrame, bool bClearZ, const MyColor& frameColor, float zValue = 1.f);

	void ClearZBuffer(float zValue = 1.f);

	IDirect3DDevice9* GetD3DDevice() { return m_pd3dDevice; }


	/** 屏幕的显示象素  宽 */
	inline int GetScreenWidth() const { return m_d3dsdBackBuffer.Width; }
	/** 屏幕的显示象素  高 */
	inline int GetScreenHeight() const { return m_d3dsdBackBuffer.Height; }

	/*------------------------------------------------------------------------------------------------*/
	//设置照相机参数
	/** 设置投影矩阵数据	 */
	Matrix4 SetProjectionData(
		float inFov,
		float inNear,
		float inFar);

	/** 得到目前的投影矩阵数据 */
	void GetProjectionData(
		float* inFov,
		float* inNear,
		float* inFar);

	/** 默认视域变换 */
	void MakeViewport();

	/** 得到视域变换矩阵 */
	void GetViewMatrix(Matrix4* pMat);
	/** 设置视域变换矩阵 */
	void SetViewMatrix(const Matrix4& mat);

	/** 得到投影变换矩阵 */
	void GetProjectionMatrix(Matrix4* pMat);
	/** 设置投影变换矩阵 */
	void SetProjectionMatrix(const Matrix4& mat);

	/** 得到世界变换矩阵 */
	void GetWorldMatrix(Matrix4* pMat);
	/** 设置世界变换矩阵 */
	void SetWorldMatrix(const Matrix4& mat);
	/*------------------------------------------------------------------------------------------------*/

	/*------------------------------------------------------------------------------------------------*/
	//设置光源
	HRESULT SetLight(DWORD LightIndex, MyLight * light);
	HRESULT GetLight(DWORD LightIndex, MyLight * light);
	HRESULT LightEnable(DWORD LightIndex, bool bEnable);

	/*------------------------------------------------------------------------------------------------*/
	//设置渲染状态
	HRESULT SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	HRESULT SetFVF(DWORD FVF);
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

	/*------------------------------------------------------------------------------------------------*/
	//设置贴图
	bool SetTexture(int stage, const string& strName);
	bool SetTexture(int stage, ResHandle resHandle);
	bool SetTexture(int stage, My2DTexture * texture);

	/** 绘制基本图元, 不采用 vertex buffer 的情况
	* @param[D3DPRIMITIVETYPE] PrimitiveType: 绘制方法--D3DPT_POINTLIST.	点绘制
	*													D3DPT_LINELIST.		线条列表
	*													D3DPT_LINESTRIP.	条形线条
	*													D3DPT_TRIANGLELIST.	三角列表
	*													D3DPT_TRIANGLESTRIP.三角条形
	*													D3DPT_TRIANGLEFAN.	三角扇形
	*													D3DPT_FORCE_DWORD.	还没用到
	* @param[UINT] PrimitiveCount: 图元数量
	* @param[const void *] pVertexStreamZeroData: 内存起始位置
	* @param[UINT] VertexStreamZeroStride: 每个顶点的内存大小
	*/
	HRESULT DrawPrimitiveUp(D3DPRIMITIVETYPE PrimitiveType,
		UINT PrimitiveCount,
		const void *pVertexStreamZeroData,
		UINT VertexStreamZeroStride);
};


#endif
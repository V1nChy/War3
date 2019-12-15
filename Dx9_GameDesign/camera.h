#pragma once

#include <d3dx9.h>

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };//地面巡航  随意飞行

	Camera();
	Camera(CameraType cameraType);
	~Camera();

	void InitCamera(D3DXVECTOR3& CameraPos,D3DXVECTOR3& LookAtPos,D3DXVECTOR3& UpPos);

	//平移函数
	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward
	
	//旋转函数
	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector


	void setCameraType(CameraType cameraType); 
	void setPosition(D3DXVECTOR3* pos);
	void setRight(D3DXVECTOR3* right);
	void setUp(D3DXVECTOR3* up);
	void setLook(D3DXVECTOR3* look);
	void setMoveRate (float rate);
	void setupProjection(LPDIRECT3DDEVICE9 pDevice,FLOAT fovY,FLOAT Aspect,FLOAT zn,FLOAT zf);//设置摄像机的透视投影矩阵
	void Zoom(float fStep);//调整摄影机的距离

	void UpdateViewMatrix(LPDIRECT3DDEVICE9 pDevice);
	void getPosition(D3DXVECTOR3* pos); 
	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

	const D3DXMATRIX&	GetViewMatrix() { return m_matView; }
	const D3DXMATRIX&	GetProjMatrix() { return m_matProj; }
	const D3DXMATRIX&	GetInvProjMatrix() { return m_matInvProj; }
	const D3DXMATRIX&	GetViewProjMatrix() { return m_matViewProj; }
	const D3DXMATRIX&	GetInvViewProjMatrix() { return m_matInvViewProj; }
	const D3DXMATRIX&	GetInvViewMatrix() { return m_matInvView; }

	
private:
	CameraType  _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
	float _fMoveRate;

	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;
	D3DXMATRIX		m_matViewProj;
	D3DXMATRIX		m_matInvView;
	D3DXMATRIX		m_matInvProj;
	D3DXMATRIX		m_matInvViewProj;
};
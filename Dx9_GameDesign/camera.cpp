#include "camera.h"

Camera::Camera()
{
	_cameraType = AIRCRAFT;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	_fMoveRate = 1.0f;

	memset(&m_matView, 0, sizeof(D3DXMATRIX));
	memset(&m_matProj, 0, sizeof(D3DXMATRIX));
	memset(&m_matInvProj, 0, sizeof(D3DXMATRIX));
	memset(&m_matViewProj, 0, sizeof(D3DXMATRIX));
	memset(&m_matInvViewProj, 0, sizeof(D3DXMATRIX));
	memset(&m_matInvView, 0, sizeof(D3DXMATRIX));
}

Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	_fMoveRate = 1.0f;
}

Camera::~Camera()
{

}

void Camera::InitCamera(D3DXVECTOR3& CameraPos,D3DXVECTOR3& LookAtPos,D3DXVECTOR3& UpPos)
{
	D3DXVECTOR3 vLookAt=LookAtPos-CameraPos;
	D3DXVECTOR3 vUp=UpPos;

	D3DXVec3Normalize(&vLookAt, &vLookAt);

	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vLookAt );  //DirectX可使用左手判断叉积方向 
	D3DXVec3Normalize(&vRight,&vRight);

	D3DXVec3Cross(&vUp, &vLookAt, &vRight);
	D3DXVec3Normalize(&vUp,&vUp);

	_pos=CameraPos;
	_right=vRight;
	_look=vLookAt;
	_up=vUp;
}

void Camera::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

void Camera::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void Camera::setRight(D3DXVECTOR3* right)
{
	_right = *right;
}

void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}
void Camera::setUp(D3DXVECTOR3* up)
{
	_up = *up;
}
void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}
void Camera::setLook(D3DXVECTOR3* look)
{
	_look = *look;
}

void Camera::Zoom(float fStep) //调整摄影机的距离
{
	_pos += (_pos - _look) * fStep;
}

void Camera::setMoveRate (float rate)
{
	_fMoveRate = rate ;
}

void Camera::walk(float units)
{
	// move only on xz plane for land object
	units *= _fMoveRate;
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _look * units;
}

void Camera::strafe(float units)
{
	// move only on xz plane for land object
	units *= _fMoveRate;
	if( _cameraType == LANDOBJECT )
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if( _cameraType == AIRCRAFT )
		_pos += _right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	units *= _fMoveRate;
	if( _cameraType == LANDOBJECT )
		_pos.y += units;

	if( _cameraType == AIRCRAFT )
		_pos += _up * units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right,	angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up,&_up, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( _cameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( _cameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_look,&_look, &T);
}

void Camera::roll(float angle)
{
	// only roll for aircraft type
	if( _cameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&_right,&_right, &T);
		D3DXVec3TransformCoord(&_up,&_up, &T);
	}
}

void Camera::UpdateViewMatrix(LPDIRECT3DDEVICE9 pDevice)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	D3DXMATRIX& V = m_matView;

	V(0, 0) = _right.x; V(0, 1) = _up.x; V(0, 2) = _look.x; V(0, 3) = 0.0f;
	V(1, 0) = _right.y; V(1, 1) = _up.y; V(1, 2) = _look.y; V(1, 3) = 0.0f;
	V(2, 0) = _right.z; V(2, 1) = _up.z; V(2, 2) = _look.z; V(2, 3) = 0.0f;
	V(3, 0) = x;        V(3, 1) = y;     V(3, 2) = z;       V(3, 3) = 1.0f;

	D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);

	D3DXMATRIX& iV = m_matInvView;

	iV(0, 0) = V(0, 0);		iV(0, 1) = V(1, 0);		iV(0, 2) = V(2, 0);

	iV(1, 0) = V(0, 1);		iV(1, 1) = V(1, 1);		iV(1, 2) = V(2, 1);

	iV(2, 0) = V(0, 2);		iV(2, 1) = V(1, 2);		iV(2, 2) = V(2, 2);

	iV(3, 0) = _pos.x;		iV(3, 1) = _pos.y;		iV(3, 2) = _pos.z;

	iV(3, 3) = 1.0f;

	D3DXMatrixMultiply(&m_matInvViewProj, &m_matInvProj, &m_matInvView);

	pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}


void Camera::setupProjection( LPDIRECT3DDEVICE9 pDevice,FLOAT fovY,FLOAT Aspect,FLOAT zn,FLOAT zf)
{
	D3DXMatrixPerspectiveFovLH( &m_matProj, fovY, Aspect, zn, zf );
	pDevice->SetTransform( D3DTS_PROJECTION, &m_matProj);

	//计算透视投影逆矩阵

	float fny = 1 / tan(fovY / 2);

	float fnx = fny / Aspect;

	float a = zf / (zf - zn);

	float b = -zn*zf / (zf - zn);

	D3DXMATRIX& V = m_matInvProj;

	memset(&V, 0, sizeof(D3DXMATRIX));

	V(0, 0) = 1 / fnx;

	V(1, 1) = 1 / fny;

	V(3, 2) = 1;

	V(2, 3) = 1 / b;

	V(3, 3) = -a / b;
}



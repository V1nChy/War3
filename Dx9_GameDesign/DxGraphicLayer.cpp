#include "DxGraphicLayer.h"
#include "MyUTIL.h"

DxGraphicLayer* DxGraphicLayer::m_pGraphicLayer = new DxGraphicLayer;

DxGraphicLayer::DxGraphicLayer():
	m_pD3D(NULL),
	m_pd3dDevice(NULL)
{
}
DxGraphicLayer::~DxGraphicLayer()
{

}
void DxGraphicLayer::FinalCleanup()
{
	ReleaseD3D9();
	ReleaseD3D9Device();
	SAFE_DELETE(m_pGraphicLayer);
}
void DxGraphicLayer::ReleaseD3D9()
{
	SAFE_RELEASE(m_pD3D);
}
void DxGraphicLayer::ReleaseD3D9Device()
{
	if (m_pd3dDevice)
		if (m_pd3dDevice->Release() > 0)
		{
			//LOGINFO("没有释放所有资源");
		}
	m_pd3dDevice = NULL;
}

HRESULT DxGraphicLayer::CreateD3D9()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (NULL == m_pD3D)
		return S_FALSE;

	return S_OK;
}
HRESULT DxGraphicLayer::CreateD3DDevice()
{
	//判断显卡是否支持硬件顶点处理
	D3DCAPS9 caps;

	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	//取得目标桌面显示模式
	D3DDISPLAYMODE d3ddm;
	if (m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm) != D3D_OK)
		return false;

	//建立绘图设备
	//D3DPRESENT_PARAMETERS d3dpp;
	//memset(&d3dpp, 0, sizeof(d3dpp));
	//d3dpp.Windowed = TRUE; //创建窗口模式的Direct3D程序
	//d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.BackBufferFormat = d3ddm.Format;

	////d3dpp.MultiSampleType = multiType;//多重采样

	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//创建32位深度/模板缓存，24位深度，8为模板
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));
	d3dpp.Windowed = TRUE; //创建窗口模式的Direct3D程序
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	//创建设备对象，并获取接口IDirect3DDevice9的指针
	if (m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, //使用缺省的显卡
		D3DDEVTYPE_HAL,
		m_hWnd,
		vp,
		&d3dpp,
		&m_pd3dDevice) != D3D_OK)
		return false;

	return true;
}
HRESULT DxGraphicLayer::Initialize3DEnvironment(HWND hwnd)
{
	m_hWnd = hwnd;
	HRESULT hr = S_OK;
	hr &= CreateD3D9();
	if (SUCCEEDED(hr))
	{
		hr &= CreateD3DDevice();
		_StoreBackSurfDesc();
		return hr;
	}

	return hr;

}

void DxGraphicLayer::Clear(bool bClearFrame,bool bClearZ,const MyColor& frameColor,float zValue)
{
	DWORD flags = bClearFrame ? D3DCLEAR_TARGET : 0L;
	flags |= bClearZ ? D3DCLEAR_ZBUFFER : 0L;

	HRESULT R = m_pd3dDevice->Clear(0, NULL, flags, frameColor.GetAsARGB(), zValue, 0);

}

void DxGraphicLayer::ClearZBuffer(float zValue)
{
	static const D3DCOLOR COLOR = MyColor::NoColor.GetAsARGB();
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, COLOR, zValue, 0);
}

void DxGraphicLayer::_StoreBackSurfDesc()
{
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pBackBuffer->GetDesc(&m_d3dsdBackBuffer);
	pBackBuffer->Release();
}

Matrix4 DxGraphicLayer::SetProjectionData(
	float inFov,
	float inNear,
	float inFar)
{
	m_fov = inFov;
	m_far = inFar;
	m_near = inNear;

	Matrix4 mat;

	DWORD width, height;
	width = GetScreenWidth();
	height = GetScreenHeight();

	float fAspect = ((float)height) / width;

	if (fabsf(m_far - m_near) < 0.01f)
		return S_FALSE;
	if (fabsf(sinf(m_fov / 2)) < 0.01f)
		return S_FALSE;

	float w = fAspect * (float)(cosf(m_fov / 2) / sinf(m_fov / 2));
	float h = 1.0f  * (float)(cosf(m_fov / 2) / sinf(m_fov / 2));
	float Q = m_far / (m_far - m_near);

	mat.Zero();
	mat[0][0] = w;
	mat[1][1] = h;
	mat[2][2] = Q;
	mat[2][3] = 1.0f;
	mat[3][2] = -Q*m_near;

	return mat;
}

void DxGraphicLayer::GetProjectionData(
	float* inFov,
	float* inNear,
	float* inFar)
{
	if (inFov)
		*inFov = m_fov;
	if (inFar)
		*inFar = m_far;
	if (inNear)
		*inNear = m_near;
}

HRESULT DxGraphicLayer::DrawPrimitiveUp(D3DPRIMITIVETYPE PrimitiveType,
	UINT PrimitiveCount,
	const void *pVertexStreamZeroData,
	UINT VertexStreamZeroStride)
{
	return m_pd3dDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

/*------------------------------------------------------------------------------------------------*/
//设置光源
HRESULT DxGraphicLayer::SetLight(DWORD LightIndex, MyLight * light)
{
	return m_pd3dDevice->SetLight(LightIndex, light->GetLight());
}

HRESULT DxGraphicLayer::GetLight(DWORD LightIndex, MyLight * light)
{
	return m_pd3dDevice->GetLight(LightIndex, light->GetLight());
}

HRESULT DxGraphicLayer::LightEnable(DWORD LightIndex, bool bEnable)
{
	return m_pd3dDevice->LightEnable(LightIndex, bEnable);
}

/*------------------------------------------------------------------------------------------------*/
//设置照相机参数
void DxGraphicLayer::GetViewMatrix(Matrix4* pMat)
{
	// Pass the call to the Direct3D Device
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	m_pd3dDevice->GetTransform(D3DTS_VIEW, &mat);

	pMat->SetMatrix((float *)&mat);
}

void DxGraphicLayer::SetViewMatrix(const Matrix4& mat)
{
	D3DMATRIX* vm = (D3DMATRIX*)(mat.Get4_4Array());

	// Pass the call to the Direct3D Device
	if (m_pd3dDevice->SetTransform(D3DTS_VIEW, vm) != D3D_OK)
		LOGINFO("View transform failed");
}

void DxGraphicLayer::GetProjectionMatrix(Matrix4* pMat)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &mat);

	pMat->SetMatrix((float *)&mat);
}

void DxGraphicLayer::SetProjectionMatrix(const Matrix4& mat)
{
	D3DMATRIX* vm = (D3DMATRIX*)(mat.Get4_4Array());

	// Pass the call to the Direct3D Device
	if (m_pd3dDevice->SetTransform(D3DTS_PROJECTION, vm) != D3D_OK)
		LOGINFO("View transform failed");
}

void DxGraphicLayer::SetWorldMatrix(const Matrix4& mat)
{
	D3DMATRIX* vm = (D3DMATRIX*)(mat.Get4_4Array());

	// Pass the call to the Direct3D Device
	m_pd3dDevice->SetTransform(D3DTS_WORLD, vm);
}

void DxGraphicLayer::GetWorldMatrix(Matrix4* pMat)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	m_pd3dDevice->GetTransform(D3DTS_WORLD, &mat);

	pMat->SetMatrix((float *)&mat);
}

void DxGraphicLayer::MakeViewport()
{
	HRESULT hr;
	if (!m_pd3dDevice)
	{
		//		DP("[cGraphicsLayer::MakeViewport]: no device\n");
		return;
	}

	D3DVIEWPORT9 vp = { 0, 0, GetScreenWidth(), GetScreenHeight(), 0.0f, 1.0f };

	hr = m_pd3dDevice->SetViewport(&vp);
	if (FAILED(hr))
		return;
}

/*------------------------------------------------------------------------------------------------*/
//设置贴图
bool DxGraphicLayer::SetTexture(int stage, const string& strName)
{
	My2DTexture * txt = (My2DTexture *)MyTextureManager::GetSingleton()->GetResource(strName);

	if (!txt)
	{
		string err = "Can't find this resource: ";
		err += strName;
		LOGINFO(err.c_str());
		return false;
	}

	return SetTexture(stage, txt);
}

bool DxGraphicLayer::SetTexture(int stage, ResHandle resHandle)
{
	My2DTexture * txt = (My2DTexture *)MyTextureManager::GetSingleton()->GetResource(resHandle);

	if (!txt)
	{
		string err = "Can't find this resource handle: ";
		err += resHandle;
		LOGINFO(err.c_str());
		return false;
	}

	return SetTexture(stage, txt);
}

bool DxGraphicLayer::SetTexture(int stage, My2DTexture * texture)
{
	if (!texture)
	{
		LOGINFO("Set texture failed, it doesn't exist.");
		return false;
	}

	texture->Load();

	// make no dual texture setting
	//if (_pLastTexture == texture)
	//	return true;
	//else
	//	_pLastTexture = texture;

	if (m_pd3dDevice->SetTexture(stage, texture->GetTexture()) != D3D_OK)
	{
		string err = "Set texture failed, it may not load successfully";
		LOGINFO(err.c_str());
		return false;
	}
	else
		return true;
}


HRESULT DxGraphicLayer::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return m_pd3dDevice->SetRenderState(State, Value);
}

HRESULT DxGraphicLayer::SetFVF(DWORD FVF)
{
	return m_pd3dDevice->SetFVF(FVF);
}

HRESULT DxGraphicLayer::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return m_pd3dDevice->SetTextureStageState(Stage, Type, Value);
}
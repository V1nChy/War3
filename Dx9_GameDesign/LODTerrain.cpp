#include <vector>
#include <fstream>
#include "LODterrain.h"
#include "LOD.h"
#include "PerlinNoise.h"
#include "PerlinNoise.cpp"

using namespace std;
CTerrain::CTerrain()
{
	m_D3D			= NULL;
	m_pVB			= NULL;
	m_Tex			= NULL;
	m_LOD			= NULL;
	m_pSections		= NULL;			
	m_HeightTable	= NULL;		
	m_NormalTable	= NULL;
	m_pVertexDecl	= NULL;
	m_Size			= 0;
	m_SectionCountSq= 0;

	SetSunDir(D3DXVECTOR3(1,1,1));
}

CTerrain::~CTerrain()
{
	m_D3D				= NULL;
	m_LOD				= NULL;
	m_pVB				= NULL;
	m_Size				= 0;
	m_SectionCountSq	= 0;


	m_VS.Release();
	safe_release(m_pVB);
	safe_release(m_pVertexDecl);
	safe_release(m_Tex);
	safe_delete_array(m_pSections);
	safe_delete_array(m_HeightTable);
	safe_delete_array(m_NormalTable);
}


bool CTerrain::Create(IDirect3DDevice9* pDevice,
					  LPCSTR lpszHeightMapName,
					  unsigned int size,
					  const CubeEx& World
					  )
{
	

	unsigned int input = size;
	register int result;
	_asm bsr eax, input
	_asm mov result, eax
	unsigned int test = ~(1<<result);


	assert(((test & input) == 0 || (test & input) == 1) 
		&& "地图大小设置错误，请确保地图的大小必须是2的幂或者+1");	
	assert(size>= (1<<e_SectionLevel)
		&& "地图大小设置错误，请确保地图的大小要大于等于最小单位为地形块的大小");


	m_D3D		= pDevice;
	m_Size		= size;
	m_rtWorld	= World;

	if ((test & input) == 1) --m_Size;

	
	bool re = true;

	//通过文件加载高度图
	if (lpszHeightMapName)
	{
		re = LoadHeightMap(lpszHeightMapName);

		if (!re)
		{
			::MessageBox(0,"加载高度图失败",0,0);
			return false;
		}
	}
	else//柏林噪音计算高度图
	{
		if(!PerlinHeightMap()) return false;
	}


	//计算法向量图
	re = ComputeNormalMap();
	
	if (!re)
	{
		::MessageBox(0,"计算地图法向量失败",0,0);
		return false;
	}

	//创建顶点和索引缓存
	CreateBuffer();

	//顶点着色
	m_VS.Create(m_D3D);

	//创建地形纹理
	D3DXCreateTextureFromFile(
		m_D3D,
		"Textures\\Grass2.jpg",
		&m_Tex);

	//创建顶点描述
	D3DVERTEXELEMENT9 vertex_desc[]=
	{
		// 流0
		{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },				
		{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 0 },

		// 流1
		{ 1, 0, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
		{ 1, 4, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },

		D3DDECL_END()
	};
	m_D3D->CreateVertexDeclaration(vertex_desc, &m_pVertexDecl);

	return true;
}
void CTerrain::SetLODType(CLOD* LOD)
{
	m_LOD = LOD;
}

void CTerrain::Render(const LODmath::Frustum& F)
{

	assert(m_LOD);

	//设置顶点申明和公用流0
	m_D3D->SetVertexDeclaration(m_pVertexDecl);
	m_D3D->SetStreamSource(0, m_pVB, 0, sizeof(SSectionVertex));
	m_D3D->SetVertexShader(m_VS.TerShader);
	m_D3D->SetTexture(0, m_Tex);

	m_D3D->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_D3D->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	m_D3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_D3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_D3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE2X);

	
	m_LOD->Build(F);

	m_D3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	m_D3D->SetTexture(0, 0);
	m_D3D->SetVertexDeclaration(0);
	m_D3D->SetVertexShader(0);


}

bool CTerrain::LoadHeightMap(LPCSTR lpszMapName)
{
	//地图补成2^n + 1
	unsigned int factSize = m_Size + 1;
	std::vector<BYTE> in( factSize*factSize );
	
	std::ifstream inFile(lpszMapName, std::ios_base::binary);

	if( !inFile )
		return false;

	inFile.read(
		(char*)&in[0],
		m_Size*m_Size);

	inFile.close();

	m_HeightTable = new float[factSize*factSize];
	
	//读取外部数据，并且把地图由2的幂扩展到2的幂+1
	//由于世界坐标和高度图的坐标成镜像，所以写的时候也镜像的在写一次
	int Test = 0,n1 = 0;
	for( int j=m_Size-1; j>=0; --j)
	{
		unsigned int i;
		for (i = 0; i<m_Size; ++i)
		{
			m_HeightTable[j*(m_Size+1)+i] = in[n1++];
			++Test;
		}
		m_HeightTable[j*(m_Size+1)+i] = in[n1-1];
		++Test;
	}
	
	for (unsigned int i = 0; i < m_Size+1; ++i)
	{
		m_HeightTable[m_Size*(m_Size+1)+i] = m_HeightTable[(m_Size-1)*(m_Size+1)+i];
		++Test;
	}
	assert(Test == factSize*factSize);

	m_Size = factSize;
	return true;
}

bool CTerrain::ComputeNormalMap()
{

	//每个顶点的法向量是根据它相邻的4个顶点所形成的法向量的平均值

	MSG msg = {0,0,0,0,0,{0,0}};

	if (NULL != m_NormalTable )
	{
		safe_delete_array(m_NormalTable);
	}

	m_NormalTable = new D3DXVECTOR3[m_Size*m_Size];

	for (unsigned int y = 0; y < m_Size; ++y)
	{
		for (unsigned int x = 0; x < m_Size; ++x)
		{

			D3DXVECTOR3 lelf(0.0f,0.0f,0.0f);
			D3DXVECTOR3 right(0.0f,0.0f,0.0f);
			D3DXVECTOR3 up(0.0f,0.0f,0.0f);
			D3DXVECTOR3 down(0.0f,0.0f,0.0f);

			D3DXVECTOR3 cur = GetPos(x,y);

			if (x > 0)			lelf	= GetPos(x-1, y)	- cur;
			if (x+1 < m_Size)	right	= GetPos(x+1, y)	- cur;
			if (y+1 < m_Size)	up		= GetPos(x,	  y+1)	- cur;
			if (y > 0)			down	= GetPos(x,	  y-1)	- cur;

			D3DXVECTOR3 lu, ru, ld, rd;

			D3DXVec3Cross(&lu,&lelf,&up);
			D3DXVec3Cross(&ru,&up,&right);
			D3DXVec3Cross(&ld,&down,&lelf);
			D3DXVec3Cross(&rd,&right,&down);

			int average = 0;

			if (x>0 && y>0)				 {D3DXVec3Normalize(&ld,&ld); average++;}
			if (x>0 && y+1<m_Size)		 {D3DXVec3Normalize(&lu,&lu); average++;}
			if (y>0 && x+1<m_Size)		 {D3DXVec3Normalize(&rd,&rd); average++;}
			if (x+1<m_Size && y+1<m_Size){D3DXVec3Normalize(&ru,&ru); average++;}

			int index = y*m_Size + x;

			D3DXVECTOR3 cur_normal = (lu+ru+ld+rd)/(float)average;

			D3DXVec3Normalize(&cur_normal,&cur_normal);

			m_NormalTable[index] = cur_normal;

			if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))		
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			if(msg.message == WM_QUIT)
			{
				::PostQuitMessage(0);
				return false;
			}

		}
	}
	
	return true;
}
bool CTerrain::CreateBuffer()
{
	
	//创建顶点缓存
	unsigned int vectSize = sizeof(SSectionVertex)*
									 e_SectionSize*
									 e_SectionSize;

	HRESULT re = m_D3D->CreateVertexBuffer(
										vectSize,
										0,
										0,
										D3DPOOL_MANAGED,
										&m_pVB,
										0);
	if (re != D3D_OK)
	{
		assert(!"创建共享顶点缓存失败");
		return false;
	}
		
	SSectionVertex* pVerts;

	m_pVB->Lock(0,0,(void**)&pVerts,0);
		
	D3DXVECTOR3	WorldSize = m_rtWorld.size();
	D3DXVECTOR2 cellSize(WorldSize.x/(m_Size-1), WorldSize.z/(m_Size-1));
	D3DXVECTOR2 set(0.0f,0.0f);

	for (int y=0; y<e_SectionSize; ++y)
	{
		set = D3DXVECTOR2(0.0f,y*cellSize.y);

		for (int x=0; x<e_SectionSize; ++x)
		{
			pVerts[(y*e_SectionSize)+x].xyPos = set;
			pVerts[(y*e_SectionSize)+x].uv = D3DXVECTOR2(
				(float)x/(float)(e_SectionSize-1),
				(float)y/(float)(e_SectionSize-1));

			set.x += cellSize.x;
		}
	}

	m_pVB->Unlock();


	//创建地形块
	unsigned int TileCount;

	TileCount = (m_Size-1)/(e_SectionSize-1);
	m_SectionCountSq = TileCount;
	TileCount = TileCount * TileCount;

	m_pSections = new STerrSection[TileCount];

	for (unsigned int i = 0; i < TileCount; ++i)
	{
		CreateTileBuffer(i);
	}

	return true;
}
bool CTerrain::CreateTileBuffer(unsigned int i)
{
	assert(i < m_SectionCountSq*m_SectionCountSq && !m_pSections[i].m_pVB);

	unsigned int vectSize = sizeof(SVertex)*
							  e_SectionSize*
							  e_SectionSize;

	 HRESULT re = m_D3D->CreateVertexBuffer(
		vectSize,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pSections[i].m_pVB,
		0);
	if (re != D3D_OK)
	{
		assert(!"创建顶点缓存失败");
		return false;
	}

	SVertex* pVerts;
	m_pSections[i].m_pVB->Lock(0,0,(void**)&pVerts,0);


	for (int y=0; y<e_SectionSize; ++y)
	{
		for (int x=0; x<e_SectionSize; ++x)
		{
			assert((y*e_SectionSize)+x < e_SectionSize*e_SectionSize);

			pVerts[(y*e_SectionSize)+x].height = GetTileHeight(i,x,y);

			pVerts[(y*e_SectionSize)+x].Normal = GetTileNormal(i,x,y);

		}
	}

	m_pSections[i].m_pVB->Unlock();


	return true;
}
bool CTerrain::PerlinHeightMap()
{
	//如果没有提供地图，则由这个函数完成
	++m_Size;

	//柏林噪声对每个顶点进行了5次降噪,需要点时间，画一个进度条

	float lastTime = (float)timeGetTime(); 
	float currTime = 0.0f;
	float timeDelta = 0.0f;

	ID3DXFont*		BarFont = NULL;

	CreateDefaultID3DXFont(	m_D3D,&BarFont,"微软雅黑",25,10);
												
	D3DVIEWPORT9 vp;

	m_D3D->GetViewport(&vp);

	//IUICtrlManager*		cm = NULL;

	////设置载入地形数据的进度条
	//CreateCtrlManager(&cm);

	//cm->Init(m_D3D);

	//IUICtrl* BarBackGround		= NULL;
	//IUICtrl* BarBackGround2		= NULL;
	//IUICtrl* CurProgress		= NULL;
	//IUICtrl* UIBarFont			= NULL;

	//int w = vp.Width-100;

	//int h = 50;

	//unsigned int AllCount = m_Size*m_Size;

	//cm->CreateCtrl(CT_CTRL,(void**)&BarBackGround);
	//cm->CreateCtrl(CT_CTRL,(void**)&BarBackGround2);
	//cm->CreateCtrl(CT_CTRL,(void**)&CurProgress);
	//cm->CreateCtrl(CT_CTRL,(void**)&UIBarFont);

	//BarBackGround->SetPos(D3DXVECTOR2(float((vp.Width-w)/2),float((vp.Height-h)/2)));
	//BarBackGround->SetSize(D3DXVECTOR2(float(w),float(h)));
	//BarBackGround->SetColor(0xffffff00);
	//BarBackGround2->SetPos(BarBackGround->GetPos()+D3DXVECTOR2(2,2));
	//BarBackGround2->SetSize(BarBackGround->GetSize()-D3DXVECTOR2(4,4));
	//BarBackGround2->SetColor(0xff000000);

	//int CurAllW = int(BarBackGround2->GetSize().x-4);
	//int CurAllH = int(BarBackGround2->GetSize().y-4);

	//CurProgress->SetPos(BarBackGround2->GetPos()+D3DXVECTOR2(2,2));
	//CurProgress->SetSize(D3DXVECTOR2(0.0f,float(CurAllH)));
	//CurProgress->SetColor(0xff00ffff);

	//UIBarFont->SetPos(BarBackGround->GetPos()+D3DXVECTOR2(0.0f,h+10.0f));
	//UIBarFont->SetSize(BarBackGround->GetSize());
	//UIBarFont->SetColor(0x00000000);
	//UIBarFont->AttachID3DXFont(BarFont,0,0,DT_CENTER|DT_VCENTER,-1,false,0xffffff00);

	char string[50];
    



	CPerlinNoise2D PL;
	
	m_HeightTable = new float[m_Size*m_Size];

	float invfre = 0.013f;

	//对于每个点进行噪音处理
	for (int y=0; y<int(m_Size); ++y)
	{
		for (int x=0; x<int(m_Size); ++x)
		{
			float accum = 0;

			//总共5次噪音，第1、2次呈现大的山峦,3、4、5次实现小山包
			//倍频分别为0.8, 1.0, 3.0, 3.4, 3.7
			//振幅分别为1.0, 0.9, 0.17, 0.12， 0.1
			accum += PL.Noise(x*invfre,y*invfre,0.8f)*1.0f;
			accum += PL.Noise(x*invfre+0.3f,y*invfre+0.3f,1.0f)*0.9f;
			accum += PL.Noise(x*invfre,y*invfre,3.0f)*0.17f;
			accum += PL.Noise(x*invfre,y*invfre,3.4f)*0.12f;
			accum += PL.Noise(x*invfre,y*invfre,3.7f)*0.1f;

			accum *= 0.4f;
			accum += 0.5f;		

			D3DXVECTOR3	WorldSize = m_rtWorld.size();

			float scaleY = WorldSize.y;

			float transY = m_rtWorld.Pos0.y;
			
			m_HeightTable[y*m_Size+x] = scaleY*accum + transY;

			

			//绘制地形数据加载进度条

			currTime  = (float)timeGetTime();

			timeDelta = (currTime - lastTime);

			if(timeDelta < 15)	continue;

			/*float now = float(y*m_Size + x)/AllCount;

			CurProgress->SetSize(D3DXVECTOR2(float(now*CurAllW),float(CurAllH)));

			sprintf( string, "地形数据载入中,请稍后...\n%d%%", int(now*100)+1 );

			UIBarFont->SetAttachFontText(string);

			if(!DrawProgressBar(timeDelta,cm)) return false;*/
			
			lastTime = currTime;

		}
	}

	/*CurProgress->SetSize(D3DXVECTOR2(float(CurAllW),float(CurAllH)));

	UIBarFont->SetAttachFontText("地形数据载入中,请稍后...\n100%");

	DrawProgressBar(0,cm);

	::Sleep(700);

	UIBarFont->SetAttachFontText("正在计算地形法向量...");

	DrawProgressBar(0,cm);

	ReleaseCtrlManager(&cm);

	BarFont->Release();*/

	return true;

} 
//bool CTerrain::DrawProgressBar(float dt,IUICtrlManager* cm)	
//								 	
//{
//	static MSG msg = {0,0,0,0,0,{0,0}};
//
//	static POINT pt = {-1,-1};
//	
//
//	if(msg.message == WM_QUIT)
//	{
//		::PostQuitMessage(0);
//		return false;
//	}
//
//
//	if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))		
//	{
//		::TranslateMessage(&msg);
//		::DispatchMessage(&msg);
//	}
//	
//	m_D3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
//
//	m_D3D->BeginScene();
//
//	cm->Rander(dt,pt,0);
//
//	m_D3D->EndScene();
//
//	m_D3D->Present(0, 0, 0, 0);
//	
//	
//	
//
//	return true;
//}
#pragma once
#include <d3dx9.h>
#include <vector>
#include "MyCamera.h"
#include "LODmath.h"
#include "Shaders.h"
using namespace LODmath;
class CLOD;
//地形类,把整个地形按块状来分则可共享顶点数据
//块的大小初定为129*129，若地形的大小是2049*2049则块的个数为16*16块
class CTerrain
{

private:
	//顶点数据分为2个流
	//SSectionVertex提供给地方块共享的数据
	//SVertex是地形的全局数据，对于地形块来说是唯一的
	//STerrSection是地形块，是独立数据
	struct SSectionVertex
	{
		D3DXVECTOR2 xyPos;
		D3DXVECTOR2 uv;
	};
	struct SVertex
	{
		float		height;
		D3DXVECTOR3 Normal;
	};
	struct STerrSection
	{
		STerrSection():m_pVB(0){}
		IDirect3DVertexBuffer9*	m_pVB;				
	};
	// 常量定义
	enum eConstants
	{
		e_SectionLevel = 6,
		e_SectionSize = (1<<e_SectionLevel)+1, 
	};
	
	friend class CLOD_Quad;


	IDirect3DDevice9*				m_D3D;				//设备
	IDirect3DVertexBuffer9*			m_pVB;				//顶点缓存
	IDirect3DTexture9*				m_Tex;				//地表纹理
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDecl;		//顶点申明
	CLOD*							m_LOD;				//层次细节
	STerrSection*					m_pSections;		//地形块
	float*							m_HeightTable;		//高度图
	D3DXVECTOR3*					m_NormalTable;		//法向量图
	unsigned int					m_Size;				//地图大小（宽度=高度）
	unsigned int					m_SectionCountSq;	//块数的长或者宽
	CubeEx							m_rtWorld;			//世界坐标空间
	Terrain_VS						m_VS;				//顶点着色
	D3DXVECTOR3						m_SunDir;		
	
					

	//内部辅助函数...
	bool LoadHeightMap(LPCSTR lpszMapName);				//加载高度图
	bool PerlinHeightMap();								//柏林噪音高度图
	bool ComputeNormalMap();							//计算法向量图
	bool CreateBuffer();								//创建共享顶点和索引缓存
	bool CreateTileBuffer(UINT TileID);					//创建地形块缓存(非共享)
	void SetVSConstantTable(UINT TileID);				//设置顶点着色常量表
	//bool DrawProgressBar(float dt,IUICtrlManager* cm);	//绘制载入地形数据的进度条
									
														
	
															
public:


	const D3DXMATRIX** m_ppViewProj;	

	CTerrain();
	~CTerrain();

	//创建地形 
	//如果高度图文件名为null，则用柏林噪音法计算出高度图
	//地图的大小可以是2的幂+1,也可以是2的幂（程序补成2的幂+1）
	bool Create(IDirect3DDevice9*		D3D,
				LPCSTR					lpszHeightMapName,						
				unsigned int			HeightMapSize,			
				const CubeEx&			World				);
					

	
	void SetLODType(CLOD* LOD);				//设置LOD类型（Quad\Roam...)

	void SetSunDir(const D3DXVECTOR3& dir);	//设置光源
	
	void Render(const LODmath::Frustum& F);			//渲染地形




	float				GetHeight(UINT x,UINT y)				const;//获得高度
	D3DXVECTOR3			GetPos(UINT x,UINT y)					const;//或得位置
	UINT				GetIndex(UINT TileID,UINT x,UINT y)		const;//获得索引
	float				GetTileHeight(UINT TileID,UINT x,UINT y)const;//获得块的高度数据
	const D3DXVECTOR3&	GetTileNormal(UINT TileID,UINT x,UINT y)const;//获得块的法向量数据
	UINT				GetSize(){return m_Size;}
	CubeEx				GetWorld(){return m_rtWorld;}


	
};







//内联函数具体实现...
inline UINT CTerrain::GetIndex(UINT TileID,UINT x,UINT y)const
{
	assert(TileID < m_SectionCountSq*m_SectionCountSq 
			&& x<e_SectionSize && y<e_SectionSize);

	return	(TileID/m_SectionCountSq*(1<<e_SectionLevel)+y)*m_Size
			+TileID%m_SectionCountSq*(1<<e_SectionLevel)+x;
}
inline float CTerrain::GetTileHeight(UINT TileID,UINT x,UINT y)const
{
	return m_HeightTable[GetIndex(TileID,x,y)];
}
inline const D3DXVECTOR3& CTerrain::GetTileNormal(UINT TileID,UINT x,UINT y)const
{
	return m_NormalTable[GetIndex(TileID,x,y)];
}
inline D3DXVECTOR3 CTerrain::GetPos(UINT x,UINT y)const
{
	static D3DXVECTOR3	WorldSize = m_rtWorld.size();

	static D3DXVECTOR2 cellSize(WorldSize.x/(m_Size-1),
								WorldSize.z/(m_Size-1));

	float PosX = m_rtWorld.Pos0.x + x*cellSize.x;
	float PosZ = m_rtWorld.Pos0.z + y*cellSize.y;

	return D3DXVECTOR3(PosX, GetHeight(x,y), PosZ);

}
inline float CTerrain::GetHeight(UINT x,UINT y)const
{
	return m_HeightTable[x+y*m_Size];
}
inline void CTerrain::SetVSConstantTable(UINT TileID)
{
	assert(TileID < m_SectionCountSq*m_SectionCountSq && m_ppViewProj);

	static D3DXVECTOR3	WorldSize = m_rtWorld.size();

	static D3DXVECTOR2 SectionSize(WorldSize.x/(m_Size-1)*(1<<e_SectionLevel),
									WorldSize.z/(m_Size-1)*(1<<e_SectionLevel));

	static float Pos[2],sun[4];

	UINT y = TileID/m_SectionCountSq;
	UINT x = TileID%m_SectionCountSq;

	Pos[0] = m_rtWorld.Pos0.x + x*SectionSize.x;
	Pos[1] = m_rtWorld.Pos0.z + y*SectionSize.y;

	m_VS.ConstTab->SetFloatArray(m_D3D,m_VS.h_PosOffset,Pos,2);

	sun[0] = m_SunDir.x;
	sun[1] = m_SunDir.y;
	sun[2] = m_SunDir.z;
	sun[3] = 0;

	m_VS.ConstTab->SetFloatArray(m_D3D,m_VS.h_SunDir,sun,4);


	m_VS.ConstTab->SetMatrix(
		m_D3D, 
		m_VS.h_ViewProjMatrix,
		*m_ppViewProj
		);
}

inline void CTerrain::SetSunDir(const D3DXVECTOR3& dir)
{
	m_SunDir = dir;
	D3DXVec3Normalize(&m_SunDir, &m_SunDir);
}
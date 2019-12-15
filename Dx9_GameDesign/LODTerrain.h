#pragma once
#include <d3dx9.h>
#include <vector>
#include "MyCamera.h"
#include "LODmath.h"
#include "Shaders.h"
using namespace LODmath;
class CLOD;
//������,���������ΰ���״������ɹ���������
//��Ĵ�С����Ϊ129*129�������εĴ�С��2049*2049���ĸ���Ϊ16*16��
class CTerrain
{

private:
	//�������ݷ�Ϊ2����
	//SSectionVertex�ṩ���ط��鹲�������
	//SVertex�ǵ��ε�ȫ�����ݣ����ڵ��ο���˵��Ψһ��
	//STerrSection�ǵ��ο飬�Ƕ�������
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
	// ��������
	enum eConstants
	{
		e_SectionLevel = 6,
		e_SectionSize = (1<<e_SectionLevel)+1, 
	};
	
	friend class CLOD_Quad;


	IDirect3DDevice9*				m_D3D;				//�豸
	IDirect3DVertexBuffer9*			m_pVB;				//���㻺��
	IDirect3DTexture9*				m_Tex;				//�ر�����
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDecl;		//��������
	CLOD*							m_LOD;				//���ϸ��
	STerrSection*					m_pSections;		//���ο�
	float*							m_HeightTable;		//�߶�ͼ
	D3DXVECTOR3*					m_NormalTable;		//������ͼ
	unsigned int					m_Size;				//��ͼ��С�����=�߶ȣ�
	unsigned int					m_SectionCountSq;	//�����ĳ����߿�
	CubeEx							m_rtWorld;			//��������ռ�
	Terrain_VS						m_VS;				//������ɫ
	D3DXVECTOR3						m_SunDir;		
	
					

	//�ڲ���������...
	bool LoadHeightMap(LPCSTR lpszMapName);				//���ظ߶�ͼ
	bool PerlinHeightMap();								//���������߶�ͼ
	bool ComputeNormalMap();							//���㷨����ͼ
	bool CreateBuffer();								//�������������������
	bool CreateTileBuffer(UINT TileID);					//�������ο黺��(�ǹ���)
	void SetVSConstantTable(UINT TileID);				//���ö�����ɫ������
	//bool DrawProgressBar(float dt,IUICtrlManager* cm);	//��������������ݵĽ�����
									
														
	
															
public:


	const D3DXMATRIX** m_ppViewProj;	

	CTerrain();
	~CTerrain();

	//�������� 
	//����߶�ͼ�ļ���Ϊnull�����ð���������������߶�ͼ
	//��ͼ�Ĵ�С������2����+1,Ҳ������2���ݣ����򲹳�2����+1��
	bool Create(IDirect3DDevice9*		D3D,
				LPCSTR					lpszHeightMapName,						
				unsigned int			HeightMapSize,			
				const CubeEx&			World				);
					

	
	void SetLODType(CLOD* LOD);				//����LOD���ͣ�Quad\Roam...)

	void SetSunDir(const D3DXVECTOR3& dir);	//���ù�Դ
	
	void Render(const LODmath::Frustum& F);			//��Ⱦ����




	float				GetHeight(UINT x,UINT y)				const;//��ø߶�
	D3DXVECTOR3			GetPos(UINT x,UINT y)					const;//���λ��
	UINT				GetIndex(UINT TileID,UINT x,UINT y)		const;//�������
	float				GetTileHeight(UINT TileID,UINT x,UINT y)const;//��ÿ�ĸ߶�����
	const D3DXVECTOR3&	GetTileNormal(UINT TileID,UINT x,UINT y)const;//��ÿ�ķ���������
	UINT				GetSize(){return m_Size;}
	CubeEx				GetWorld(){return m_rtWorld;}


	
};







//������������ʵ��...
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
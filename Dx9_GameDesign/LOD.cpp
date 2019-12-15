#include ".\lod.h"
#include "Terrain.h"

CLOD_Quad::CLOD_Quad()
{
	m_nLevel			= 0;	
	m_data				= NULL;
	m_Fru				= NULL;
	m_pVariant			= NULL;
	m_Cam				= NULL;
	m_pIB				= NULL;
	m_factor			= 0.001f;
	m_cellSize			= 0;
	m_BuildTrianlges	= 0;
	memset(m_AdjctAct,0,sizeof(m_AdjctAct));
}

CLOD_Quad::~CLOD_Quad()
{
	m_nLevel	= 0;
	m_cur_Level = 0;
	m_data		= NULL;
	m_Fru		= NULL;
	m_Cam		= NULL;
	safe_delete_array(m_pVariant);
	safe_release(m_pIB);


}
float CLOD_Quad::InitVariant(int level, int x, int y)
{

	assert(m_data);
	assert(x>=0 && y>=0);
	assert(x < (int)m_data->m_Size && y < (int)m_data->m_Size);

	float Var10[10] = {0};

	int index = y*m_data->m_Size + x;

	int iter = 0;

	if (level > 1)
	{
		unsigned int Size = 1<<(level-2);

		Var10[iter++] = InitVariant(level-1, x+Size, y+Size);
		Var10[iter++] = InitVariant(level-1, x-Size, y-Size);
		Var10[iter++] = InitVariant(level-1, x+Size, y-Size);
		Var10[iter++] = InitVariant(level-1, x-Size, y+Size);
	}

	unsigned int Size = 1<<(level-1);

	float lu = m_data->GetHeight(x-Size, y+Size);
	float ld = m_data->GetHeight(x-Size, y-Size);
	float ru = m_data->GetHeight(x+Size, y+Size);
	float rd = m_data->GetHeight(x+Size, y-Size);

	float left = (lu + ld)/2.0f;

	Var10[iter++] =abs(m_data->GetHeight(x-Size, y) - left);

	float right = (ru + rd)/2.0f;

	Var10[iter++] =abs(m_data->GetHeight(x+Size, y) - right);

	float up = (ru + lu)/2.0f;

	Var10[iter++] =abs(m_data->GetHeight(x, y+Size) - up);

	float down = (rd + ld)/2.0f;

	Var10[iter++] =abs(m_data->GetHeight(x, y-Size) - down);

	float center = (lu + rd)/2.0f;

	Var10[iter++] =abs(m_data->GetHeight(x, y) - center);

	center = (ld + ru)/2.0f;

	Var10[iter++] =abs(m_data->GetHeight(x, y) - center);

	if (level > 1)assert(10 == iter);

	else assert(6 == iter);
	
	float max = Var10[0];
	for (int i = 1; i < 10; ++i)
	{
		if (max < Var10[i]) max = Var10[i];
	}

	assert(max >= 0.0f);
	m_pVariant[y*m_data->m_Size+x] = max;

	return max;
}
void CLOD_Quad::Init(CTerrain * T,MyCamera* C)
{
	m_data = T;

	m_Cam = C;

	m_Bit.Create(T->m_Size,T->m_Size);

	safe_delete_array(m_pVariant);


	unsigned int input = T->m_Size;
	register int All_Level;
	_asm bsr eax, input
	_asm mov All_Level, eax

	m_nLevel = All_Level;

	int size = 1<<(All_Level-1);

	m_pVariant = new float[T->m_Size*T->m_Size];

	m_RenderTable.reserve(T->m_SectionCountSq*T->m_SectionCountSq);


	InitVariant(All_Level, size, size);

	float cellX =  m_data->GetPos(1,0).x - m_data->GetPos(0,0).x;

	float cellY =  m_data->GetPos(0,1).z - m_data->GetPos(0,0).z;

	cellX>cellY ? m_cellSize = cellX : m_cellSize = cellY;



	m_data->m_D3D->CreateIndexBuffer(
		sizeof(WORD)*IB_SIZE,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_pIB,
		0);


	
}
void CLOD_Quad::Build(const LODmath::Frustum& F)
{

	m_Fru = &F;

	m_data->m_D3D->SetIndices(m_pIB);
	
	//通过2个队列来交替当前级别的节点
	static vector<NODE> 	next_Nodes((1<<(m_nLevel-1)));	
	static vector<NODE>		cur_Nodes((1<<(m_nLevel-1)));	

	cur_Nodes.clear();
	next_Nodes.clear();

	//根节点
	NODE root(1<<(m_nLevel-1),1<<(m_nLevel-1));

	cur_Nodes.push_back(root);

	static NODE child;

	m_BuildTrianlges = 0;


	//遍历层次级别，从根节点开始
	//并遍历每一个级别的节点
	for (unsigned int i=m_nLevel; i>0; --i)
	{
		m_cur_Level = i;

		for (size_t j=0; j<cur_Nodes.size(); ++j)
		{
			if (!NodeIsVisible(cur_Nodes[j]))
			{
				DividNode(cur_Nodes[j]);
				continue;
			}
			if (NodeCanDivid(cur_Nodes[j]))
			{
				DividNode(cur_Nodes[j]);

				int d = 1<<(i-2);

				child.x = cur_Nodes[j].x + d;
				child.y = cur_Nodes[j].y + d;
				next_Nodes.push_back(child);

				child.x = cur_Nodes[j].x - d;
				child.y = cur_Nodes[j].y + d;
				next_Nodes.push_back(child);

				child.x = cur_Nodes[j].x + d;
				child.y = cur_Nodes[j].y - d;
				next_Nodes.push_back(child);

				child.x = cur_Nodes[j].x - d;
				child.y = cur_Nodes[j].y - d;
				next_Nodes.push_back(child);

			}
			else
			{
				DisableNode(cur_Nodes[j]);
				AttachNode(cur_Nodes[j]);
				
			}

		}

		cur_Nodes.swap(next_Nodes);
		next_Nodes.clear();
	}


}
void CLOD_Quad::AttachNode(const NODE & node)
{
	if (m_cur_Level > CTerrain::e_SectionLevel) return;


	unsigned int IDX = node.x/(CTerrain::e_SectionSize-1);

	unsigned int IDY = node.y/(CTerrain::e_SectionSize-1);

	unsigned int ID = IDY*m_data->m_SectionCountSq + IDX;

	int	 Size = 1<<(m_cur_Level-1);

	assert ( m_cur_Level-1 >= 0 );
	assert (Size <= (CTerrain::e_SectionSize>>1));
	assert (ID < m_data->m_SectionCountSq*m_data->m_SectionCountSq );

	int TileSize = CTerrain::e_SectionSize;

	int cellX = node.x%(TileSize-1);

	int cellY = node.y%(TileSize-1);

	static int ibOffset = 0;

	static WORD* P = 0;

	if ((ibOffset+IB_BATCH_SIZE) >= IB_SIZE) ibOffset = 0;

	m_pIB->Lock(
		ibOffset*sizeof(WORD),
		IB_BATCH_SIZE*sizeof(WORD),
		(void**)&P,
		ibOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

	int i = 0;

	P[i++] = cellX		+ cellY*TileSize;				//中点

	P[i++] = cellX-Size	+ (cellY+Size)*TileSize;		//左上

	if (m_AdjctAct[AD_UP])
	{
		P[i++] = cellX	+ (cellY+Size)*TileSize;		//中上
	}

	P[i++] = cellX+Size	+ (cellY+Size)*TileSize;		//右上

	if (m_AdjctAct[AD_RIGHT])
	{
		P[i++] = cellX+Size	+ (cellY)*TileSize;				//中右
	}

	P[i++] = cellX+Size	+ (cellY-Size)*TileSize;		//右下


	if (m_AdjctAct[AD_DOWN])
	{
		P[i++] = cellX + (cellY-Size)*TileSize;		//中下
	}

	P[i++] = cellX-Size	+ (cellY-Size)*TileSize;		//左下


	if (m_AdjctAct[AD_LEFT])
	{
		P[i++] = cellX-Size	+ (cellY)*TileSize;				//中左
	}

	P[i++] = cellX-Size	+ (cellY+Size)*TileSize;		//左上


	m_pIB->Unlock();

	assert(i<=10);

	m_data->SetVSConstantTable(ID);

	m_data->m_D3D->SetStreamSource(
		1,
		m_data->m_pSections[ID].m_pVB, 
		0,
		sizeof(CTerrain::SVertex));

	
	m_data->m_D3D->DrawIndexedPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		0,
		TileSize*TileSize,
		ibOffset,
		i-2
		);

	m_BuildTrianlges += (i-2);

	ibOffset += IB_BATCH_SIZE;


}
bool CLOD_Quad::NodeIsVisible(const NODE & node)
{
	assert(node.x >= 0 && node.x < (int)m_data->m_Size);

	assert(node.y >= 0 && node.y < (int)m_data->m_Size);

	unsigned int curSize = (1<<(m_cur_Level-1));

	float R = curSize*m_cellSize*2.6f;

	return m_Fru->TestSphere(m_data->GetPos(node.x,node.y),R);
}
bool CLOD_Quad::NodeCanDivid(const NODE & node)
{

	assert(node.x >= 0 && node.x < (int)m_data->m_Size);

	assert(node.y >= 0 && node.y < (int)m_data->m_Size);

	memset(m_AdjctAct,1,sizeof(m_AdjctAct));

	D3DXVECTOR3 vecL = m_data->GetPos(node.x,node.y) - D3DXVECTOR3(m_Cam->GetPosition().x, m_Cam->GetPosition().y, m_Cam->GetPosition().z);

	float l = vecL.x*vecL.x + vecL.y*vecL.y + vecL.z*vecL.z;

	unsigned int Size = 1<<m_cur_Level;

	float realSize = Size * m_cellSize;

	float Var = m_pVariant[node.x+m_data->m_Size*node.y]; 

	bool Divid = m_cur_Level > 1;

	if ( m_cur_Level <=CTerrain::e_SectionLevel &&
		 realSize*Var/l < m_factor				 )	 Divid = false;
		

	int Trans = node.y+Size;
	if (Trans < (int)m_data->m_Size && !m_Bit.IsTrue(node.x,Trans))
	{
		m_AdjctAct[AD_UP] = 0;

		Divid = false;
	}

	Trans = node.y-Size;
	if (Trans >= 0 && !m_Bit.IsTrue(node.x,Trans))
	{
		m_AdjctAct[AD_DOWN] = 0;

		Divid = false;
	}

	Trans = node.x-Size;
	if (Trans >= 0 && !m_Bit.IsTrue(Trans,node.y))
	{
		m_AdjctAct[AD_LEFT] = 0;

		Divid = false;
	}

	Trans = node.x+Size;
	if (Trans < (int)m_data->m_Size && !m_Bit.IsTrue(Trans,node.y))
	{
		m_AdjctAct[AD_RIGHT] = 0;

		Divid = false;
	}

	return Divid;
}
void CLOD_Quad::DisableNode(const NODE & node)
{
	m_Bit.Set(node.x,node.y,true);

	if (m_cur_Level < 2 ) return;

	unsigned int Size = 1<<(m_cur_Level-2);

	m_Bit.Set(node.x+Size,node.y+Size,false);
	m_Bit.Set(node.x-Size,node.y-Size,false);
	m_Bit.Set(node.x+Size,node.y-Size,false);
	m_Bit.Set(node.x-Size,node.y+Size,false);


}
void CLOD_Quad::DividNode(const NODE & node)
{

	m_Bit.Set(node.x,node.y,true);

	if (m_cur_Level < 2 ) return;

	unsigned int Size = 1<<(m_cur_Level-2);

	m_Bit.Set(node.x+Size,node.y+Size,true);
	m_Bit.Set(node.x-Size,node.y-Size,true);
	m_Bit.Set(node.x+Size,node.y-Size,true);
	m_Bit.Set(node.x-Size,node.y+Size,true);

}
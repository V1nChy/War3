#pragma once
#include <vector>
#include "LODmath.h"
#include "camera.h"
#include "LODTerrain.h"

using namespace std;
using namespace LODmath;

#define IB_BATCH_SIZE 10
#define IB_SIZE (IB_BATCH_SIZE*1000)

class CLOD
{

public:

	virtual void Build(const LODmath::Frustum& F) = 0;			//计算地形层次细节

	virtual void Init(CTerrain * T, MyCamera* C) = 0;		//初始化地形层次细节

	virtual int BuildTrianlges()		= 0;			//地形实际渲染三角形片元总数

	virtual void SetFactor(float f)		= 0;			//地形粗糙因子

	virtual ~CLOD(){}


};


//基于4叉树的LOD
class CLOD_Quad: public CLOD
{
private:
	struct NODE
	{
		int x,y;

		NODE(int _x, int _y):x(_x),y(_y){}; 
		NODE(){};
	};
	enum {AD_LEFT = 0,AD_RIGHT,AD_UP,AD_DOWN};

	unsigned int				m_nLevel;			//等级(分辨率)
	unsigned int				m_cur_Level;		//当前等级
	unsigned int				m_AdjctAct[4];		//相邻节点是否活跃	 
	CBit						m_Bit;				//标记节点
	float						m_factor;			//粗糙因子
	float*						m_pVariant;			//形变
	CTerrain*					m_data;				//地形数据
	const LODmath::Frustum*		m_Fru;				//平截头体
	MyCamera*					m_Cam;				//摄像机
	float						m_cellSize;			//单位距离
	vector<int>					m_RenderTable;		//地形渲染表
	IDirect3DIndexBuffer9*		m_pIB;				//节点索引缓存（扇形）
	int							m_BuildTrianlges;	//三角形总数目



	//私有成员方法...内部使用
	float InitVariant(int level,int x, int y);		//初始化节点误差值
	void AttachNode(const NODE & node);				//挂上节点
	bool NodeIsVisible(const NODE & node);			//节点可见
	bool NodeCanDivid(const NODE & node);			//节点能分割
	void DisableNode(const NODE & node);			//使此节点无效
	void DividNode(const NODE & node);				//拆分此节点
	


public:
	CLOD_Quad();
	~CLOD_Quad();

	void Init(CTerrain * T,MyCamera* C);		//初始化地形层次细节

	void Build(const LODmath::Frustum& F);			//计算地形的LOD

	int BuildTrianlges(){return m_BuildTrianlges;}

	void SetFactor(float f){m_factor = f;}

	
};




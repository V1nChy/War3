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

	virtual void Build(const LODmath::Frustum& F) = 0;			//������β��ϸ��

	virtual void Init(CTerrain * T, MyCamera* C) = 0;		//��ʼ�����β��ϸ��

	virtual int BuildTrianlges()		= 0;			//����ʵ����Ⱦ������ƬԪ����

	virtual void SetFactor(float f)		= 0;			//���δֲ�����

	virtual ~CLOD(){}


};


//����4������LOD
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

	unsigned int				m_nLevel;			//�ȼ�(�ֱ���)
	unsigned int				m_cur_Level;		//��ǰ�ȼ�
	unsigned int				m_AdjctAct[4];		//���ڽڵ��Ƿ��Ծ	 
	CBit						m_Bit;				//��ǽڵ�
	float						m_factor;			//�ֲ�����
	float*						m_pVariant;			//�α�
	CTerrain*					m_data;				//��������
	const LODmath::Frustum*		m_Fru;				//ƽ��ͷ��
	MyCamera*					m_Cam;				//�����
	float						m_cellSize;			//��λ����
	vector<int>					m_RenderTable;		//������Ⱦ��
	IDirect3DIndexBuffer9*		m_pIB;				//�ڵ��������棨���Σ�
	int							m_BuildTrianlges;	//����������Ŀ



	//˽�г�Ա����...�ڲ�ʹ��
	float InitVariant(int level,int x, int y);		//��ʼ���ڵ����ֵ
	void AttachNode(const NODE & node);				//���Ͻڵ�
	bool NodeIsVisible(const NODE & node);			//�ڵ�ɼ�
	bool NodeCanDivid(const NODE & node);			//�ڵ��ָܷ�
	void DisableNode(const NODE & node);			//ʹ�˽ڵ���Ч
	void DividNode(const NODE & node);				//��ִ˽ڵ�
	


public:
	CLOD_Quad();
	~CLOD_Quad();

	void Init(CTerrain * T,MyCamera* C);		//��ʼ�����β��ϸ��

	void Build(const LODmath::Frustum& F);			//������ε�LOD

	int BuildTrianlges(){return m_BuildTrianlges;}

	void SetFactor(float f){m_factor = f;}

	
};




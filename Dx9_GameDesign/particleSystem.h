#pragma once

#include "d3dUtility.h"
#include "camera.h"
#include <list>
using namespace std;

//���ӵ���Ⱦ���ԣ���Ⱦ����ʱʹ�ã�
class Particle
{
public:
		D3DXVECTOR3 _position;
		D3DCOLOR    _color;
		static const DWORD FVF;
};
	
//���ӵ��������ԣ����������ټ���������ʱ��ʹ�ã�
class Attribute
{
public:
	Attribute()
	{
		_lifeTime = 0.0f;
		_age      = 0.0f;
		_isAlive  = true;
	}

	D3DXVECTOR3 _position;   //λ��  
	D3DXVECTOR3 _velocity;   //�ٶ�
	D3DXVECTOR3 _acceleration; //���ٶ�
	float       _lifeTime;     //����������֮ǰ��������  
	float       _age;          //���ӵĵ�ǰ����
	D3DXCOLOR   _color;        //���ӵ���ɫ
	D3DXCOLOR   _colorFade;    //��������ʱ����ɫ
	bool        _isAlive;      //�Ƿ���
};


/*
����ϵͳ�࣬������¡���ʾ��ɱ������������
*/
class ParticleSystem
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	//��ʼ���������������㻺��������������
	virtual bool init(IDirect3DDevice9* device, char* texFileName);
	//����ϵͳ��ÿ�����ӵ�����
	virtual void reset();
	
	// sometimes we don't want to free the memory of a dead particle,
	// but rather respawn it instead.
	virtual void resetParticle(Attribute* attribute) = 0;//����һ�����ӵ����ԣ��ú������Ϊ���麯������ʵ������������
	//���һ�����ӵ�ϵͳ�С�
	virtual void addParticle();

	virtual void update(float timeDelta) = 0;//����ϵͳ�е��������ӣ���ʵ�������ھ�������

	virtual void preRender();//���ó�ʼ��Ⱦ״̬������������Ⱦ֮ǰ������
	virtual void render();//��ʾϵͳ�е���������
	virtual void postRender();//�ָ�ĳЩ��Ⱦ״̬

	bool isEmpty();//�Ƿ�û������
	bool isDead();//�Ƿ��������Ӷ�������

	inline list<Attribute>* GetListMap()
	{
		return &_particles;
	}

protected:
	virtual void removeDeadParticles();//�Ƴ����������ӣ���������޸ģ���Щ��Ҫ�����ã�

protected:
	IDirect3DDevice9*       _device;
	D3DXVECTOR3             _origin;
	BoundingBox        _boundingBox;
	float                   _emitRate;   // rate new particles are added to system
	float                   _size;       // size of particles
	IDirect3DTexture9*      _tex;
	IDirect3DVertexBuffer9* _vb;
	std::list<Attribute>    _particles;
	int                     _maxParticles; // max allowed particles system can have

	//
	// Following three data elements used for rendering the p-system efficiently
	//

	DWORD _vbSize;      // size of vb
	DWORD _vbOffset;    // offset in vb to lock   
	DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


class Snow : public ParticleSystem
{
	Camera* _camera;
	D3DXVECTOR3 m_wordPos;
public:
	Snow(BoundingBox* boundingBox, int numParticles, Camera* camera);
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);
	void render();
};

class Firework : public ParticleSystem
{
public:
	Firework(D3DXVECTOR3* origin, int numParticles);//origin��ʾ��ը��
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);
	void preRender();
	void postRender();
};

class ParticleGun : public ParticleSystem
{
public:
	D3DXVECTOR3 m_begin, m_end;
	ParticleGun();
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);
	void render();
};
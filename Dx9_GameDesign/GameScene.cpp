#include "GameScene.h"
#include "DxGraphicLayer.h"
#include "MyTextureManager.h"
#include "MyEngineFrame.h"
#include "MyGUIButtonBase.h"
#include "MyMouse.h"
#include "MyApp.h"

HRESULT GameScene::OneTimeSceneInit()
{
	MyGUIContainer* pCtr;
	MyGUIImage* pImage;
	MyGUIButton* pBtn;
	MyGUILabel* pLab;
	MyGUICheckBox* pCheckB;

	//创建一个UI管理对象，并设为当前编辑对象
	m_GUI = new MyGUIManager();
	MyGUIManager::SetCurrentGUIManager(m_GUI);

	//新建一个容器，为背景UI容器
	pCtr = m_GUI->CreateContainer("plane1");

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetPosition(200, EngineFrame::GetSingleton()->GetWindowedHeight() - 140);
	pImage->ResetImage("Texture/plane1.png");
	pImage->SetSize(850, 140);

	MyGUIImage* minimap = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	minimap->SetPosition(EngineFrame::GetSingleton()->GetWindowedWidth()- 250, EngineFrame::GetSingleton()->GetWindowedHeight()-250);
	minimap->ResetImage("Texture/minimap.png");
	minimap->SetSize(250, 250);

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetName("Avatar");
	pImage->SetPosition(0, 200);
	pImage->ResetImage("Texture/Avatar.png");
	pImage->SetSize(48, 48);
	pImage->SetParent(minimap);

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetName("hp");
	pImage->SetPosition(485, 819);
	pImage->ResetImage("Texture/hp.png");
	pImage->SetSize(359, 13);

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetName("mp");
	pImage->SetPosition(485, 837);
	pImage->ResetImage("Texture/mp.png");
	pImage->SetSize(359, 13);

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetName("win");
	pImage->SetPosition(485, 250);
	pImage->ResetImage("Texture/win.png");
	pImage->SetSize(600, 240);
	pImage->SetVisible(false);

	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetName("fail");
	pImage->SetPosition(485, 250);
	pImage->ResetImage("Texture/fail.png");
	pImage->SetSize(600, 240);
	pImage->SetVisible(false);

	m_GUI->OneTimeInit();

	detalTime = 0;
	return S_OK;
}
HRESULT GameScene::LoadContent()
{
	MyApp* pApp = (MyApp*)EngineFrame::GetSingleton();
	pApp->RenderLoading();

	m_bOver = false;

	m_pDevice = DxGraphicLayer::GetSingleton()->GetD3DDevice();
	////初始化照相机///////////////////////////////////////////
	m_pCamera = new Camera(Camera::LANDOBJECT);
	m_pCamera->InitCamera(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 98.7f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pCamera->setupProjection(m_pDevice, D3DX_PI * 0.5f, // 90 - degree
		(float)DxGraphicLayer::GetSingleton()->GetScreenWidth() / (float)DxGraphicLayer::GetSingleton()->GetScreenHeight(),
		1.0f,
		1000.0f);
	m_pCamera->setPosition(new D3DXVECTOR3(0,100, 550));//550
	////载入天空盒//////////////////////////////////////////////////////////////////
	m_pSkyBox = new CSkyBox(m_pDevice);
	m_pSkyBox->InitSkyBox(10);
	m_pSkyBox->SetTexture("Res\\posx.jpg", 0);
	m_pSkyBox->SetTexture("Res\\negx.jpg", 1);
	m_pSkyBox->SetTexture("Res\\posz.jpg", 4);
	m_pSkyBox->SetTexture("Res\\negz.jpg", 5);
	m_pSkyBox->SetTexture("Res\\negy.jpg", 3);
	m_pSkyBox->SetTexture("Res\\posy.jpg", 2);
	////载入地形///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_pTerrain = new Terrain(m_pDevice, "Res\\map.raw", 256, 256, 10, 0.5f);
	//_TheTerrain->genTexture(&lightDirection);
	//m_pTerrain->genTexture(new D3DXVECTOR3(0, 1, 1));
	//m_pTerrain->loadTexture("Res\\floor.jpg");
	//m_pTerrain->lightTerrain(new D3DXVECTOR3(0,1,0));

	m_MovePlane = new Model(m_pDevice, L"media\\plane.obj");

	/*m_Build = new Model(m_pDevice, L"media\\terrible.obj");
	m_RedCastle = new Model(m_pDevice, L"media\\red_castle.obj");
	m_RedCastle->SetOffset(D3DXVECTOR3(-10.0f,0.0f,70.0f));
	m_RedCastle->SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_RedCastle->m_sphere.CreateMesh(0.6, D3DXVECTOR3(-20, -30, -30));
	m_RedCastle->m_sphere.isRender = false;

	m_BlueCastle = new Model(m_pDevice, L"media\\blue_castle.obj");
	m_BlueCastle->SetOffset(D3DXVECTOR3(-5.0f, 0.0f, -30.0f));
	m_BlueCastle->SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	m_BlueCastle->m_sphere.CreateMesh(0.6, D3DXVECTOR3(-10, -30, 50));
	m_RedCastle->m_sphere.isRender = false;

	ParticleGun* gun = new ParticleGun();
	gun->m_begin = D3DXVECTOR3(38, 70, 255);
	gun->m_end= D3DXVECTOR3(38, 70, 255);
	gun->init(m_pDevice, "Texture\\boom.tga");
	m_RedTower = new Tower(m_pDevice, L"media\\red_tower.obj", gun);
	m_RedTower->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 175.0f));
	m_RedTower->SetScale(D3DXVECTOR3(0.6f, 0.6f, 0.6f));

	gun = new ParticleGun();
	gun->m_begin = D3DXVECTOR3(30, 62, -178);
	gun->m_end = D3DXVECTOR3(30, 62, -178);
	gun->init(m_pDevice, "Texture\\boom.tga");
	m_BlueTower = new Tower(m_pDevice, L"media\\blue_tower.obj", gun);
	m_BlueTower->SetOffset(D3DXVECTOR3(0.0f, 0.0f, -110.0f));
	m_BlueTower->SetScale(D3DXVECTOR3(0.6f, 0.6f, 0.6f));*/

	m_DrawTriangle.Create(m_pDevice);
	// setup light
	D3DLIGHT9 light;
	D3DXVECTOR3 dir(0.0f, -1.0f, 1.0f);
	D3DXCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
	light = InitDirectionalLight(&dir, &color);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir);
	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, TRUE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00505050);
	pApp->RenderLoading();
	////角色加载//////////////////////////////////////////////////////////////////////////////
	//全部动画名称：Stand_1;Stand_2;Attack_1;Attack_2;Stand_Channel;Spell;Dissipate;
	//Walk;Stand_Victory;Stand_Ready;Stand_Hit;Death
	m_pRole = new Sprite(m_pDevice, "Res\\Arthas.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -70, 3000);
	m_pRole->m_sphere.CreateMesh(0.7, D3DXVECTOR3(-1, -42, -5));	//创建并调整包围球位置
	m_pRole->m_sphere._ID = 0;
	m_pRole->SetPosition(POINT{ 25,525 });
	m_pRole->m_fMaxMoveStep = 0.2;
	//m_pRole->m_sphere.isRender = false;	//取消显示包围盒
	pApp->RenderLoading();
	/*---------------------------------------------------------------------*/
	//Stand_1 ,Stand_2 ,Walk ,Attack_1 ,Attack_2 ,Death 
	//m_EnemyManager = new SpriteManager(m_pRole, m_pDevice);
	//m_EnemyManager->target = m_RedCastle;
	//m_EnemyManager->AddPrefab();
	//Sprite* sprite;
	//sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	//sprite->SetPosition(POINT{ 70,-200 });
	//sprite->SetRotateAngle(-PI/2);
	//sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	//sprite->m_sphere.isRender = false;	//取消显示包围盒
	//m_pEnemy->m_sphere._ID = 1;		//包围球ID
	//m_pEnemy->m_fMaxMoveStep = 0.2;	//移动速度
	//sprite->SetView(100);			//发现敌人范围
	//sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	//m_EnemyManager->Add(sprite);
	//pApp->RenderLoading();
	//sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	//sprite->SetPosition(POINT{ 70,-250 });
	//sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	//sprite->SetRotateAngle(-PI / 2);
	//sprite->SetView(100);
	//sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	//sprite->m_sphere.isRender = false;
	//m_EnemyManager->Add(sprite);
	//pApp->RenderLoading();
	//sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	//sprite->SetPosition(POINT{ 0,-250 });
	//sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	//sprite->SetRotateAngle(-PI / 2);
	//sprite->SetView(100);
	//sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	//sprite->m_sphere.isRender = false;
	//m_EnemyManager->Add(sprite);
	//pApp->RenderLoading();
	//sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	//sprite->SetPosition(POINT{ 0,-200 });
	//sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	//sprite->SetRotateAngle(-PI / 2);
	//sprite->SetView(100);
	//sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	//sprite->m_sphere.isRender = false;
	//m_EnemyManager->Add(sprite);
	//pApp->RenderLoading();



	BoundingBox box;
	box._max = D3DXVECTOR3(300, 200, 600);
	box._min = D3DXVECTOR3(-200, 0, -600);
	m_Snow = new Snow(&box, 2000, m_pCamera);
	m_Snow->init(m_pDevice, "Texture\\snow.tga");

	return S_OK;
}
void GameScene::UnloadContent()
{
	SAFE_DELETE(m_GUI);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSkyBox);

	//SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_MovePlane);

	m_DrawTriangle.Release();

	//SAFE_DELETE(m_EnemyManager);
}
void GameScene::Update(float dt)
{
	/*if (m_bOver)
		return;
	if (m_EnemyManager->isAllDeath())
	{
		m_bOver = true;
		m_GUI->GetContainer("plane1")->GetElement("win")->SetVisible(true);
	}
	else if (m_pRole->m_iNowHp < 0 || m_RedCastle->m_iNowHp<0)
	{
		m_bOver = true;
		m_GUI->GetContainer("plane1")->GetElement("fail")->SetVisible(true);
	}*/

	m_Snow->update(dt);
	/*m_RedTower->Update(dt);
	m_RedTower->AttackTest(m_EnemyManager->GetContent());
	m_BlueTower->AttackTest(m_pRole->GetVecPos());
	m_BlueTower->Update(dt);*/
	//Stand_1;Stand_2;Attack_1;Attack_2;Stand_Channel;Spell;Dissipate;Walk;Stand_Victory;Stand_Ready
	//Stand_Hit;Death
	if (::GetAsyncKeyState('Q') & 0x8000f)
		m_pRole->SetAction(A_ATTACK);
	if (::GetAsyncKeyState('W') & 0x8000f)
		m_pRole->SetAction(A_ATTACK2);
	if (::GetAsyncKeyState('E') & 0x8000f)
		m_pRole->SetAction(A_SPELL);
	if (::GetAsyncKeyState('R') & 0x8000f)
		m_pRole->SetAction(A_SPELL2);

	if (::GetAsyncKeyState('1') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_Hit", 1);
	if (::GetAsyncKeyState('2') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_2", 1);
	if (::GetAsyncKeyState('3') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_Ready", 1);

	if (::GetAsyncKeyState('6') & 0x8000f)
		m_pRole->SetAnimationSet("Death", 1);
	if (::GetAsyncKeyState('7') & 0x8000f)
		m_pRole->SetAnimationSet("Dissipate", 1);
	if (::GetAsyncKeyState('8') & 0x8000f)
		m_pRole->SetAnimationSet("Walk", 1);
	if (::GetAsyncKeyState('9') & 0x8000f)
		m_pRole->SetAnimationSet("Spell", 1);
	if (::GetAsyncKeyState('X') & 0x8000f)
		m_pRole->SetAction(A_SPELL);
	if (::GetAsyncKeyState('C') & 0x8000f)
		m_pRole->SetAction(A_STOP);

	detalTime = dt;
	POINT p;
	p.x = MyMouse::GetSingleton()->GetPos().x;
	p.y = MyMouse::GetSingleton()->GetPos().y;
	
	D3DXVECTOR3 origCamPos;
	m_pCamera->getPosition(&origCamPos);
	if (p.y <= 10 || ::GetAsyncKeyState(VK_UP) & 0x8000f)
	{
		m_pCamera->walk(100.0f * dt);
		
	}
	if (p.y >= (ScreenHeight - 10) || ::GetAsyncKeyState(VK_DOWN) & 0x8000f)
	{
		m_pCamera->walk(-100.0f * dt);
		
	}
	if (p.x <= 10 || ::GetAsyncKeyState(VK_LEFT) & 0x8000f)
	{
		m_pCamera->strafe(-100.0f * dt);
		
	}
	if (p.x >= (ScreenWidth - 10) || ::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
	{
		m_pCamera->strafe(100.0f * dt);
		
	}
	if (::GetAsyncKeyState('A') & 0x8000f)
		m_pCamera->yaw(-1.0f * dt);
	if (::GetAsyncKeyState('D') & 0x8000f)
		m_pCamera->yaw(1.0f * dt);
	if (::GetAsyncKeyState('F') & 0x8000f)
		m_pCamera->pitch(1.0f * dt);
	if (::GetAsyncKeyState('S') & 0x8000f)
		m_pCamera->pitch(-1.0f * dt);

	//更新照相机位置
	D3DXVECTOR3 camPos, dir;
	D3DXVECTOR3* pos;
	m_pCamera->getPosition(&camPos);
	dir = D3DXVECTOR3(0, -1, 0);
	pos = RayIntersectFace(camPos, dir);
	if (pos)
	{
		float h = pos->y + 60.0f;
		camPos.y = h;
		m_pCamera->setPosition(&camPos);
		m_pCamera->UpdateViewMatrix(m_pDevice);

		m_pRole->SetHeight(h-142);
	}
	else
	{
		m_pCamera->setPosition(&origCamPos);
		m_pCamera->UpdateViewMatrix(m_pDevice);
	}

	if((pos = MousePick()))
		m_pRole->SetEndPoint(*pos);

	m_pRole->UpDate();
	POINT rolePos = m_pRole->GetPosition();
	m_GUI->GetContainer("plane1")->GetElement("Avatar")->SetPosition((-rolePos.y + 560) % 1000 / 1000.0f*200.0f, 200 - (-rolePos.y+560)%1000/1000.0f*200.0f);
	m_GUI->GetContainer("plane1")->GetElement("hp")->SetSize(1.0f*m_pRole->m_iNowHp / m_pRole->m_iMaxHp * 359,13);
	//m_pEnemy->UpDate();

	//m_EnemyManager->Update(dt);
}

bool g_IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
	D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
	FLOAT* t, FLOAT* u, FLOAT* v)
{
	// Find vectors for two edges sharing vert0
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	D3DXVECTOR3 tvec;
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return FALSE;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)
		return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return TRUE;
}

D3DXVECTOR3* GameScene::RayIntersectFace(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir)
{

	IDirect3DVertexBuffer9* pVB = NULL;
	IDirect3DIndexBuffer9* pIB = NULL;

	m_MovePlane->GetMesh()->GetVertexBuffer(&pVB);
	m_MovePlane->GetMesh()->GetIndexBuffer(&pIB);

	DWORD*      pIndices;
	VERTEX*    pVertices;

	pIB->Lock(0, 0, (void**)&pIndices, 0);
	pVB->Lock(0, 0, (void**)&pVertices, 0);

	INTERSECTION Intersection;
	Intersection.fDist = 1000.0f;
	Intersection.bPick = false;

	BOOL bHit;
	
	DWORD dwFace, dwNumIntersections;
	FLOAT fBary1, fBary2, fDist;
	

	LPD3DXBUFFER pBuffer = NULL;
	D3DXINTERSECTINFO* pIntersectInfoArray;
	D3DXIntersect(m_MovePlane->GetMesh(), &orig, &dir, &bHit, NULL, NULL, NULL, NULL, &pBuffer, &dwNumIntersections);
	if (bHit && dwNumIntersections>0)
	{
		pIntersectInfoArray = (D3DXINTERSECTINFO*)pBuffer->GetBufferPointer();

		if (dwNumIntersections > MAX_INTERSECTIONS)
			dwNumIntersections = MAX_INTERSECTIONS;

		DWORD nearest;
		for (DWORD iIntersection = 0; iIntersection < dwNumIntersections; iIntersection++)
		{
			if (!Intersection.bPick || pIntersectInfoArray[iIntersection].Dist < Intersection.fDist)
			{
					Intersection.dwFace = pIntersectInfoArray[iIntersection].FaceIndex;
					Intersection.fBary1 = pIntersectInfoArray[iIntersection].U;
					Intersection.fBary2 = pIntersectInfoArray[iIntersection].V;
					Intersection.fDist = pIntersectInfoArray[iIntersection].Dist;

					Intersection.bPick = true;
			}
		}

		if (Intersection.bPick)
		{
			DWORD* iThisTri;

			iThisTri = &pIndices[3 * Intersection.dwFace];
			// get vertices hit
			Intersection.v[0] = pVertices[iThisTri[0]].position;
			Intersection.v[1] = pVertices[iThisTri[1]].position;
			Intersection.v[2] = pVertices[iThisTri[2]].position;

		}
	}

	//D3DXIntersect(m_Model->GetMesh(), &orig, &dir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, NULL, NULL);
	//if (bHit)
	//{
	//	dwNumIntersections = 1;

	//	Intersection.dwFace = dwFace;
	//	Intersection.fBary1 = fBary1;
	//	Intersection.fBary2 = fBary2;
	//	Intersection.fDist = fDist;


	//	DWORD* iThisTri;

	//	iThisTri = &pIndices[3*Intersection.dwFace];
	//	// get vertices hit
	//	Intersection.v[0] = pVertices[iThisTri[0]].position;
	//	Intersection.v[1] = pVertices[iThisTri[1]].position;
	//	Intersection.v[2] = pVertices[iThisTri[2]].position;

	//	Intersection.bPick = true;
	//}
	//else
	//{
	//	dwNumIntersections = 0;
	//}

	//DWORD dwNumFaces = m_Model->GetMesh()->GetNumFaces();
	//for (DWORD i = 0; i < dwNumFaces; i++)
	//{
	//	DWORD i1 = pIndices[3 * i + 0];
	//	DWORD i2 = pIndices[3 * i + 1];
	//	DWORD i3 = pIndices[3 * i + 2];

	//	D3DXVECTOR3 v0 = pVertices[pIndices[3 * i + 0]].position;
	//	D3DXVECTOR3 v1 = pVertices[pIndices[3 * i + 1]].position;
	//	D3DXVECTOR3 v2 = pVertices[pIndices[3 * i + 2]].position;

	//	// Check if the Pick ray passes through this point
	//	if (g_IntersectTriangle(orig, dir, v0, v1, v2,
	//		&fDist, &fBary1, &fBary2))
	//	{
	//		if (!Intersection.bPick || fDist < Intersection.fDist)
	//		{
	//			Intersection.dwFace = i;
	//			Intersection.fBary1 = fBary1;
	//			Intersection.fBary2 = fBary2;
	//			Intersection.fDist = fDist;

	//			Intersection.v[0] = v0;
	//			Intersection.v[1] = v1;
	//			Intersection.v[2] = v2;

	//			Intersection.bPick = true;
	//		}
	//	}
	//}


	pVB->Unlock();
	pIB->Unlock();

	
	D3DXVECTOR3* IntersectPos = NULL;;
	if (Intersection.bPick)
	{
		D3DXVECTOR3 det, pos;
		det = Intersection.v[1] - Intersection.v[0];
		det *= Intersection.fBary1;
		pos = Intersection.v[0] + det;

		det = Intersection.v[2] - pos;
		det *= Intersection.fBary2;
		pos = pos + det;

		IntersectPos = &pos;

		m_DrawTriangle.Updata(Intersection.v);

		char temp[150];

		/*sprintf(temp, "Dis:%f\n%d", dis, isCollision);
		m_DrawText.SetText("Test", temp);*/

		sprintf(temp, "x:%.2f,y:%.2f,z:%.2f",
			pos.x, pos.y, pos.z);
		text = temp;
	}

	return IntersectPos;
}

D3DXVECTOR3* GameScene::MousePick()
{
	if (!MyMouse::GetSingleton()->IsMouseKeyDown(1, true))
		return NULL;

	int x, y;

	x = MyMouse::GetSingleton()->GetPos().x;
	y = MyMouse::GetSingleton()->GetPos().y;

	//视域体
	D3DVIEWPORT9 vp;
	m_pDevice->GetViewport(&vp);

	//投影矩阵
	D3DXMATRIX proj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	//投影窗口坐标
	D3DXVECTOR3 v;
	v.x = (((2.0f * x) / vp.Width) - 1) / proj(0, 0);
	v.y = -(((2.0f * y) / vp.Height) - 1) / proj(1, 1);
	v.z = 1.0f;

	sRay ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = v;

	//得到逆矩阵
	D3DXMATRIX view;
	//pDevice_->GetTransform(D3DTS_VIEW, &view);
	view = m_pCamera->GetViewMatrix();

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&ray._origin,
		&ray._origin,
		&viewInverse);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray._direction,
		&ray._direction,
		&viewInverse);

	// normalize the direction
	D3DXVec3Normalize(&ray._direction, &ray._direction);

	//m_DrawRay.Updata(ray);
	/*if(m_pRole->m_sphere.RaySphereIntTest(&ray))
	MessageBox(0, "Hit!", "HIT", 0);*/
	/*--------------------------------------------------------------------------------*/

	return RayIntersectFace(ray._origin, ray._direction);
}

void GameScene::Render()
{

	D3DXVECTOR3 pos;
	m_pCamera->getPosition(&pos);
	m_pSkyBox->Render(pos);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (m_pTerrain)
		//m_pTerrain->draw(&I, true);

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_Build->Render(m_pCamera, detalTime);
	//m_RedCastle->Render(m_pCamera, detalTime);
	//m_BlueCastle->Render(m_pCamera, detalTime);
	//m_RedTower->Render(m_pCamera, detalTime);
	//m_BlueTower->Render(m_pCamera, detalTime);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_DrawTriangle.Draw();

	m_Snow->render();

	m_pRole->Render(0.015f);
	//m_EnemyManager->Render();
	//m_pEnemy->Render(0.015f);
	
	MyFontManager::GetSingleton()->m_pNormalFont->DrawText(text, MyColor::White, 0, 30);
}

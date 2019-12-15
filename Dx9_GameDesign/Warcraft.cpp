#include"Warcraft.h"

WarcraftDemo::WarcraftDemo() :Dx9DemoBase(),
m_pTerrain(NULL),
m_pCamera(NULL),
m_pSkyBox(NULL),
m_BillBoard(NULL),
m_GUI(NULL),
m_FontMg(NULL)
{
	m_pInstance = this;
}

WarcraftDemo:: ~WarcraftDemo()
{
}

WarcraftDemo* WarcraftDemo::GetObject()
{
	if (!m_pInstance)
		m_pInstance = new WarcraftDemo();
	return (WarcraftDemo*)m_pInstance;
}

LRESULT CALLBACK WarcraftDemo::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:

		//拾取计算
		Pick(LOWORD(lParam), HIWORD(lParam));

		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
void TestButton()
{
	MyGUILabel* pEle = (MyGUILabel*)MyGUIManager::GetCurrentGUIManager()->GetContainer("Test")->GetElement("label1");
	pEle->SetVisible(false);
}
bool WarcraftDemo::LoadContent()
{
	m_FontMg = MyFontManager::GetSingleton();
	m_FontMg->OneTimeSceneInit();

	m_GUI = new MyGUIManager();
	MyGUIManager::SetCurrentGUIManager(m_GUI);
	MyGUIContainer* pCtr = m_GUI->CreateContainer("Test");
	MyGUILabel* pEle = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pEle->SetName("label1");
	pEle->SetPosition(100,100);
	//pEle->SetSize(100,100);
	pEle->SetFormat(AF_VMD_CTR);
	//pEle->SetFont(3);
	//pEle->SetTextColor(MyColor::White);
	//pEle->SetAlpha(0.5);
	pEle->SetText("LabelText");
	pEle->SetShowLevel(1);

	MyGUIImage* pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->SetPosition(100,100);
	pImage->SetSize(200,50);
	//pImage->RotateInDegree(90);
	

	MyGUIButton* pBtn= (MyGUIButton*)pCtr->CreateElement(GUI_Button);
	pBtn->SetPosition(200,200);
	pBtn->SetImage("Texture/Button_NLPD.tga",110,160, 110, 40);
	pBtn->BindFunction(TestButton);


	m_GUI->OneTimeInit();
	////载入地形///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pTerrain = new Terrain(m_pDevice, "Res\\map.raw", 256, 256, 10, 0.5f);
	//_TheTerrain->genTexture(&lightDirection);
	m_pTerrain->loadTexture("Res\\floor.jpg");
	// Set texture filters.
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	////初始化照相机///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pCamera = new Camera(Camera::LANDOBJECT);
	m_pCamera->InitCamera(D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, -1.0f, 1.0f),D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pCamera->setupProjection(m_pDevice, D3DX_PI * 0.5f, // 90 - degree
		(float)ScreenWidth / (float)ScreenWidth,
		1.0f,
		1000.0f);
	////载入天空盒//////////////////////////////////////////////////////////////////
	m_pSkyBox = new CSkyBox(m_pDevice);
	m_pSkyBox->InitSkyBox(10);
	m_pSkyBox->SetTexture("Res\\sky_RT.jpg",0);
	m_pSkyBox->SetTexture("Res\\sky_LF.jpg", 1);
	m_pSkyBox->SetTexture("Res\\sky_FR.jpg", 4);
	m_pSkyBox->SetTexture("Res\\sky_BK.jpg", 5);
	m_pSkyBox->SetTexture("Res\\sky_DN.jpg", 3);
	m_pSkyBox->SetTexture("Res\\sky_UP.jpg", 2);
	////雨（RainParticle）粒子系统//////////////////////////////////////////////////////////////////
	D3DXVECTOR3 light_dir(0.0f, -1.0f, 1.0f);
	/*BoundingBox box;
	float size = 200;
	box._max = D3DXVECTOR3(size, size, size);
	box._min = D3DXVECTOR3(-size, -size, -size);
	rain = new RainParticle(m_pDevice, &box,500, light_dir);

	m_Rain = new Rain(&box, 1000);
	m_Rain->init(m_pDevice, "Res\\rainline.dds");*/

	////UI/////////////////////////////////////////////////////////////////////////////////
	m_BillBoard = new BillBoard();
	m_BillBoard->Init(m_pDevice);
	m_BillBoard->LoadTexture(m_pDevice, UIT01, "Res\\UITile01.BMP");
	m_BillBoard->LoadTexture(m_pDevice, UIT02, "Res\\UITile02.BMP");
	m_BillBoard->LoadTexture(m_pDevice, UIT03, "Res\\UITile03.BMP");
	m_BillBoard->LoadTexture(m_pDevice, UIT04, "Res\\UITile04.BMP");
	m_BillBoard->LoadTexture(m_pDevice, MINIMAP, "Res\\Minimap.bmp");
	m_BillBoard->LoadTexture(m_pDevice, BUTTONROLE, "Res\\ButtomRole.bmp");
	m_BillBoard->LoadTexture(m_pDevice, BUTTONCENTER, "Res\\ButtomCenter.jpg");

	m_BillBoard->LoadTexture(m_pDevice, BTNATTACK, "Res\\BTNAttack.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNHOLDPOSITION, "Res\\BTNHoldPosition.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNMOVE, "Res\\BTNMove.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNNONE, "Res\\BTNNone.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNPATROL, "Res\\BTNPatrol.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNSKILL1, "Res\\BTNResurrection.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNSKILLZ, "Res\\BTNSkillz.jpg");
	m_BillBoard->LoadTexture(m_pDevice, BTNSTOP, "Res\\BTNStop.jpg");

	////角色加载//////////////////////////////////////////////////////////////////////////////
	//全部动画名称：Stand_1;Stand_2;Attack_1;Attack_2;Stand_Channel;Spell;Dissipate;
	//Walk;Stand_Victory;Stand_Ready;Stand_Hit;Death
	m_pRole = new Sprite(m_pDevice, "Res\\Arthas.X", D3DXVECTOR3(0.1, 0.1, 0.1), "Stand_1", 1, -70, 10);
	m_pRole->m_sphere.CreateMesh(0.7,D3DXVECTOR3(-1,-62,-5));	//创建并调整包围球位置
	m_pRole->m_sphere._ID = 0;
	//m_pRole->m_sphere.isRender = false;	//取消显示包围盒

	//Stand_1 ,Stand_2 ,Walk ,Attack_1 ,Attack_2 ,Death 
	m_pEnt = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.1, 0.1, 0.1), "Stand_1", 1, -70, 10);
	m_pEnt->m_sphere.CreateMesh(4, D3DXVECTOR3(42, -92, 23));	//创建并调整包围球位置
	//m_pEnt->m_sphere.isRender = false;	//取消显示包围盒
	m_pEnt->SetPosition(POINT{ 100,100 });
	m_pEnt->m_sphere._ID = 1;		//包围球ID
	m_pEnt->m_fMaxMoveStep = 0.2;	//移动速度
	m_pEnt->SetView(200);			//发现敌人范围

	//walk ,kill,stop
	//m_pQD = new Sprite(pDevice_, "Res\\QD.X", D3DXVECTOR3(10, 10, 10), "walk", 1, 0, 10);
	//m_pQD->m_SkinMesh->SetTransHeight(20);

	// setup light
	D3DLIGHT9 light;
	D3DXVECTOR3 dir(0.0f, -1.0f, 1.0f);
	D3DXCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
	light = InitDirectionalLight(&light_dir, &color);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir);
	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, TRUE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00505050);

	//拾取射线，三角形渲染初始化
	m_DrawRay.Create(m_pDevice);
	m_DrawTriangle.Create(m_pDevice);
	//文本输出初始化
	m_DrawText.Init(m_pDevice);
	sTextMsg* temp1 = new sTextMsg;
	temp1->_rect = RECT{ 0,0,300,30 };
	m_DrawText.AddText("Mouse", temp1);

	sTextMsg* temp2 = new sTextMsg;
	temp2->_rect = RECT{ ScreenWidth - 100,0,ScreenWidth,100 };
	m_DrawText.AddText("FPS", temp2);

	sTextMsg* temp3 = new sTextMsg;
	temp3->_rect = RECT{ 0,30,500,150 };
	m_DrawText.AddText("Pick", temp3);

	sTextMsg* temp4 = new sTextMsg;
	temp4->_rect = RECT{ 0,200,500,300 };
	m_DrawText.AddText("Test", temp4);

	//m_Sound.InitDSound(hwnd_);
	//m_Sound.Set_dwMaxBytesStatic(1024000*25);
	//m_Sound.CreateStaticBuffer(0, "Res\\NightElf.wav");
	//m_Sound.PlayStatic(0, true,true);
	return true;
}

void WarcraftDemo::UnloadContent()
{
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSkyBox);
	SAFE_DELETE(m_BillBoard);
	SAFE_DELETE(m_pRole);
	SAFE_DELETE(m_pEnt);


	SAFE_DELETE(m_FontMg);
	SAFE_DELETE(m_GUI);
	m_DrawRay.Release();
	m_DrawTriangle.Release();

	MyMouse::GetSingleton()->FreeDevice();
	Angle::GetSingleton()->FinalCleanUp();
}

void WarcraftDemo::Update(float dt)
{
	m_GUI->FrameMove(dt);

	//Stand_1;Stand_2;Attack_1;Attack_2;Stand_Channel;Spell;Dissipate;Walk;Stand_Victory;Stand_Ready
	//Stand_Hit;Death
	if (::GetAsyncKeyState('1') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_Hit", 1);
	if (::GetAsyncKeyState('2') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_2", 1);
	if (::GetAsyncKeyState('3') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_Ready", 1);
	if (::GetAsyncKeyState('4') & 0x8000f)
		m_pRole->SetAnimationSet("Attack_2", 1);
	if (::GetAsyncKeyState('5') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_Channel", 1);
	if (::GetAsyncKeyState('6') & 0x8000f)
		m_pRole->SetAnimationSet("Death", 1);
	if (::GetAsyncKeyState('7') & 0x8000f)
		m_pRole->SetAnimationSet("Dissipate", 1);
	if (::GetAsyncKeyState('8') & 0x8000f)
		m_pRole->SetAnimationSet("Walk", 1);
	if (::GetAsyncKeyState('9') & 0x8000f)
		m_pRole->SetAnimationSet("Spell", 1);
	if (::GetAsyncKeyState('0') & 0x8000f)
		m_pRole->SetAnimationSet("Stand_Victory", 1);
	if (::GetAsyncKeyState('Z') & 0x8000f)
		m_pRole->SetAction(A_ATTACK);
	if (::GetAsyncKeyState('X') & 0x8000f)
		m_pRole->SetAction(A_SPELL);
	if (::GetAsyncKeyState('C') & 0x8000f)
		m_pRole->SetAction(A_STOP);

	POINT p;
	GetCursorPos(&p);
	HCURSOR hcurR = (HCURSOR)LoadImage(NULL, "Res\\Orc Hand.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);

	if (p.y <= 10 || ::GetAsyncKeyState(VK_UP) & 0x8000f)
	{
		m_pCamera->walk(100.0f * dt);
		hcurR = (HCURSOR)LoadImage(NULL, "Res\\3D Vertical.ani", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	}
	if (p.y >= (ScreenHeight - 10) || ::GetAsyncKeyState(VK_DOWN) & 0x8000f)
	{
		m_pCamera->walk(-100.0f * dt);
		hcurR = (HCURSOR)LoadImage(NULL, "Res\\3D Vertical.ani", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	}
	if (p.x <= 10 || ::GetAsyncKeyState(VK_LEFT) & 0x8000f)
	{
		m_pCamera->strafe(-100.0f * dt);
		hcurR = (HCURSOR)LoadImage(NULL, "Res\\3D Horizontal.ani", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	}
	if (p.x >= (ScreenWidth - 10) || ::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
	{
		m_pCamera->strafe(100.0f * dt);
		hcurR = (HCURSOR)LoadImage(NULL, "Res\\3D Horizontal.ani", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	}
	if (::GetAsyncKeyState('A') & 0x8000f)
		m_pCamera->yaw(-1.0f * dt);
	if (::GetAsyncKeyState('D') & 0x8000f)
		m_pCamera->yaw(1.0f * dt);
	if (::GetAsyncKeyState('W') & 0x8000f)
		m_pCamera->pitch(1.0f * dt);
	if (::GetAsyncKeyState('S') & 0x8000f)
		m_pCamera->pitch(-1.0f * dt);

	SetClassLong(m_hwnd, GCL_HCURSOR, (long)hcurR);

	//更新照相机位置
	D3DXVECTOR3 pos;
	m_pCamera->getPosition(&pos);
	float height = m_pTerrain->getHeight(pos.x, pos.z);
	pos.y = height + 30.0f; // add height because we're standing up
	m_pCamera->setPosition(&pos);
	m_pCamera->UpdateViewMatrix(m_pDevice);

	m_pRole->UpDate();
	m_pEnt->UpDate();
	//m_pQD->UpDate();
	//m_Rain->update(dt);

	//更新主角包围球位置，及主角高度
	POINT rolePos;
	rolePos = m_pRole->GetPosition();
	pos.x = rolePos.x;
	pos.z = rolePos.y;
	float h = m_pTerrain->getHeight(rolePos.x, rolePos.y);
	m_pRole->SetHeight(h-91);
	pos.y = h;
	m_pRole->m_sphere._worldPos = pos;

	//检测主角位置是否在攻击范围内
	if(m_pEnt->FieldOfView(rolePos))
		m_pEnt->SetEndPoint(pos);
	POINT enemyPos;
	enemyPos = m_pEnt->GetPosition();
	pos.x = enemyPos.x;
	pos.z = enemyPos.y;
	h = m_pTerrain->getHeight(enemyPos.x, enemyPos.y);
	m_pEnt->SetHeight(h - 85);
	pos.y = h;
	m_pEnt->m_sphere._worldPos = pos;

	//自动攻击，检测包围球碰撞，碰撞则攻击
	if (m_pRole->m_sphere.CollisionShere(m_pEnt->m_sphere))
	{
		m_pRole->SetAction(A_ATTACK);
		m_pRole->SetRotateAngle(pos);
	}
	if (m_pEnt->m_sphere.CollisionShere(m_pRole->m_sphere))
		m_pEnt->SetAction(A_ATTACK);
	////更新文本内容//////////////////////////////////////////////////////////////////
	char temp[50];

	/*sprintf(temp, "Dis:%f\n%d", dis, isCollision);
	m_DrawText.SetText("Test", temp);*/

	sprintf(temp, "MousePos:%d,%d", p.x, p.y);
	m_DrawText.SetText("Mouse", temp);

	static int FrameCnt = 0;
	static float TimeElapsed = 0.0;
	float FPS;
	FrameCnt++;

	TimeElapsed += dt;

	if (TimeElapsed >= 1.0f)
	{
		FPS = (float)FrameCnt / TimeElapsed;

		sprintf(temp, "%.2f", FPS);

		TimeElapsed -= 1.0f;
		FrameCnt = 0;

		m_DrawText.SetText("FPS", temp);
	}
}

void WarcraftDemo::RenderUI()
{
	////UITile01///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(0.948, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(0, 0));
	m_BillBoard->Render(UIT01, RECT{ 0,0,512,100 });

	m_BillBoard->SetSize(&D3DXVECTOR2(0.948, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(0, ScreenHeight - 320));
	m_BillBoard->Render(UIT01, RECT{ 0,170,512,512 });
	////UITile02///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(1, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(485, 0));
	m_BillBoard->Render(UIT02, RECT{ 0,0,512,100 });

	m_BillBoard->SetSize(&D3DXVECTOR2(1, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(485, ScreenHeight - 320));
	m_BillBoard->Render(UIT02, RECT{ 0,170,512,512 });
	////UITile03///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(1, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(997, 0));
	m_BillBoard->Render(UIT03, RECT{ 0,0,512,100 });

	m_BillBoard->SetSize(&D3DXVECTOR2(0.945, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(997, ScreenHeight - 320));
	m_BillBoard->Render(UIT03, RECT{ 0,170,512,512 });
	////UITile04///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(1, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1475, 0));
	m_BillBoard->Render(UIT04, RECT{ 0,0,64,100 });

	m_BillBoard->SetSize(&D3DXVECTOR2(1, 0.948));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1475, ScreenHeight - 320));
	m_BillBoard->Render(UIT04, RECT{ 0,170,64,512 });

	////////MINIMAP///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(1.04, 1.025));
	m_BillBoard->SetPosition(&D3DXVECTOR2(17, ScreenHeight - 270));
	m_BillBoard->Render(MINIMAP, RECT{ 0,0,256,256 });

	////////BUTTONROLE///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(0.6, 0.9));
	m_BillBoard->SetPosition(&D3DXVECTOR2(405, ScreenHeight - 230));
	m_BillBoard->Render(BUTTONROLE, RECT{ 0,0,256,256 });

	////////BUTTONCENTER///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_BillBoard->SetSize(&D3DXVECTOR2(0.81, 0.9));
	m_BillBoard->SetPosition(&D3DXVECTOR2(570, ScreenHeight - 230));
	m_BillBoard->Render(BUTTONCENTER, RECT{ 0,0,512,256 });

	////////BUTTON-skill///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//11
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1195, ScreenHeight - 248));
	m_BillBoard->Render(BTNMOVE, RECT{ 0,0,64,64 });
	//12
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1277, ScreenHeight - 248));
	m_BillBoard->Render(BTNSTOP, RECT{ 0,0,64,64 });
	//13
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1359, ScreenHeight - 248));
	m_BillBoard->Render(BTNHOLDPOSITION, RECT{ 0,0,64,64 });
	//14
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1441, ScreenHeight - 248));
	m_BillBoard->Render(BTNATTACK, RECT{ 0,0,64,64 });
	//21
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1195, ScreenHeight - 165));
	m_BillBoard->Render(BTNPATROL, RECT{ 0,0,64,64 });
	//22
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1277, ScreenHeight - 165));
	m_BillBoard->Render(BTNNONE, RECT{ 0,0,64,64 });
	//23
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1359, ScreenHeight - 165));
	m_BillBoard->Render(BTNNONE, RECT{ 0,0,64,64 });
	//24
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1441, ScreenHeight - 165));
	m_BillBoard->Render(BTNSKILLZ, RECT{ 0,0,64,64 });
	//31
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1195, ScreenHeight - 83));
	m_BillBoard->Render(BTNSKILL1, RECT{ 0,0,64,64 });
	//32
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1277, ScreenHeight - 83));
	m_BillBoard->Render(BTNNONE, RECT{ 0,0,64,64 });
	//33
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1359, ScreenHeight - 83));
	m_BillBoard->Render(BTNNONE, RECT{ 0,0,64,64 });
	//34
	m_BillBoard->SetSize(&D3DXVECTOR2(1.17, 1.17));
	m_BillBoard->SetPosition(&D3DXVECTOR2(1441, ScreenHeight - 83));
	m_BillBoard->Render(BTNNONE, RECT{ 0,0,64,64 });
}

void WarcraftDemo::Render()
{
	//用指定颜色清除后备缓冲区
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//Direct3D规定在渲染前必须调用BeginScene，结束时调用EndScene
	m_pDevice->BeginScene();


	D3DXVECTOR3 pos;
	m_pCamera->getPosition(&pos);
	m_pSkyBox->Render(pos);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	m_pDevice->SetTransform(D3DTS_WORLD, &I);


	m_pRole->Render(0.02f);
	m_pEnt->Render(0.025f);

	//m_pQD->Render(0.025f);

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, pos.x-100, pos.y, pos.z-100);
	
	/*D3DXVECTOR3 right;
	m_pCamera->getRight(&right);
	D3DXMATRIX rotate;
	float ang = atan(right.x / right.z)*180/3.14159f + 90;
	D3DXMatrixRotationY(&rotate, ang);
	D3DXMatrixMultiply(&world, &rotate, &world);*/

	/*char temp[50];
	sprintf(temp, "PickMsg:%f",ang);*/

	/*m_DrawText.SetText("Pick", temp);*/

	//rain->Render(world);
	//m_Rain->render();

	RenderUI();

	m_DrawRay.Draw();
	m_DrawTriangle.Draw();
	m_DrawText.Draw();

	m_GUI->RenderForwardGUI();

	m_pDevice->EndScene();


	//交换当前/后备缓冲区，刷新窗口
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}


bool IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
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
void WarcraftDemo::Pick(int x, int y)
{
	//视域体
	D3DVIEWPORT9 vp;
	m_pDevice->GetViewport(&vp);

	//投影矩阵
	D3DXMATRIX proj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	//投影窗口坐标
	D3DXVECTOR3 v;
	v.x = (((2.0f * x) / vp.Width) - 1) / proj(0,0);
	v.y = -(((2.0f * y) / vp.Height) - 1) / proj(1,1);
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

	m_DrawRay.Updata(ray);
	/*if(m_pRole->m_sphere.RaySphereIntTest(&ray))
		MessageBox(0, "Hit!", "HIT", 0);*/
	/*--------------------------------------------------------------------------------*/
	INTERSECTION g_IntersectionArray = { false,0,0,0,0,0,0 }; //拾取信息

	memset(&g_IntersectionArray, 0, sizeof(INTERSECTION));
	g_IntersectionArray.fDist = 1000.0f;
	g_IntersectionArray.bPick = false;

	IDirect3DVertexBuffer9* pVB=NULL;
	IDirect3DIndexBuffer9* pIB=NULL;

	pVB = m_pTerrain->getVertexBuffer();
	pIB = m_pTerrain->getIndexBuffer();

	WORD* pIndices;
	TerrainVertex* pVertices;

	pIB->Lock(0, 0, (void**)&pIndices, 0);
	pVB->Lock(0, 0, (void**)&pVertices, 0);

	DWORD dwNumFaces = m_pTerrain->_numTriangles;
	FLOAT fBary1, fBary2;
	FLOAT fDist;
	for (DWORD i = 0; i < dwNumFaces; i++)
	{
		D3DXVECTOR3 v0 = D3DXVECTOR3(pVertices[pIndices[3 * i + 0]]._x, pVertices[pIndices[3 * i + 0]]._y, pVertices[pIndices[3 * i + 0]]._z);
		D3DXVECTOR3 v1 = D3DXVECTOR3(pVertices[pIndices[3 * i + 1]]._x, pVertices[pIndices[3 * i + 1]]._y, pVertices[pIndices[3 * i + 1]]._z);
		D3DXVECTOR3 v2 = D3DXVECTOR3(pVertices[pIndices[3 * i + 2]]._x, pVertices[pIndices[3 * i + 2]]._y, pVertices[pIndices[3 * i + 2]]._z);

		// Check if the Pick ray passes through this point
		if (IntersectTriangle(ray._origin, ray._direction, v0, v1, v2,
			&fDist, &fBary1, &fBary2))
		{
			if (!g_IntersectionArray.bPick || fDist < g_IntersectionArray.fDist)
			{
				g_IntersectionArray.dwFace = i;
				g_IntersectionArray.fBary1 = fBary1;
				g_IntersectionArray.fBary2 = fBary2;
				g_IntersectionArray.fDist = fDist;

				g_IntersectionArray.v[0] = v0;
				g_IntersectionArray.v[1] = v1;
				g_IntersectionArray.v[2] = v2;

				g_IntersectionArray.bPick = true;
			}
		}
	}
	pVB->Unlock();
	pIB->Unlock();

	if (g_IntersectionArray.bPick)
	{
		D3DXVECTOR3 det, pos;
		det = g_IntersectionArray.v[1] - g_IntersectionArray.v[0];
		det *= g_IntersectionArray.fBary1;
		pos = g_IntersectionArray.v[0] + det;

		det = g_IntersectionArray.v[2] - pos;
		det *= g_IntersectionArray.fBary2;
		pos = pos + det;

		m_DrawTriangle.Updata(g_IntersectionArray.v);
		m_pRole->SetEndPoint(pos);

		char temp[100];
		sprintf(temp, "PickMsg:\n	Dis:%.2f\n	U:%.2f,V:%.2f\n	PickPos:%.2f,%.2f,%.2f",
			g_IntersectionArray.fDist, g_IntersectionArray.fBary1, g_IntersectionArray.fBary2,
			pos.x,pos.y,pos.z);

		m_DrawText.SetText("Pick", temp);
	}
}
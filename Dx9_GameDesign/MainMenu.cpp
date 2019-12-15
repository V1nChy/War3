#include "MainMenu.h"
#include "DxGraphicLayer.h"
#include "MyTextureManager.h"
#include "MyEngineFrame.h"
#include "MyGUIButtonBase.h"
#include "MyMouse.h"

HRESULT MainMenu::LoadContent()
{
	return S_OK; 
}
HRESULT MainMenu::OneTimeSceneInit()
{

	MyGUIContainer* pCtr;
	MyGUIImage* pImage;
	MyGUIButton* pBtn;
	MyGUILabel* pLab;
	MyGUICheckBox* pCheckB;

	//设置鼠标图标
	pImage = new MyGUIImage(NULL, 0, "DEFAULT", "Texture/NightElfCursor.BMP", true, MyColor::Black,
		0, 0, 30, 30, MyColor::White, true);
	pImage->SetUVMap(0, 0, 32 / 256.f, 32 / 128.f);
	pImage->SetBehaviour(new UVAni(4, 8, 32, 32, 256, 128));
	pImage->OneTimeInit();
	MyMouse::GetSingleton()->SetCursor(Normal, pImage);
	MyMouse::GetSingleton()->SetOffsetCursor(-9,-2);

	//创建一个UI管理对象，并设为当前编辑对象
	m_GUI = new MyGUIManager();
	MyGUIManager::SetCurrentGUIManager(m_GUI);

	//新建一个容器，为背景UI容器
	pCtr = m_GUI->CreateContainer("Bg",false);
	//新建一个图片
	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/bg.bmp");
	pImage->SetSize(EngineFrame::GetSingleton()->GetWindowedWidth(), EngineFrame::GetSingleton()->GetWindowedHeight());


	//新建一个容器，为前景UI容器  Option
	pCtr = m_GUI->CreateContainer("Option");
	pCtr->SetPosition(20, 0);
	//pCtr->SetBehaviour(new ContainerMoveAni(800 - 265, -350, 1));

	//新建一个图片
	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/panel1.bmp");
	//pImage->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pImage->SetSize(900, 750);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(30, 130);
	pLab->SetFont(0);
	pLab->SetText("游戏性设置");
	pLab->SetTextColor(MyColor::White);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(30, 160);
	pLab->SetText("鼠标滚动：");
	pLab->SetTextColor(MyColor::Yellow);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(75, 200);
	pLab->SetText("慢");
	pLab->SetTextColor(MyColor::White);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(620, 200);
	pLab->SetText("快");
	pLab->SetTextColor(MyColor::White);

	MyGUIScrollBarHori* pScroll = (MyGUIScrollBarHori*)pCtr->CreateElement(GUI_ScrollBarHori);
	pScroll->SetPosition(110, 195);
	
	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_MIDDLE);
	pImage->ResetImage("Texture/slot.bmp");
	pImage->SetSize(500,25);
	pImage->SetUVMap(0,0,1,1);

	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_TARGET);
	pImage->ResetImage("Texture/slider.bmp",MyColor::Pink);
	pImage->SetUVMap(0, 0, 1, 1);
	pImage->SetSize(25, 25);
	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_BEGIN);
	pImage->SetVisible(false);
	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_END);
	pImage->SetVisible(false);
	pScroll->SetSize(500, 40);

	pCheckB = (MyGUICheckBox*)pCtr->CreateElement(GUI_CheckBox);
	pCheckB->SetPosition(35,240);
	pCheckB->SetLabelText("取消鼠标滚动");
	pCheckB->SetLabelSize(250,30);
	pCheckB->SetLabelTextColor(MyColor::Yellow);
	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_NORMAL);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(0,0,1/6.f,1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_BUTTONDOWN);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(4 / 6.f, 0, 5 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_MOUSEOVER);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(2 / 6.f, 0, 3 / 6.f, 1);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(30, 280);
	pLab->SetText("键盘滚动：");
	pLab->SetTextColor(MyColor::Yellow);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(75, 320);
	pLab->SetText("慢");
	pLab->SetTextColor(MyColor::White);

	pLab = (MyGUILabel*)pCtr->CreateElement(GUI_Label);
	pLab->SetPosition(620, 320);
	pLab->SetText("快");
	pLab->SetTextColor(MyColor::White);

	pScroll = (MyGUIScrollBarHori*)pCtr->CreateElement(GUI_ScrollBarHori);
	pScroll->SetPosition(110, 315);

	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_MIDDLE);
	pImage->ResetImage("Texture/slot.bmp");
	pImage->SetSize(500, 25);
	pImage->SetUVMap(0, 0, 1, 1);

	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_TARGET);
	pImage->ResetImage("Texture/slider.bmp", MyColor::Pink);
	pImage->SetUVMap(0, 0, 1, 1);
	pImage->SetSize(25, 25);
	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_BEGIN);
	pImage->SetVisible(false);
	pImage = pScroll->GetImage(MyGUIScrollBar::BAR_END);
	pImage->SetVisible(false);
	pScroll->SetSize(500, 40);


	pCheckB = (MyGUICheckBox*)pCtr->CreateElement(GUI_CheckBox);
	pCheckB->SetPosition(35, 365);
	pCheckB->SetCheck(true);
	pCheckB->SetLabelSize(250,30);
	pCheckB->SetLabelText("高级工具提示");
	pCheckB->SetLabelTextColor(MyColor::Yellow);
	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_NORMAL);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(0, 0, 1 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_BUTTONDOWN);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(4 / 6.f, 0, 5 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_MOUSEOVER);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(2 / 6.f, 0, 3 / 6.f, 1);

	pCheckB = (MyGUICheckBox*)pCtr->CreateElement(GUI_CheckBox);
	pCheckB->SetPosition(35, 405);
	pCheckB->SetLabelSize(300, 30);
	pCheckB->SetLabelText("子组顺序修改键");
	pCheckB->SetLabelTextColor(MyColor::Yellow);
	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_NORMAL);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(0, 0, 1 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_BUTTONDOWN);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(4 / 6.f, 0, 5 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_MOUSEOVER);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(2 / 6.f, 0, 3 / 6.f, 1);

	pCheckB = (MyGUICheckBox*)pCtr->CreateElement(GUI_CheckBox);
	pCheckB->SetPosition(35, 445);
	pCheckB->SetLabelSize(300, 30);
	pCheckB->SetButtonStatus(MyGUIButtonBase::BTS_DISABLE);
	pCheckB->SetLabelText("启动队形移动");
	pCheckB->SetLabelTextColor(MyColor::Yellow);
	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_NORMAL);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(0, 0, 1 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_DISABLE);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(5 / 6.f, 0, 1, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_BUTTONDOWN);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(4 / 6.f, 0, 5 / 6.f, 1);

	pImage = pCheckB->GetImage(MyGUIButtonBase::BTS_MOUSEOVER);
	pImage->ResetImage("Texture/checkbox.bmp");
	pImage->SetUVMap(2 / 6.f, 0, 3 / 6.f, 1);
	pCtr->SetVisible(false);

	//新建一个容器，为前景UI容器  help
	pCtr = m_GUI->CreateContainer("Help");
	pCtr->SetPosition(20, 0);
	//pCtr->SetBehaviour(new ContainerMoveAni(800 - 265, -350, 1));

	//新建一个图片
	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/panel1.bmp");
	//pImage->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pImage->SetSize(900, 750);
	pCtr->SetVisible(false);

	/*------------------------------------------------------------------*/
	//新建一个容器，为前景UI容器Select
	pCtr = m_GUI->CreateContainer("Select");
	pCtr->SetPosition(1100, 600);
	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/panel3.bmp");
	pImage->SetSize(410, 190);

	pBtn = (MyGUIButton*)pCtr->CreateElement(GUI_Button);
	pBtn->SetBehaviour(new ClickButton());
	//pBtn->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pBtn->SetPosition(84, 38);
	pBtn->SetLabelText("确 定");
	pBtn->SetLabelTextColor(MyColor::Yellow);
	pBtn->SetImage("Texture/btn.bmp", 1, 1, 0.25, 1, MyColor::Pink);
	pBtn->SetSize(295, 45);

	pBtn = (MyGUIButton*)pCtr->CreateElement(GUI_Button);
	pBtn->SetBehaviour(new ClickButtonQuit());
	//pBtn->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pBtn->SetPosition(84, 110);
	pBtn->SetLabelText("取 消");
	pBtn->SetLabelTextColor(MyColor::Yellow);
	pBtn->SetImage("Texture/btn.bmp", 1, 1, 0.25, 1, MyColor::Pink);
	pBtn->SetSize(295, 45);

	//新建一个容器，为前景UI容器Menu
	pCtr = m_GUI->CreateContainer("Menu");
	pCtr->SetPosition(1065, 0);
	pCtr->SetBehaviour(new ContainerMoveAni(1065, -500, 0.6));

	//新建一个图片
	pImage = (MyGUIImage*)pCtr->CreateElement(GUI_Image);
	pImage->ResetImage("Texture/panel2.bmp");
	//pImage->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pImage->SetSize(450, 500);

	pBtn = (MyGUIButton*)pCtr->CreateElement(GUI_Button);
	pBtn->SetBehaviour(new ClickButtonChangeScene());
	//pBtn->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pBtn->SetPosition(92, 203);
	pBtn->SetLabelText("新 战 役");
	pBtn->SetLabelTextColor(MyColor::Yellow);
	pBtn->SetImage("Texture/btn.bmp", 1, 1, 0.25, 1, MyColor::Pink);
	pBtn->SetSize(325, 50);
	//pBtn->BindFunction(TestButton);

	pBtn = (MyGUIButton*)pCtr->CreateElement(GUI_Button);
	pBtn->SetName("SelectBtn");
	ClickButtonShow1* pBehaviour = (ClickButtonShow1*)pBtn->SetBehaviour(new ClickButtonShow1(20,-800,20,0,0.6));
	pBehaviour->pCont = m_GUI->GetContainer("Option");

	//pBtn->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pBtn->SetPosition(92, 297);
	pBtn->SetLabelText("游 戏 性");
	pBtn->SetLabelTextColor(MyColor::Yellow);
	pBtn->SetSize(325, 50);
	pBtn->SetImage("Texture/btn.bmp", 1, 1, 0.25, 1, MyColor::Pink);
	//pBtn->BindFunction(TestButton);

	pBtn = (MyGUIButton*)pCtr->CreateElement(GUI_Button);
	pBtn->SetName("HelpBtn");
	ClickButtonShow2* pBehaviour2 = (ClickButtonShow2*)pBtn->SetBehaviour(new ClickButtonShow2(20, -800, 20, 0, 0.6));
	pBehaviour2->pCont = m_GUI->GetContainer("Help");

	//pBtn->SetLocation(sLocation(GetSystemMetrics(SM_CXSCREEN) / 800.0f, GetSystemMetrics(SM_CYSCREEN) / 600.0f));
	pBtn->SetPosition(92, 392);
	pBtn->SetLabelText("帮 助");
	pBtn->SetLabelTextColor(MyColor::Yellow);
	pBtn->SetSize(325, 50);
	pBtn->SetImage("Texture/btn.bmp", 1, 1, 0.25, 1, MyColor::Pink);
	//pBtn->BindFunction(TestButton);

	m_GUI->OneTimeInit();

	return S_OK;
}
void MainMenu::UnloadContent()
{
	SAFE_DELETE(m_GUI);
}
void MainMenu::Update(float dt)
{}
void MainMenu::Render()
{}
﻿#include <BxScene.hpp>
#include <BxPanel.hpp>
#include <BxImage.hpp>

namespace BxCore
{
	namespace System
	{
		bool& ForSystem_LetEnablePanelDebug();
	}
}

class BxSimulator
{
public:
	enum {MENU_HUB, MENU_GRID, MENU_LOG, MENU_START, MENU_SNAP, MENU_LENGTH};
	enum {BTN_EXIT, BTN_MAX, BTN_MIN, BTN_LENGTH};
	enum {TOUCH_0, TOUCH_1, TOUCH_LENGTH};
	enum {GUI_MENUWIDTH = 112};

	point NCWindowPos;
	point NCClickPos;
	id_font GUIFont;
	id_font GUIFontMini;
	BxImage TitleImage;

	bool IsMenuVisible;
	int MenuPos;
	bool IsMenuOn[MENU_LENGTH];
	string MenuName[MENU_LENGTH];
	bool IsMenuDown[MENU_LENGTH];
	BxImage MenuImage[MENU_LENGTH][2];
	BxImage TextImage[MENU_LENGTH];

	bool IsFullScreenMode;
	string BtnName[BTN_LENGTH];
	bool IsBtnDown[BTN_LENGTH];
	BxImage BtnImage[BTN_LENGTH][3];

	point TouchPos[2];
	BxImage TouchImage[TOUCH_LENGTH];

	point CursorPos;
	int AvgFrameTime;
	int AvgFrameRate;
	BxString::Parse StartPage;

public:
	BxSimulator() : NCWindowPos(point::zero()), NCClickPos(point::zero()), GUIFont(nullptr)
	{
		if(!BxCore::Font::IsExistNick("nanum_gothic_extra_bold"))
			BxCore::Font::NickOpen("nanum_gothic_extra_bold", "sys/nanum_gothic_extra_bold.ttf");
		if(!BxCore::Font::IsExistNick("huy152"))
			BxCore::Font::NickOpen("huy152", "sys/huy152.ttf");
        GUIFont = BxCore::Font::Open("nanum_gothic_extra_bold", 22);
        GUIFontMini = BxCore::Font::Open("huy152", 15);

		TitleImage.Load("sys/title.png", BxImage::PNG, BxImage::PAD5);

		IsMenuVisible = false;
		MenuPos = 0;
		IsMenuOn[MENU_HUB] = false;
		IsMenuOn[MENU_GRID] = false;
		IsMenuOn[MENU_LOG] = false;
		IsMenuOn[MENU_START] = false;
		IsMenuOn[MENU_SNAP] = false;
		MenuName[MENU_HUB] = "BxSimulator:Hub";
		MenuName[MENU_GRID] = "BxSimulator:Grid";
		MenuName[MENU_LOG] = "BxSimulator:Log";
		MenuName[MENU_START] = "BxSimulator:Start";
		MenuName[MENU_SNAP] = "BxSimulator:Snap";
		IsMenuDown[MENU_HUB] = false;
		IsMenuDown[MENU_GRID] = false;
		IsMenuDown[MENU_LOG] = false;
		IsMenuDown[MENU_START] = false;
		IsMenuDown[MENU_SNAP] = false;
		MenuImage[MENU_HUB][0].Load("sys/icon_hub.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_HUB][1].Load("sys/icon_hub_d.png", BxImage::PNG, BxImage::PAD8);
        MenuImage[MENU_GRID][0].Load("sys/icon_grid.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_GRID][1].Load("sys/icon_grid_d.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_LOG][0].Load("sys/icon_log.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_LOG][1].Load("sys/icon_log_d.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_START][0].Load("sys/icon_start.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_START][1].Load("sys/icon_start_d.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_SNAP][0].Load("sys/icon_snap.png", BxImage::PNG, BxImage::PAD8);
		MenuImage[MENU_SNAP][1].Load("sys/icon_snap_d.png", BxImage::PNG, BxImage::PAD8);
		TextImage[MENU_HUB].Load("sys/text_hub.png", BxImage::PNG, BxImage::PAD5);
		TextImage[MENU_GRID].Load("sys/text_grid.png", BxImage::PNG, BxImage::PAD5);
		TextImage[MENU_LOG].Load("sys/text_log.png", BxImage::PNG, BxImage::PAD5);
		TextImage[MENU_START].Load("sys/text_start.png", BxImage::PNG, BxImage::PAD5);
		TextImage[MENU_SNAP].Load("sys/text_snap.png", BxImage::PNG, BxImage::PAD5);

		IsFullScreenMode = false;
		BtnName[BTN_EXIT] = "BxSimulator:Exit";
		BtnName[BTN_MAX] = "BxSimulator:Max";
		BtnName[BTN_MIN] = "BxSimulator:Min";
		IsBtnDown[BTN_EXIT] = false;
		IsBtnDown[BTN_MAX] = false;
		IsBtnDown[BTN_MIN] = false;
		BtnImage[BTN_EXIT][0].Load("sys/exit_base.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_EXIT][1].Load("sys/exit_focus.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_EXIT][2].Load("sys/exit_push.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_MAX][0].Load("sys/max_base.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_MAX][1].Load("sys/max_focus.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_MAX][2].Load("sys/max_push.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_MIN][0].Load("sys/min_base.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_MIN][1].Load("sys/min_focus.png", BxImage::PNG, BxImage::PAD7);
		BtnImage[BTN_MIN][2].Load("sys/min_push.png", BxImage::PNG, BxImage::PAD7);

		TouchPos[TOUCH_0] = point::zero();
		TouchPos[TOUCH_1] = point::zero();
		TouchImage[TOUCH_0].Load("sys/systouch_0.png", BxImage::PNG, BxImage::PAD5);
		TouchImage[TOUCH_1].Load("sys/systouch_1.png", BxImage::PNG, BxImage::PAD5);

		CursorPos = point::zero();
		AvgFrameTime = 0;
		AvgFrameRate = 0;
	}

	~BxSimulator()
	{
        BxCore::Font::Close(GUIFont);
        BxCore::Font::Close(GUIFontMini);
	}
};

FRAMEWORK_SCENE_HIDDEN(BxSimulator, "BxSimulator")

syseventresult OnEvent(BxSimulator& This, const sysevent& Event)
{
	if(Event.type == syseventtype_init)
		This.StartPage = Event.init.message;
	else if(Event.type == syseventtype_button)
	{
        if(StrCmp(Event.button.name, "BxSimulator:TitleBar") == same
			|| StrCmp(Event.button.name, "BxSimulator:MenuBar") == same)
		{
			if(Event.button.type == sysbuttontype_down)
			{
                This.NCWindowPos = BxCore::Simulator::GetWindowPos();
                This.NCClickPos = BxCore::Simulator::GetCursorPos();
			}
			else if(Event.button.type == sysbuttontype_drag)
			{
                const point NewClickPos = BxCore::Simulator::GetCursorPos();
                BxCore::Simulator::SetWindowPos(
					This.NCWindowPos.x + NewClickPos.x - This.NCClickPos.x,
					This.NCWindowPos.y + NewClickPos.y - This.NCClickPos.y);
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.BtnName[This.BTN_EXIT]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsBtnDown[This.BTN_EXIT] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsBtnDown[This.BTN_EXIT] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
                    BxCore::System::DoQuit();
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.BtnName[This.BTN_MAX]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsBtnDown[This.BTN_MAX] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsBtnDown[This.BTN_MAX] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
				{
					if(This.IsFullScreenMode = !This.IsFullScreenMode)
						BxCore::Simulator::DoFullScreen();
					else BxCore::Simulator::DoNormalScreen();
				}
			}
			return syseventresult_done;
		}
        else if(StrCmp(Event.button.name, This.BtnName[This.BTN_MIN]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsBtnDown[This.BTN_MIN] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsBtnDown[This.BTN_MIN] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
                    BxCore::Simulator::DoMinimize();
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.MenuName[This.MENU_HUB]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsMenuDown[This.MENU_HUB] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsMenuDown[This.MENU_HUB] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
					This.IsMenuOn[This.MENU_HUB] ^= true;
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.MenuName[This.MENU_GRID]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsMenuDown[This.MENU_GRID] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsMenuDown[This.MENU_GRID] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
				{
					This.IsMenuOn[This.MENU_GRID] ^= true;
					BxCore::System::ForSystem_LetEnablePanelDebug() = This.IsMenuOn[This.MENU_GRID];
				}
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.MenuName[This.MENU_LOG]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsMenuDown[This.MENU_LOG] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsMenuDown[This.MENU_LOG] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
					This.IsMenuOn[This.MENU_LOG] ^= true;
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.MenuName[This.MENU_START]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsMenuDown[This.MENU_START] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsMenuDown[This.MENU_START] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
				{
					if(!BxScene::IsActivity(This.StartPage))
					{
						BxScene::AddRequest(This.StartPage);
						for(int i = 0, iend = BxScene::GetCount(); i < iend; ++i)
						{
							string OldScene = BxScene::GetName((scenelayer) i);
							BxScene::SubRequest(OldScene);
						}
					}
				}
			}
			return syseventresult_done;
		}
		else if(StrCmp(Event.button.name, This.MenuName[This.MENU_SNAP]) == same)
		{
			if(Event.button.type == sysbuttontype_down)
				This.IsMenuDown[This.MENU_SNAP] = true;
			else if(Event.button.type == sysbuttontype_up)
			{
				This.IsMenuDown[This.MENU_SNAP] = false;
				if(Event.button.field.getsize().inside(Event.button.x, Event.button.y))
				{}
			}
			return syseventresult_done;
		}
	}
	else if(Event.type == syseventtype_touch)
	{
		if(Event.touch.id < 2)
			This.TouchPos[Event.touch.id] = XY(Event.touch.x, Event.touch.y);
	}
	return syseventresult_pass;
}

sysupdateresult OnUpdate(BxSimulator& This)
{
	sysupdateresult Result = sysupdateresult_no_render_fixed_frame;
    const point WindowPos = BxCore::Simulator::GetWindowPos();
    const point CursorPos = BxCore::Simulator::GetCursorPos();
	This.CursorPos.x = CursorPos.x - WindowPos.x;
	This.CursorPos.y = CursorPos.y - WindowPos.y;

	// 업데이트 속도계산
    global_data uhuge LastTime = BxCore::System::GetTimeMilliSecond();
	global_data int UpdateTime[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	global_data int UpdateTimeFocus = -1;
    const uhuge ThisTime = BxCore::System::GetTimeMilliSecond();
	UpdateTime[UpdateTimeFocus = (UpdateTimeFocus + 1) % 16] = Max(1, (int)(ThisTime - LastTime));
	LastTime = ThisTime;
	const int AvgUpdateTime = Max(1, (UpdateTime[0] + UpdateTime[1] + UpdateTime[2] + UpdateTime[3] + UpdateTime[4]
		+ UpdateTime[5] + UpdateTime[6] + UpdateTime[7] + UpdateTime[8] + UpdateTime[9] + UpdateTime[10]
		+ UpdateTime[11] + UpdateTime[12] + UpdateTime[13] + UpdateTime[14] + UpdateTime[15]) / 16);
	const int AvgFrameTime = 1000 / AvgUpdateTime;
    const int AvgFrameRate = 100 * BxCore::Main::GetCurrentFrameTime(true) / AvgUpdateTime;
	if(This.AvgFrameTime != AvgFrameTime || This.AvgFrameRate != AvgFrameRate)
	{
		Result = (sysupdateresult) (Result | sysupdateresult_do_render_fixed_frame);
		This.AvgFrameTime = AvgFrameTime;
		This.AvgFrameRate = AvgFrameRate;
	}

	// 패널강조
	if(BxCore::System::IsEnablePanelDebug())
		BxPanel::HitupDebugLine(This.CursorPos.x - BxCore::Main::GetGUIMarginL(),
			This.CursorPos.y - BxCore::Main::GetGUIMarginT());

	// 메뉴애니메이션
	if(This.IsMenuVisible)
		This.MenuPos = (This.GUI_MENUWIDTH + This.MenuPos + 1) / 2;
	else This.MenuPos = (0 + This.MenuPos) / 2;
	return Result;
}

void OnRender(BxSimulator& This, BxDraw& Draw)
{
	const int TitleHeight = BxCore::System::GetConfigNumber("Bx.Framework.GUIMargin.Top", 100);
	const int MenuBarWidth = BxCore::System::GetConfigNumber("Bx.Framework.GUIMargin.Left", 100);
	const int Gap = 5;

	// 멀티터치 표식
	if(BxCore::System::IsTouchDown() && !BxCore::System::IsTouchDown(false))
	BxTRY(Draw, CLIP(XYXY(MenuBarWidth, TitleHeight, Draw.Width(), Draw.Height())))
		for(int i = 0; i < BxSimulator::TOUCH_LENGTH; ++i)
			Draw.Area(This.TouchPos[i].x, This.TouchPos[i].y, FORM(&This.TouchImage[i]) >> OPACITY(192));

	// 타이틀
	BxTRY(Draw, CLIP(XYXY(0, 0, Draw.Width(), TitleHeight)), "BxSimulator:TitleBar")
	{
		const int TitleBottom = Draw.Height() - BxCore::Font::GetHeight(This.GUIFontMini) - 2;
		Draw.Rectangle(FILL, XYWH(0, 0), COLOR(55, 55, 55));
		// 아이콘
		Draw.Area(MenuBarWidth / 2, TitleHeight / 2, FORM(&This.TitleImage));
		// 제목
		global_data BxString::Parse SubjectName(BxCore::System::GetConfigString("Bx.Currently.Title", "No Title"));
		Draw.TextByRect(This.GUIFont, SubjectName, XYXY(Gap + MenuBarWidth, 0, Draw.Width(), TitleBottom - 3),
			textalign_left_bottom, COLOR(255, 255, 255));
		// 씬리스트
		global_data BxString::Parse SceneList;
		SceneList = "[";
		for(int i = 0, iend = BxScene::GetCount(); i < iend; ++i)
		{
			if(0 < i) SceneList += "/ ";
			SceneList += BxScene::GetName((scenelayer) i);
		}
		SceneList += "]";
		Draw.TextByRect(This.GUIFontMini, SceneList, XYXY(Gap + MenuBarWidth, TitleBottom, Draw.Width(), Draw.Height()),
			textalign_left_middle, COLOR(128, 138, 150));
		// 버튼
		int x = Draw.Width();
		for(int i = 0; i < BxSimulator::BTN_LENGTH; ++i)
		{
			BxImage& CurImage = This.BtnImage[i][0];
			x -= CurImage.Width();
			BxTRY(Draw, CLIP(XYWH(x, 0, CurImage.Width(), CurImage.Height())), This.BtnName[i])
			{
				int Index = 0;
				if(This.IsBtnDown[i]) Index = 2;
				else if(BxDrawGlobal::_DrawOption::CurClipRect().inside(This.CursorPos))
					Index = 1;
				Draw.Area(0, 0, FORM(&This.BtnImage[i][Index]));
			}
		}
		// 프레임
		global_data BxString::Parse FrameInfo;
		FrameInfo = BxCore::Util::Print("<>:<A>F/ <A>%", BxARG(This.AvgFrameTime, This.AvgFrameRate));
		Draw.TextByRect(This.GUIFontMini, FrameInfo, XYXY(0, 0, x, TitleBottom + 4),
			textalign_right_bottom, COLOR(4, 237, 244));
	}

	// 메뉴
	bool IsMenuVisible = false;
	BxTRY(Draw, CLIP(XYXY(0, TitleHeight, Draw.Width(), Draw.Height())))
	{
		// 메뉴바
		BxTRY(Draw, CLIP(XYWH(0, 0, MenuBarWidth, Draw.Height())), "BxSimulator:MenuBar")
		{
			Draw.Rectangle(FILL, XYWH(0, 0), COLOR(44, 44, 44));
			for(int i = 0, y = 0; i < BxSimulator::MENU_LENGTH; ++i)
			{
				const int Index = (This.IsMenuOn[i])? 1 : 0;
				BxImage& CurImage = This.MenuImage[i][Index];
				BxTRY(Draw, CLIP(XYWH(0, y, Draw.Width(), CurImage.Height() + Gap * 2)))
				{
					Draw.Area(Draw.Width() / 2, Gap, FORM(&CurImage));
					if(!BxCore::System::IsTouchDown())
					if(BxDrawGlobal::_DrawOption::CurClipRect().inside(This.CursorPos))
						IsMenuVisible = true;
				}
				y += CurImage.Height() + Gap * 2;
			}
		}

		// 메뉴
		BxTRY(Draw, CLIP(XYXY(MenuBarWidth, 0, Draw.Width(), Draw.Height())))
		{
			const int MenuPos = This.MenuPos - This.GUI_MENUWIDTH;
			BxTRY(Draw, CLIP(XYWH(MenuPos, 0, This.GUI_MENUWIDTH, Draw.Height())))
			{
				Draw.Rectangle(FILL, XYWH(0, 0), COLOR(242, 242, 234));
				for(int i = 0, y = 0; i < BxSimulator::MENU_LENGTH; ++i)
				{
					BxImage& CurImage = This.MenuImage[i][0];
					BxTRY(Draw, CLIP(XYWH(0, y, Draw.Width(), CurImage.Height() + Gap * 2)), This.MenuName[i])
					{
						// 포커싱
						bool IsFocus = false;
						if(BxDrawGlobal::_DrawOption::CurClipRect().inside(This.CursorPos))
						{
							IsFocus = true;
							IsMenuVisible = true;
						}
						else if(This.IsMenuDown[i])
							IsFocus = true;
						if(IsFocus)
							Draw.Rectangle(FILL, XYWH(3, 1, Draw.Width() - 6, Draw.Height() - 2 - 1),
								COLOR(255, 255, 255) >> OPACITY(224));
						// 스트링
						Draw.Area(Draw.Width() / 2, Draw.Height() / 2 + ((This.IsMenuDown[i])? 2 : 0),
							FORM(&This.TextImage[i]));
						// 구분선
						Draw.Rectangle(FILL, XYWH(3, Draw.Height() - 1, Draw.Width() - 6, 1),
							COLOR(217, 217, 210));
					}
					y += CurImage.Height() + Gap * 2;
				}
			}
			// 그림자
			if(0 < This.MenuPos)
			BxTRY(Draw, CLIP(XYWH(MenuPos + This.GUI_MENUWIDTH, 0, 5, Draw.Height())) >> COLOR(0, 0, 0))
			{
				for(int x = 0, o = 5; x < 5; ++x, --o)
					Draw.Rectangle(FILL, XYWH(x, 0, 1), OPACITY(o * o * 5));
			}
		}
	}
	This.IsMenuVisible = IsMenuVisible;
}

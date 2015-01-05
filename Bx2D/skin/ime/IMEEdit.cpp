﻿#include <BxPanel.hpp>

class IMEEdit : public BxPanel
{
	public: VariableS::Font* _ref_ RefFont;
	public: BxString DefaultText;
	public: BxString Text;
	public: BxString Tail;
	public: BxString PtrString;
	public: IMEEdit(bool isRef, void* varL) : BxPanel(isRef, varL)
	{
		RefFont = nullptr;
		PtrString = "|";
		PtrString += BxUtilGlobal::MtoH(true, (mint) this);
		PtrString += "|";
		PtrString += BxUtilGlobal::MtoH(true, (mint) &Text);
		PtrString += "|";
		PtrString += BxUtilGlobal::MtoH(true, (mint) &Tail);
		PtrString += "|";
	}
	public: virtual ~IMEEdit() {}
};
FRAMEWORK_PANEL_CLASS("fw.ime.IMEEdit", IMEEdit, XYXY)

void OnCreate(string option, unknown param, const bool first)
{
	BxVar<BxString::Parse> ParsedOption;
	BxString::Parse::Division(ParsedOption, option);

	// 폰트셋팅
	BxPanel::VariableS::Font& CurFont = s--.fnt(ParsedOption[0] + ParsedOption[1]);
	if(!CurFont.IsLoaded())
		CurFont.Load(ParsedOption[0], BxUtilGlobal::AtoI(ParsedOption[1]));

	m--.RefFont = &CurFont;
	m--.DefaultText = ParsedOption[2];
	m--.flag("IsEnable") = false;
	m--.flag("IsCursorEng") = true;
}

unknown OnNotify(string message, unknown param)
{
	if(StrCmp(message, "GetString") == same)
		return (unknown) (string) m--.Text;
	else if(StrCmp(message, "GetPtrString") == same)
		return (unknown) (string) m--.PtrString;
	else if(StrCmp(message, "ClearString") == same)
	{
		m--.Text = "";
		m--.Tail = "";
	}
	else if(StrCmp(message, "SetEnable") == same)
		m--.flag("IsEnable") = (bool) param;
	else if(StrCmp(message, "SetCursorEng") == same)
		m--.flag("IsCursorEng") = (bool) param;
	return nullptr;
}

string OnTouch(BxPanel::Touch type)
{
	return "";
}

void OnPaint(rect& r, int x1, int y1, int x2, int y2)
{
	BxDraw& Draw = m--.GetDraw();
	BxTRY(Draw, CLIP(r = XYXY(x1, y1, x2, y2)))
	{
		if(0 < m--.Text.GetLength() || 0 < m--.Tail.GetLength() || m--.flag("IsEnable"))
		{
			int XPos = Draw.Text(*m--.RefFont, m--.Text, XY(0, Draw.Height() / 2),
				textalign_left_middle, COLOR(0, 0, 0)).x;
			XPos = Draw.Text(*m--.RefFont, m--.Tail, XY(XPos, Draw.Height() / 2),
				textalign_left_middle, COLOR(128, 128, 128)).x;
			// 커서
			if(m--.flag("IsEnable"))
			{
				const uhuge Time = BxCore::System::GetTimeMilliSecond();
				id_text Text = BxCore::Font::TextOpen("|");
				const int FontWidth = BxCore::Font::GetWidth(*m--.RefFont, Text);
				const int FontHeight = BxCore::Font::GetHeight(*m--.RefFont);
				Text = BxCore::Font::TextClose(Text);
				if((Time >> 8) & 1)
					Draw.Rectangle(FILL, XYWH(XPos, (Draw.Height() - FontHeight) / 2, FontWidth, FontHeight),
						m--.flag("IsCursorEng")? COLOR(128, 160, 192) : COLOR(192, 160, 128));
			}
		}
		else Draw.Text(*m--.RefFont, m--.DefaultText, XY(0, Draw.Height() / 2),
			textalign_left_middle, COLOR(128, 128, 128));
	}
}
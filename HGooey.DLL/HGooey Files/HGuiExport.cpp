#define HGOOEY_EXPORTS
#include "HGIncDef_Private.h"

#define keyCount 96

char* DIK_KeyText[keyCount] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", 
							  "0", "A", "B", "C", "D", "E", "F", "G", "H", "I", 
							  "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", 
							  "U", "V", "W", "X", "Y", "Z", "F1", "F2", "F3", "F4", 
							  "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", 
							  "ESCAPE", "TAB", "MINUS", "EQUALS", "BACK", "ENTER", 
							  "LBRACKET", "RBRACKET", "LCONTROL", "RCONTROL", 
							  "LSHIFT", "RSHIFT", "LALT", "RALT", "SEMICOLON", 
							  "APOSTROPHE", "BACKSLASH", "COMMA", "PERIOD", 
							  "SPACE", "CAPITAL", "NUMLOCK", "SCROLL", "LEFT", 
							  "RIGHT", "UP", "DOWN", "PAUSE", "HOME", "INSERT", 
							  "DELETE", "NUMPAD1", "NUMPAD2", "NUMPAD3", "NUMPAD4", 
							  "NUMPAD5", "NUMPAD6", "NUMPAD7", "NUMPAD8", "NUMPAD9", 
							  "NUMPAD0", "NUMPADENTER", "NUMPADEQUALS", "DECIMAL", 
							  "ADD", "SUBTRACT", "MULTIPLY", "DIVIDE"};

BYTE DIK_Key[keyCount] = { DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, 
							DIK_9, DIK_0, DIK_A, DIK_B, DIK_C, DIK_D, DIK_E, 
							DIK_F, DIK_G, DIK_H, DIK_I, DIK_J, DIK_K, DIK_L, DIK_M, 
							DIK_N, DIK_O, DIK_P, DIK_Q, DIK_R, DIK_S, DIK_T, DIK_U, 
							DIK_V, DIK_W, DIK_X, DIK_Y, DIK_Z, DIK_F1, DIK_F2, 
							DIK_F3, DIK_F4, DIK_F5, DIK_F6, DIK_F7, DIK_F8, DIK_F9, 
							DIK_F10, DIK_F11, DIK_F12, DIK_ESCAPE, DIK_TAB, 
							DIK_MINUS, DIK_EQUALS, DIK_BACK, DIK_RETURN, 
							DIK_LBRACKET, DIK_RBRACKET, DIK_LCONTROL, 
							DIK_RCONTROL, DIK_LSHIFT, DIK_RSHIFT, DIK_LALT, 
							DIK_RALT, DIK_SEMICOLON, DIK_APOSTROPHE, 
							DIK_BACKSLASH, DIK_COMMA, DIK_PERIOD, DIK_SPACE, 
							DIK_CAPITAL, DIK_NUMLOCK, DIK_SCROLL, DIK_LEFT, 
							DIK_RIGHT, DIK_UP, DIK_DOWN, DIK_PAUSE, DIK_HOME, 
							DIK_INSERT, DIK_DELETE, DIK_NUMPAD1, DIK_NUMPAD2, 
							DIK_NUMPAD3, DIK_NUMPAD4, DIK_NUMPAD5, DIK_NUMPAD6, 
							DIK_NUMPAD7, DIK_NUMPAD8, DIK_NUMPAD9, DIK_NUMPAD0, 
							DIK_NUMPADENTER, DIK_NUMPADEQUALS, DIK_DECIMAL, DIK_ADD, 
							DIK_SUBTRACT, DIK_MULTIPLY, DIK_DIVIDE};

char* DIK_Name[keyCount] = { "DIK_1", "DIK_2", "DIK_3", "DIK_4", "DIK_5", "DIK_6", "DIK_7", "DIK_8", 
							"DIK_9", "DIK_0", "DIK_A", "DIK_B", "DIK_C", "DIK_D", "DIK_E", 
							"DIK_F", "DIK_G", "DIK_H", "DIK_I", "DIK_J", "DIK_K", "DIK_L", "DIK_M", 
							"DIK_N", "DIK_O", "DIK_P", "DIK_Q", "DIK_R", "DIK_S", "DIK_T", "DIK_U", 
							"DIK_V", "DIK_W", "DIK_X", "DIK_Y", "DIK_Z", "DIK_F1", "DIK_F2", 
							"DIK_F3", "DIK_F4", "DIK_F5", "DIK_F6", "DIK_F7", "DIK_F8", "DIK_F9", 
							"DIK_F10", "DIK_F11", "DIK_F12", "DIK_ESCAPE", "DIK_TAB", 
							"DIK_MINUS", "DIK_EQUALS", "DIK_BACK", "DIK_RETURN", 
							"DIK_LBRACKET", "DIK_RBRACKET", "DIK_LCONTROL", 
							"DIK_RCONTROL", "DIK_LSHIFT", "DIK_RSHIFT", "DIK_LALT", 
							"DIK_RALT", "DIK_SEMICOLON", "DIK_APOSTROPHE", 
							"DIK_BACKSLASH", "DIK_COMMA", "DIK_PERIOD", "DIK_SPACE", 
							"DIK_CAPITAL", "DIK_NUMLOCK", "DIK_SCROLL", "DIK_LEFT", 
							"DIK_RIGHT", "DIK_UP", "DIK_DOWN", "DIK_PAUSE", "DIK_HOME", 
							"DIK_INSERT", "DIK_DELETE", "DIK_NUMPAD1", "DIK_NUMPAD2", 
							"DIK_NUMPAD3", "DIK_NUMPAD4", "DIK_NUMPAD5", "DIK_NUMPAD6", 
							"DIK_NUMPAD7", "DIK_NUMPAD8", "DIK_NUMPAD9", "DIK_NUMPAD0", 
							"DIK_NUMPADENTER", "DIK_NUMPADEQUALS", "DIK_DECIMAL", "DIK_ADD", 
							"DIK_SUBTRACT", "DIK_MULTIPLY", "DIK_DIVIDE"};


//-------------------------------------------------------
// class HGooey (private)
//-------------------------------------------------------
bool __stdcall _SetMenuOptions(int _position, float _iwidth, float _iheight)
{
	return g_HGooey->SetMenuOptions(_position, _iwidth, _iheight);
}

bool __stdcall _ToggleMenu()
{
	return g_HGooey->ToggleMenu();
}


bool __stdcall _Initialize(IDirect3DDevice8* device)
{
	return g_HGooey->Initialize(device);
}


bool __stdcall _HandleKeyboard(BYTE cKey, char* buffer, bool bDown, bool bRepeat)
{
	return g_HGooey->HandleKeyboard(cKey, buffer, bDown, bRepeat);
}


bool __stdcall _HandleMouse(int cState, DIMOUSESTATE2* mouse)
{
	return g_HGooey->HandleMouse(cState, mouse);
}


void __stdcall _EnableDraw(bool _enable)
{
	g_HGooey->DrawEnabled = _enable;
}


void __stdcall _EndScene(float _cursorX, float _cursorY)
{
	g_HGooey->EndScene(_cursorX, _cursorY);
}


void __stdcall _LostDevice(void)
{
	g_HGooey->LostDevice();
}


void __stdcall _ResetDevice(void)
{
	g_HGooey->ResetDevice();
}


//-------------------------------------------------------
// class HGooey (public)
//-------------------------------------------------------
bool __stdcall _HGVersionCheck(UINT _version)
{
	return (HGVersion >= _version) &&
		(_version >= 71223) && // xenimus locks the mouse starting version 1.114
		(_version >= 70102) // phased out ->move
		;
}


IDirect3DDevice8* __stdcall _D3DDevice(void)
{
	return g_HGooey->h_pD3DDevice;
}


KeyInfo __stdcall _Key(void)
{
	return g_HGooey->xKey;
}


MouseInfo __stdcall _Mouse(void)
{
	return g_HGooey->xMouse;
}


float __stdcall _FontWidth(void)
{
	return g_HGooey->FontWidth;
}


float __stdcall _FontHeight(void)
{
	return g_HGooey->FontHeight;
}


float __stdcall _ScreenWidth(void)
{
	return g_HGooey->h_iScreenWidth;
}


float __stdcall _ScreenHeight(void)
{
	return g_HGooey->h_iScreenHeight;
}


const char* __stdcall _ReadConfigItem(char* _item, char* _filename)
{
	return g_HGooey->ReadConfigItem(_item, _filename);
}


bool __stdcall _WriteConfigItem(char* _item, char* _data, char* _filename)
{
	return g_HGooey->WriteConfigItem(_item, _data, _filename);
}


HGNodeList* __stdcall _CreateExtension(IDDTexture* _texture, RECT* _rect)
{
	return g_HGooey->CreateExtension(_texture, _rect);
}


bool __stdcall _UnloadExtension(HGNodeList* _ext)
{
	return g_HGooey->UnloadExtension(_ext);
}


bool __stdcall _SetExtensionMenuItem(HGNodeList* _ext, IDDTexture* _texture, RECT* _rect)
{
	return g_HGooey->SetExtensionMenuItem(_ext, _texture, _rect);
}


bool __stdcall _CheckExtMenuIO(HGNodeList* _ext)
{
	return g_HGooey->CheckExtMenuIO(_ext);
}


void __stdcall _SetExtMenuIO(HGNodeList* _ext, bool _io)
{
	g_HGooey->SetExtMenuIO(_ext, _io);
}


HGOImage* __stdcall _CreateImage(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
{
	return g_HGooey->CreateImage(_wscale, _hscale, _texture, _rect);
}

HGOTextBar* __stdcall _CreateTextBar(char* _text, float _wscale, float _hscale)
{
	return g_HGooey->CreateTextBar(_text, _wscale, _hscale);
}

HGOTextOverImage* __stdcall _CreateTextOverImage(char* _text)
{
	return g_HGooey->CreateTextOverImage(_text);
}


HGObj* __stdcall _CreateAreaSelection(float _width, float _height)
{
	return g_HGooey->CreateAreaSelection(_width, _height);
}


HGOListViewer* __stdcall _CreateListViewer(float _wscale, float _hscale, bool _duplicates, float _tscale)
{
	return g_HGooey->CreateListViewer(_wscale, _hscale, _duplicates, _tscale);
}


HGOScrollBar* __stdcall _CreateScrollBar(HGOListViewer* _parent, UINT _orientation, float _width, float _height)
{
	return g_HGooey->CreateScrollBar(_parent, _orientation, _width, _height);
}


HGODefaultList* __stdcall _CreateDefaultList(char* _title, float _scale, bool _duplicates)
{
	return g_HGooey->CreateDefaultList(_title, _scale, _duplicates);
}

HGOButton* __stdcall _CreateButton(char* _text, float _scale)
{
	return g_HGooey->CreateButton(_text, _scale);
}


bool __stdcall _AttachObject1(HGObj* _parent, HGObj* _obj)
{
	return g_HGooey->AttachObject(_parent, _obj);
}


bool __stdcall _AttachObject2(HGNodeList* _parent, HGObj* _obj)
{
	return g_HGooey->AttachObject(_parent, _obj);
}


IDDTexture* __stdcall _CreateIDDTextureFromFile(float _width, float _height, char* _filename, UINT _color)
{
	return g_HGooey->CreateIDDTextureFromFile(_width, _height, _filename, _color);
}


IDDTexture* __stdcall _CreateIDDTexture(float _width, float _height, IDirect3DTexture8* _texture)
{
	return g_HGooey->CreateIDDTexture(_width, _height, _texture);
}


void __stdcall _ActivateFull1(HGObj* _obj, bool _active)
{
	g_HGooey->ActivateFull(_obj, _active);
}


void __stdcall _ActivateFull2(HGNodeList* _list, bool _active)
{
	g_HGooey->ActivateFull(_list, _active);
}


void __stdcall _DrawText(char* _str, float _x, float _y, float _scale, UINT _color, bool _vertical, RECT* _trim)
{
	g_HGooey->DrawText(_str, _x, _y, _scale, _color, _vertical, _trim);
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
char* __stdcall _restring(const char* _str, int _start, int _size)
{
	if (!_str || _start < 0 || _size < 1) return "";

	char* cTemp = new char[_size+1];

	for (int i = _start; i < _start + _size; i++)
	{
		if (i < (int)strlen(_str))
			cTemp[i - _start] = _str[i];
		else
			cTemp[i - _start] = NULL;
	}

	cTemp[_size] = NULL;

	return cTemp;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
BYTE __stdcall _convertDIK_Name(const char* _name) 
{
	if (_name)
	{
		for (int i = 0; i < keyCount; i++)
			if (_stricmp(_name, DIK_Name[i]) == 0) return DIK_Key[i];
	}

	return 0x00;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
BYTE __stdcall _convertDIK_KeyText(const char* _text) 
{
	if (_text)
	{
		for (int i = 0; i < keyCount; i++)
			if (_stricmp(_text, DIK_KeyText[i]) == 0) return DIK_Key[i];
	}

	return 0x00;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
char* __stdcall _convertDIK_Key(BYTE _key) 
{
	for (int i = 0; i < keyCount; i++)
		if (_key == DIK_Key[i]) return DIK_Name[i];

	return "";
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
float __stdcall _W2X(float _width)
{
	return _width * g_HGooey->h_fGuiScaleX;
}


float __stdcall _H2Y(float _height)
{
	return _height * g_HGooey->h_fGuiScaleY;
}


float __stdcall _X2W(float _xDifference)
{
	return _xDifference / g_HGooey->h_fGuiScaleX;
}


float __stdcall _Y2H(float _yDifference)
{
	return _yDifference / g_HGooey->h_fGuiScaleY;
}


//-------------------------------------------------------
// class IDDTexture
//-------------------------------------------------------
float __stdcall IDDTexture_GetWidth(IDDTexture* element)
{
	return element->Width;
}

float __stdcall IDDTexture_GetHeight(IDDTexture* element)
{
	return element->Height;
}


//-------------------------------------------------------
// class HGObj
//-------------------------------------------------------
bool __stdcall _SetRemoteAction(HGObj* element, ACTIONPROC _remoteaction)
{
	return element->SetRemoteAction(_remoteaction);
}

void __stdcall _SetStickyIO(HGObj* element, bool _stickyio)
{
	element->SetStickyIO(_stickyio);
}

bool __stdcall _GetStickyIO(HGObj* element)
{
	return element->GetStickyIO();
}

void __stdcall _SetIO(HGObj* element, bool _io)
{
	element->SetIO(_io);
}

bool __stdcall _GetIO(HGObj* element)
{
	return element->GetIO();
}

void __stdcall _SetActive(HGObj* element, bool _active)
{
	element->Active = _active;
}

bool __stdcall _GetActive(HGObj* element)
{
	return element->Active;
}

void __stdcall _SetCancelClick(HGObj* element, bool _cancelclick)
{
	element->SetCancelClick(_cancelclick);
}

bool __stdcall _GetCancelClick(HGObj* element)
{
	return element->GetCancelClick();
}

void __stdcall _SetAcceptAction(HGObj* element, bool _acceptaction)
{
	element->SetAcceptAction(_acceptaction);
}

bool __stdcall _GetAcceptAction(HGObj* element)
{
	return element->GetAcceptAction();
}

void __stdcall _SetMoveable(HGObj* element, UINT _moveable)
{
	element->Moveable = _moveable;
}

UINT __stdcall _GetMoveable(HGObj* element)
{
	return element->Moveable;
}

float __stdcall _GetX(HGObj* element)
{
	return element->X;
}

float __stdcall _GetY(HGObj* element)
{
	return element->Y;
}

float __stdcall _GetWidth(HGObj* element)
{
	return element->Width;
}

float __stdcall _GetHeight(HGObj* element)
{
	return element->Height;
}

void __stdcall _Move(HGObj* element, float _wDifference, float _hDifference)
{
	element->Move(_wDifference, _hDifference);
}


//-------------------------------------------------------
// class HGOImage : public HGObj
//-------------------------------------------------------
bool __stdcall _ResetRect(HGOImage* element, int _left, int _top, int _right, int _bottom)
{
	return element->ResetRect(_left, _top, _right, _bottom);
}

void __stdcall _SetTexture(HGOImage* element, float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
{
	element->SetTexture(_wscale, _hscale, _texture, _rect);
}

void __stdcall _ReScale(HGOImage* element, float _wscale, float _hscale)
{
	element->ReScale(_wscale, _hscale);
}

float __stdcall _GetWidthScale(HGOImage* element)
{
	return element->GetWidthScale();
}

float __stdcall _GetHeightScale(HGOImage* element)
{
	return element->GetHeightScale();
}


//-------------------------------------------------------
// class HGOTextOverImage : public HGOImage
//-------------------------------------------------------
void __stdcall _SetText(HGOTextOverImage* element, char* _str)
{
	element->SetText(_str);
}

void __stdcall _SetBorderWidth(HGOTextOverImage* element, float _borderwidth)
{
	element->SetBorderWidth(_borderwidth);
}

void __stdcall _SetBorderHeight(HGOTextOverImage* element, float _borderheight)
{
	element->SetBorderHeight(_borderheight);
}


//-------------------------------------------------------
// class HGOListViewer : public HGOTextOverImage
//-------------------------------------------------------
void __stdcall _SetAutoScroll(HGOListViewer* element, bool _autoscroll)
{
	element->AutoScroll = _autoscroll;
}

bool __stdcall _GetAutoScroll(HGOListViewer* element)
{
	return element->AutoScroll;
}

bool __stdcall _LoadString(HGOListViewer* element, char* _str)
{
	return element->LoadString(_str);
}

bool __stdcall _UnloadString(HGOListViewer* element, char* _str)
{
	return element->UnloadString(_str);
}

bool __stdcall _UnloadStringAt(HGOListViewer* element, int _index)
{
	return element->UnloadStringAt(_index);
}

char* __stdcall _ContainsString(HGOListViewer* element, char* _str)
{
	return element->ContainsString(_str);
}

char* __stdcall _GetSelected(HGOListViewer* element)
{
	return element->GetSelected();
}

void __stdcall _UnSelect(HGOListViewer* element)
{
	element->UnSelect();
}

bool __stdcall _SetStringAt(HGOListViewer* element, int _index, char* _str)
{
	return element->SetStringAt(_index, _str);
}

char* __stdcall _StringAt(HGOListViewer* element, int _index)
{
	return element->StringAt(_index);
}

void __stdcall _Empty(HGOListViewer* element)
{
	element->Empty();
}

void __stdcall _ReScaleByLines(HGOListViewer* element, float _wscale, int _lines)
{
	element->ReScaleByLines(_wscale, _lines);
}

//-------------------------------------------------------
// class HGMenu
//-------------------------------------------------------
bool __stdcall _isOpen()
{
	return (g_HGooey->h_oHGMenu)->isOpen();
}


//-------------------------------------------------------
// class HGODefaultList : public HGOImage
//-------------------------------------------------------
HGOListViewer* __stdcall _GetListViewer(HGODefaultList* element)
{
	return element->ListViewer;
}

HGOButton* __stdcall _GetButtonOK(HGODefaultList* element)
{
	return element->ButtonOK;
}

HGOButton* __stdcall _GetButtonCancel(HGODefaultList* element)
{
	return element->ButtonCancel;
}


//-------------------------------------------------------
// class HGOTextBar : public HGOTextOverImage
//-------------------------------------------------------
void __stdcall _SetTextColor(HGOTextBar* element, UINT _textcolor)
{
	element->TextColor = _textcolor;
}

UINT __stdcall _GetTextColor(HGOTextBar* element)
{
	return element->TextColor;
}


//-------------------------------------------------------
// class HGOButton : public HGOTextBar
//-------------------------------------------------------
void __stdcall _SetIdleColor(HGOButton* element, UINT _idlecolor)
{
	element->IdleColor = _idlecolor;
}

UINT __stdcall _GetIdleColor(HGOButton* element)
{
	return element->IdleColor;
}

void __stdcall _SetActiveColor(HGOButton* element, UINT _activecolor)
{
	element->ActiveColor = _activecolor;
}

UINT __stdcall _GetActiveColor(HGOButton* element)
{
	return element->ActiveColor;
}

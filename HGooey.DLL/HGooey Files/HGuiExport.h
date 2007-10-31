#ifndef HGUIWRAPPER_H
#define HGUIWRAPPER_H

#pragma once

#include "HGIncDef_Public.h"

//-------------------------------------------------------
// class HGooey (private)
//-------------------------------------------------------
bool __stdcall _SetMenuOptions(int _position, float _iwidth, float _iheight);
bool __stdcall _ToggleMenu(void);
bool __stdcall _Initialize(IDirect3DDevice8* device);
bool __stdcall _HandleKeyboard(BYTE cKey, char* buffer, bool bDown, bool bRepeat);
bool __stdcall _HandleMouse(int cState, DIMOUSESTATE2* mouse);
void __stdcall _EnableDraw(bool _enable);
void __stdcall _EndScene(float _cursorX = -1, float _cursorY = -1);
void __stdcall _LostDevice(void);
void __stdcall _ResetDevice(void);

//-------------------------------------------------------
// class HGooey (public)
//-------------------------------------------------------
bool __stdcall _HGVersionCheck(UINT _version);
IDirect3DDevice8* __stdcall _D3DDevice(void);
KeyInfo __stdcall _Key(void);
MouseInfo __stdcall _Mouse(void);
float __stdcall _FontWidth(void);
float __stdcall _FontHeight(void);
float __stdcall _ScreenWidth(void);
float __stdcall _ScreenHeight(void);
const char* __stdcall _ReadConfigItem(char* _item, char* _filename);
bool __stdcall _WriteConfigItem(char* _item, char* _data, char* _filename);
HGNodeList* __stdcall _CreateExtension(IDDTexture* _texture = NULL, RECT* _rect = NULL);
bool __stdcall _UnloadExtension(HGNodeList* _ext);
bool __stdcall _SetExtensionMenuItem(HGNodeList* _ext, IDDTexture* _texture, RECT* _rect = NULL);
bool __stdcall _CheckExtMenuIO(HGNodeList* _ext);
void __stdcall _SetExtMenuIO(HGNodeList* _ext, bool _io);
HGOImage* __stdcall _CreateImage(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect = NULL);
HGOTextBar* __stdcall _CreateTextBar(char* _text, float _wscale, float _hscale);
HGOTextOverImage* __stdcall _CreateTextOverImage(char* _text = NULL);
HGObj* __stdcall _CreateAreaSelection(float _width, float _height);
HGOListViewer* __stdcall _CreateListViewer(float _wscale, float _hscale, bool _duplicates, float _tscale = 1.0f);
HGOScrollBar* __stdcall _CreateScrollBar(HGOListViewer* _parent, UINT _orientation, float _width, float _height);
HGODefaultList* __stdcall _CreateDefaultList(char* _title, float _scale, bool _duplicates);
HGOButton* __stdcall _CreateButton(char* _text, float _scale);
bool __stdcall _AttachObject1(HGObj* _parent, HGObj* _obj);
bool __stdcall _AttachObject2(HGNodeList* _parent, HGObj* _obj);
IDDTexture* __stdcall _CreateIDDTextureFromFile(float _width, float _height, char* _filename, UINT _color);
IDDTexture* __stdcall _CreateIDDTexture(float _width, float _height, IDirect3DTexture8* _texture);
void __stdcall _ActivateFull1(HGObj* _obj, bool _active);
void __stdcall _ActivateFull2(HGNodeList* _list, bool _active);
void __stdcall _DrawText(char* _str, float _x, float _y, float _scale, UINT _color, bool _vertical = false, RECT* _trim = NULL);

//-------------------------------------------------------
// 
//-------------------------------------------------------
float __stdcall _W2X(float _width);
float __stdcall _H2Y(float _height);
float __stdcall _X2W(float _xDifference);
float __stdcall _Y2H(float _yDifference);
char* __stdcall _restring(const char* _str, int _start, int _size);
BYTE __stdcall _convertDIK_Name(const char* _name);
BYTE __stdcall _convertDIK_KeyText(const char* _text);
char* __stdcall _convertDIK_Key(BYTE _key);

//-------------------------------------------------------
// class IDDTexture
//-------------------------------------------------------
float __stdcall IDDTexture_GetWidth(IDDTexture* element);
float __stdcall IDDTexture_GetHeight(IDDTexture* element);

//-------------------------------------------------------
// class HGObj
//-------------------------------------------------------
bool __stdcall _SetRemoteAction(HGObj* element, ACTIONPROC _remoteaction);
void __stdcall _SetStickyIO(HGObj* element, bool _stickyio);
bool __stdcall _GetStickyIO(HGObj* element);
void __stdcall _SetIO(HGObj* element, bool _io);
bool __stdcall _GetIO(HGObj* element);
void __stdcall _SetActive(HGObj* element, bool _active);
bool __stdcall _GetActive(HGObj* element);
void __stdcall _SetCancelClick(HGObj* element, bool _cancelclick);
bool __stdcall _GetCancelClick(HGObj* element);
void __stdcall _SetAcceptAction(HGObj* element, bool _acceptaction);
bool __stdcall _GetAcceptAction(HGObj* element);
void __stdcall _SetMoveable(HGObj* element, UINT _moveable);
UINT __stdcall _GetMoveable(HGObj* element);
float __stdcall _GetX(HGObj* element);
float __stdcall _GetY(HGObj* element);
float __stdcall _GetWidth(HGObj* element);
float __stdcall _GetHeight(HGObj* element);
void __stdcall _Move(HGObj* element, float _wDifference, float _hDifference);

//-------------------------------------------------------
// class HGOImage : public HGObj
//-------------------------------------------------------
bool __stdcall _ResetRect(HGOImage* element, int _left, int _top, int _right, int _bottom);
void __stdcall _SetTexture(HGOImage* element, float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect = NULL);
void __stdcall _ReScale(HGOImage* element, float _wscale, float _hscale);
float __stdcall _GetWidthScale(HGOImage* element);
float __stdcall _GetHeightScale(HGOImage* element);

//-------------------------------------------------------
// class HGOTextOverImage : public HGOImage
//-------------------------------------------------------
void __stdcall _SetText(HGOTextOverImage* element, char* _str);
void __stdcall _SetBorderWidth(HGOTextOverImage* element, float _borderwidth);
void __stdcall _SetBorderHeight(HGOTextOverImage* element, float _borderheight);

//-------------------------------------------------------
// class HGOListViewer : public HGOTextOverImage
//-------------------------------------------------------
void __stdcall _SetAutoScroll(HGOListViewer* element, bool _autoscroll);
bool __stdcall _GetAutoScroll(HGOListViewer* element);
bool __stdcall _LoadString(HGOListViewer* element, char* _str);
bool __stdcall _UnloadString(HGOListViewer* element, char* _str);
bool __stdcall _UnloadStringAt(HGOListViewer* element, int _index);
char* __stdcall _ContainsString(HGOListViewer* element, char* _str);
char* __stdcall _GetSelected(HGOListViewer* element);
void __stdcall _UnSelect(HGOListViewer* element);
bool __stdcall _SetStringAt(HGOListViewer* element, int _index, char* _str);
char* __stdcall _StringAt(HGOListViewer* element, int _index);
void __stdcall _Empty(HGOListViewer* element);
void __stdcall _ReScaleByLines(HGOListViewer* element, float _wscale, int _lines);

//-------------------------------------------------------
// class HGMenu
//-------------------------------------------------------
bool __stdcall _isOpen(void);

//-------------------------------------------------------
// class HGODefaultList : public HGOImage
//-------------------------------------------------------
HGOListViewer* __stdcall _GetListViewer(HGODefaultList* element);
HGOButton* __stdcall _GetButtonOK(HGODefaultList* element);
HGOButton* __stdcall _GetButtonCancel(HGODefaultList* element);

//-------------------------------------------------------
// class HGOTextBar : public HGOTextOverImage
//-------------------------------------------------------
void __stdcall _SetTextColor(HGOTextBar* element, UINT _textcolor);
UINT __stdcall _GetTextColor(HGOTextBar* element);

//-------------------------------------------------------
// class HGOButton : public HGOTextBar
//-------------------------------------------------------
void __stdcall _SetIdleColor(HGOButton* element, UINT _idlecolor);
UINT __stdcall _GetIdleColor(HGOButton* element);
void __stdcall _SetActiveColor(HGOButton* element, UINT _activecolor);
UINT __stdcall _GetActiveColor(HGOButton* element);

#endif
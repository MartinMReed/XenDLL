#ifndef HGOOEY_H
#define HGOOEY_H

#pragma once

#include "HGIncDef_Private.h"

//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGooey
{

public:

	~HGooey(void);
	HGooey(void);

	////////////////////////////////////////////////////
	/// GUI SYSTEM
	////////////////////////////////////////////////////
	const char* ReadConfigItem(char* _item, char* _filename);
	bool WriteConfigItem(char* _item, char* _data, char* _filename);
	HGNodeList* CreateExtension(IDDTexture* _texture = NULL, RECT* _rect = NULL);
	bool SetExtensionMenuItem(HGNodeList* _ext, IDDTexture* _texture, RECT* _rect = NULL);
	bool CheckExtMenuIO(HGNodeList* _ext);
	void SetExtMenuIO(HGNodeList* _ext, bool _io);
	HGOImage* CreateImage(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect = NULL);
	HGOTextBar* CreateTextBar(char* _text, float _wscale, float _hscale);
	HGOTextOverImage* CreateTextOverImage(char* _text = NULL);
	HGObj* CreateAreaSelection(float _width, float _height);
	HGOListViewer* CreateListViewer(float _wscale, float _hscale, bool _duplicates, float _tscale = 1.0f);
	HGODefaultList* CreateDefaultList(char* _title, float _scale, bool _duplicates);
	HGOButton* CreateButton(char* _text, float _scale);
	HGOScrollBar* CreateScrollBar(HGOListViewer* _parent, UINT _orientation, float _width, float _height);
	bool AttachObject(HGNodeList* _parent, HGObj* _obj);
	bool AttachObject(HGObj* _parent, HGObj* _obj);
	bool UnloadExtension(HGNodeList* _ext);
	bool SetMenuOptions(int _position, float _iwidth, float _iheight);
	bool ToggleMenu(void);
	IDDTexture* CreateIDDTextureFromFile(float _width, float _height, char* _filename, UINT _color);
	IDDTexture* CreateIDDTexture(float _width, float _height, IDirect3DTexture8* _texture);
	void ActivateFull(HGObj* _obj, bool _active);
	void ActivateFull(HGNodeList* _list, bool _active);
	void DrawText(char* _str, float _x, float _y, float _scale, UINT _color, bool _vertical = false, RECT* _trim = NULL);
	////////////////////////////////////////////////////

	bool Initialize(IDirect3DDevice8* device);

	bool HandleKeyboard(BYTE cKey, char* buffer, bool bDown, bool bRepeat);

	bool HandleMouse(int cState, DIMOUSESTATE2* mouse);

	void EndScene(float _cursorX = -1, float _cursorY = -1);

	void LostDevice(void);
	void ResetDevice(void);

//private:

	////////////////////////////////////////////////////
	/// GUI SYSTEM
	////////////////////////////////////////////////////
	HGMenu* h_oHGMenu; // menu button, and list of menu items
	HGNodeList* h_oHGExtList; // list of extensions created
	HGNodeList* h_oHGUnAttachedList; // objects created, but not yet attached to anything
	HGNodeList* h_oHGTextureList; // list of textures that have been loaded
	bool h_bMoveActive; // currently moving an object
	HGObj* h_iObjClicked; // object (id) clicked on
	IDDTexture* h_oCursorTexture;
	IDDTexture* h_oFontTexture;
	RECT FontChar[8][8];
	float FontWidth;
	float FontHeight;
	bool DrawEnabled;

	KeyInfo xKey;
	MouseInfo xMouse;
	
	void ResetAllDrawn(void);
	void DrawAll(void);
	void DrawObject(bool _highlight, HGObj* _obj);
	HGObj* UnAttached(HGObj* _obj);
	HGObj* Attached(HGObj* _obj);
	HGObj* ActionObject(HGObj* _obj);
	HGObj* CheckListAction(HGObj* _obj);
	HGObj* ActionPerformed(void); // if clicked on active obj or not
	bool ObjEvent(HGObj* _obj);
	bool CursorOver(float _x, float _y, float _xdifference, float _ydifference);
	RECT Char2Rect(char _char);
	void DrawIDDTexture(float _width, float _height, float _x, float _y, IDDTexture* _texture, RECT* _rect);
	////////////////////////////////////////////////////

	void RenderSprite(float X, float Y, float Width, float Height,			// (x,y), W by H
					  float U, float V,										// texture width/height
					  IDirect3DTexture8* pTexture,							// h_pPanelTexture
					  D3DCOLOR nColor,										// color
					  RECT* rTexture = NULL,								// optional
					  bool bCenteredX = false, bool bCenteredY = false);	// optional

	float h_iScreenWidth, h_iScreenHeight;
	float h_fGuiScaleX, h_fGuiScaleY;

	IDirect3DDevice8* h_pD3DDevice;

	ID3DXSprite* h_pSprite;
};

#endif

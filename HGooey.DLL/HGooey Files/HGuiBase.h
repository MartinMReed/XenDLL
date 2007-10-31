#ifndef HGUIBASE_H
#define HGUIBASE_H

#pragma once

#include "HGIncDef_Public.h"




//-------------------------------------------------------
// 
//-------------------------------------------------------
class IDDTexture
{

public:

	~IDDTexture(void);
	IDDTexture(float _width = 0.0f, float _height = 0.0f, IDirect3DTexture8* _texture = NULL, char* _filename = NULL, UINT _color = COLOR_DEFAULT);

	///

	UINT Color;

	char* Filename;

	float Width;
	float Height;

	IDirect3DTexture8* Texture;

protected:

private:

};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGObj
{

public:

	~HGObj(void);
	HGObj(float _width, float _height);

	///

	void Move(float _wDifference, float _hDifference);

	void MoveByCoord(float _xDifference, float _yDifference);

	void ResetDrawn(void);

	bool SetRemoteAction(ACTIONPROC _remoteaction);

	///

	virtual bool GetIO(void);
	virtual void SetIO(bool _io);

	virtual bool GetStickyIO(void);
	virtual void SetStickyIO(bool _stickyIO);

	virtual bool GetCancelClick(void);
	virtual void SetCancelClick(bool _cancelClick);

	virtual bool GetAcceptAction(void);
	virtual void SetAcceptAction(bool _acceptAction);

	virtual void Display(bool _ignore = false);

	///

	bool Drawn;

	bool AlreadyAttached;

	HGNodeList* Attachments;

	ACTIONPROC RemoteAction;

	UINT Moveable;

	bool Active;

	float Width;
	float Height;

	float X;
	float Y;

protected:

private:

	void Setup(void);

	///
	
	bool IO;
	bool StickyIO;
	bool CancelClick;
	bool AcceptAction;
};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGOImage : public HGObj
{

public:

	~HGOImage(void);
	HGOImage(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect);

	///

	bool ResetRect(int _left, int _top, int _right, int _bottom);

	///

	virtual void Display(bool _ignore = false);

	virtual float GetWidthScale(void);
	virtual float GetHeightScale(void);

	virtual void ReScale(float _wscale, float _hscale);

	virtual void SetTexture(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect = NULL);

	///
	
	RECT* Rect;
	IDDTexture* Texture;

protected:

private:

	void Setup(void);
	
	///

	float ImageWidth;
	float ImageHeight;
};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGOTextOverImage : public HGOImage
{

public:

	~HGOTextOverImage(void);
	HGOTextOverImage(char* _text);

	///

	void SetText(char* _str);

	///

	virtual void ReScale(float _wscale, float _hscale);

	virtual void SetBorderWidth(float _borderwidth);
	virtual void SetBorderHeight(float _borderheight);

	virtual void SetMaxWidth(void);
	virtual void SetMaxHeight(void);

	virtual void SetTexture(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect = NULL);

	///
	
	char* Text;

	int MaxWidth;
	int MaxHeight;

	float BorderWidth;
	float BorderHeight;

protected:

private:

	void Setup(void);
};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGOListViewer : public HGOTextOverImage
{

public:

	~HGOListViewer(void);
	HGOListViewer(float _wscale, float _hscale, bool _duplicates, float _tscale = 1.0f);

	///

	bool CheckHitBottom(void);

	char* ContainsString(char* _str);

	void Empty(void);

	char* GetSelected(void);

	bool LoadString(char* _str);

	bool UnloadString(char* _str);

	bool UnloadStringAt(int _index);

	bool UnloadStringNode(HGNode* _node);

	void UnSelect(void);

	void ReScaleByLines(float _wscale, int _lines);

	void ScrollUp(void);
	void ScrollDown(void);
	void ScrollLeft(void);
	void ScrollRight(void);

	bool SetStringAt(int _index, char* _str);

	char* StringAt(int _index);

	///

	virtual void Display(bool _ignore = false);

	virtual bool GetIO(void);
	virtual void SetIO(bool _io);

	virtual bool GetStickyIO(void);
	virtual void SetStickyIO(bool _stickyIO);

	virtual bool GetCancelClick(void);
	virtual void SetCancelClick(bool _cancelClick);

	virtual void SetAcceptAction(bool _acceptAction);

	virtual float GetWidthScale(void);
	virtual float GetHeightScale(void);

	virtual void ReScale(float _wscale, float _hscale);

	virtual void SetBorderWidth(float _borderwidth);
	virtual void SetBorderHeight(float _borderheight);

	virtual void SetMaxWidth(void);
	virtual void SetMaxHeight(void);

	virtual void SetTexture(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect = NULL);

	///

	bool AutoScroll;

protected:

private:

	void Setup(void);

	///

	HGNode* Selected;

	HGNode* VFirst;
	int HFirst; 

	int Longest;

	float TextScale;

	bool Duplicates;

	HGNodeList* StringList;

	HGOImage* List; 

	HGOScrollBar* HoriScroll;
	HGOScrollBar* VertScroll;
};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGNode
{

public:

	~HGNode(void);
	HGNode(void* _element = NULL, bool _array = false, bool _copy = false);

	///

	HGNode* Prev;
	HGNode* Next;

	void* Element;

protected:

private:

	bool Copy;
	bool Array;

};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGExtNode : public HGNode
{

public:

	~HGExtNode(void);
	HGExtNode(void* _element = NULL, bool _copy = false);

	///

	HGNodeList* Ext;

protected:

private:

};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGNodeList
{

public:

	~HGNodeList(void);
	HGNodeList(void);

	///

	HGNode* Find(void* _element);

	HGExtNode* Find(HGNodeList* _ext);

	HGNode* GetHead(void);

	bool LoadNode(HGNode* _node, bool _frontload = false);

	HGNode* NodeAt(int _index);

	bool UnloadNode(HGNode* _node, bool _delete);

	///

	HGNode* Head;
	HGNode* Last;

protected:

private:

};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGMenu
{

public:

	~HGMenu(void);
	HGMenu(HGOButton* _button, int _position, float _iwidth = 0.0f, float _iheight = 0.0f);

	///

	void ActivateItems(bool _active);

	HGOImage* AddMenuItem(IDDTexture* _texture, RECT* _rect = NULL);

	bool isOpen(void);

	bool RemoveMenuItem(HGObj* _obj);

	///

	int Position;

	int ItemCount;

	HGNodeList* ExtButtons;

	HGOImage* MoveControl;

	HGOButton* StartButton;

protected:

private:

	float ItemWidth;
	float ItemHeight;
};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGODefaultList : public HGOImage
{

public:

	~HGODefaultList(void);
	HGODefaultList(char* _title, float _scale, bool _duplicates);

	///

	virtual void Display(bool _ignore = false);

	///

	HGOButton* ButtonOK;
	HGOButton* ButtonCancel;

	HGOListViewer* ListViewer;

protected:

private:

};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGOTextBar : public HGOTextOverImage
{

public:

	HGOTextBar(char* _text, float _wscale, float _hscale);

	///

	virtual void Display(bool _ignore = false);

	///

	UINT TextColor;

protected:

private:

	void Setup(void);
};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGOButton : public HGOTextBar
{

public:

	HGOButton(char* _text, float _scale);

	///

	virtual void Display(bool _ignore = false);

	virtual void ReScale(float _wscale, float _hscale);

	///

	UINT IdleColor;
	UINT ActiveColor;

protected:

private:

};




//-------------------------------------------------------
// 
//-------------------------------------------------------
class HGOScrollBar : public HGObj
{

public:

	~HGOScrollBar(void);
	HGOScrollBar(HGOListViewer* _parent, UINT _orientation, float _width, float _height);

	///

	void ReScale(void);

	///

	virtual void Display(bool _ignore = false);

protected:

private:

	HGOListViewer* Parent;

	UINT Orientation;

	HGOImage* LeftUpImage;
	HGOImage* RightDownImage;
	HGOImage* BackSlideImage;
	HGOImage* FrontSlideImage;
};

#endif

#define HGOOEY_EXPORTS
#include "HGIncDef_Private.h"

HGooey* g_HGooey;

UINT COLOR_DRAW = 0x00000000;




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class IDDTexture




//-------------------------------------------------------
// 
//-------------------------------------------------------
IDDTexture::~IDDTexture()
{
	if (Filename)
	{
		delete[] Filename;
		Filename = NULL;
	}

	if (Texture)
	{
		Texture->Release();
		Texture = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
IDDTexture::IDDTexture(float _width, float _height, IDirect3DTexture8* _texture, char* _filename, UINT _color)
{
	Color = _color;

	if (_filename)
		Filename = _restring(_filename, 0, strlen(_filename));
	else
		Filename = NULL;

	if (_width < 0.0f)
		_width = 0.0f;

	Width = _width;

	if (_height < 0.0f)
		_height = 0.0f;

	Height = _height;

	Texture = _texture;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGObj




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj::~HGObj()
{
	if (Attachments)
	{
		delete Attachments;
		Attachments = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj::HGObj(float _width, float _height)
{
	Setup();

	if (_width > 0.0f)
		Width = _width;

	if (_height > 0.0f)
		Height = _height;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::Setup()
{
	Drawn = false;
	Width = 0.0f;
	Height = 0.0f;
	X = 0.0f;
	Y = 0.0f;
	IO = false;
	CancelClick = true;
	AcceptAction = true;
	Active = false;
	Moveable = MOVE_NEVER;
	Attachments = new HGNodeList();
	AlreadyAttached = false;
	RemoteAction = NULL;
	StickyIO = false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGObj::GetIO()
{
	return IO;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::SetIO(bool _io)
{
	IO = _io;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGObj::GetStickyIO()
{
	return StickyIO;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::SetStickyIO(bool _stickyIO)
{
	StickyIO = _stickyIO;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGObj::GetCancelClick()
{
	return CancelClick;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::SetCancelClick(bool _cancelClick)
{
	CancelClick = _cancelClick;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGObj::GetAcceptAction()
{
	return AcceptAction;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::SetAcceptAction(bool _acceptAction)
{
	AcceptAction = _acceptAction;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGObj::SetRemoteAction(ACTIONPROC _action)
{
	if (_action && !RemoteAction)
		RemoteAction = _action;
	else return false;

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::ResetDrawn(void)
{
	Drawn = false;

	if (Attachments)
	{
		HGNode* ObjNode = Attachments->Head;

		while (ObjNode)
		{
			HGObj* Obj = (HGObj*)(ObjNode->Element);
			Obj->ResetDrawn();

			ObjNode = ObjNode->Next;
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::Move(float _wDifference, float _hDifference)
{
	MoveByCoord(
		_W2X(_wDifference),
		_H2Y(_hDifference));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::MoveByCoord(float _xDifference, float _yDifference)
{
	X += _xDifference;
	Y += _yDifference;

	if (Attachments)
	{
		HGNode* ObjNode = Attachments->Head;

		while (ObjNode)
		{
			HGObj* Obj = (HGObj*)(ObjNode->Element);

			if (Obj)
				Obj->MoveByCoord(_xDifference, _yDifference);

			ObjNode = ObjNode->Next;
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGObj::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	if (!StickyIO && !_Mouse().LButtonDown)
		IO = false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGOImage : public HGObj




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOImage::~HGOImage()
{
	// taken care of by the texture list
	// if (Texture)
	// {
	// 	delete Texture;
	// 	Texture = NULL;
	// }
	//

	if (Rect)
	{
		delete Rect;
		Rect = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOImage::HGOImage(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
 : HGObj(0.0f, 0.0f)
{
	Setup();
	SetTexture(_wscale, _hscale, _texture, _rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOImage::Setup()
{
	ImageWidth = 0.0f;
	ImageHeight = 0.0f;
	Texture = NULL;
	Rect = NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOImage::ResetRect(int _left, int _top, int _right, int _bottom)
{
	if (!Texture || !Rect || 
		_left > _right || _top > _bottom ||
		_left < 0 || _top < 0 ||
		_right >= Texture->Width || 
		_bottom >= Texture->Height)
		return false;

	SetRect(Rect, _left, _top, _right, _bottom);

	Width = (float)(_right - _left) * (ImageWidth / Texture->Width);
	Height = (float)(_bottom - _top) * (ImageHeight / Texture->Height);

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOImage::SetTexture(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
{
	if (_wscale < 0.0f || _hscale < 0.0f)
		return;

	if (!_texture)
	{
		ImageWidth = 0.0f;
		ImageHeight = 0.0f;
		Texture = NULL;
		Rect = NULL;
		return;
	}

	Texture = _texture;

	ImageWidth = _texture->Width * _wscale;
	ImageHeight = _texture->Height * _hscale;

	Width = ImageWidth;
	Height = ImageHeight;

	if (_rect && _rect->left < _rect->right && _rect->top < _rect->bottom)
	{
		Rect = new RECT();
		SetRect(Rect, _rect->left, _rect->top, _rect->right, _rect->bottom);
		Width = (float)(_rect->right - _rect->left) * _wscale;
		Height = (float)(_rect->bottom - _rect->top) * _hscale;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOImage::ReScale(float _wscale, float _hscale)
{
	SetTexture(_wscale, _hscale, Texture, Rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
float HGOImage::GetWidthScale(void)
{
	if (!Texture || Texture->Width == 0 || (Rect && Rect->right - Rect->left == 0))
	{
		return 0.0f;
	}
	else 
	{
		if (Rect)
			return Width / (float)(Rect->right - Rect->left);
		else
			return Width / Texture->Width;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
float HGOImage::GetHeightScale(void)
{
	if (!Texture || Texture->Height == 0 || (Rect && Rect->bottom - Rect->top == 0))
	{
		return 0.0f;
	}
	else 
	{
		if (Rect)
			return Height / (float)(Rect->bottom - Rect->top);
		else
			return Height / Texture->Height;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOImage::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	HGObj::Display(true);

	if (!Texture || !g_HGooey)
		return;
	
	g_HGooey->DrawIDDTexture(ImageWidth, ImageHeight, X, Y, Texture, Rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGOTextOverImage : public HGOImage



//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOTextOverImage::~HGOTextOverImage()
{
	if (Text)
	{
		delete[] Text;
		Text = NULL;
	}
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOTextOverImage::HGOTextOverImage(char* _text)
 : HGOImage(0.0f, 0.0f, NULL, NULL)
{
	Setup();

	if (_text)
		Text = _restring(_text, 0, strlen(_text));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::Setup()
{
	Text = NULL;

	MaxWidth = 0;
	MaxHeight = 0;

	BorderWidth = 0.0f;
	BorderHeight = 0.0f;

	SetMaxWidth();
	SetMaxHeight();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::SetText(char* _str)
{
	if (!_str)
		return;

	if (Text)
	{
		delete[] Text;
		Text = NULL;
	}

	Text = _restring(_str, 0, strlen(_str));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::SetBorderWidth(float _borderwidth)
{
	_borderwidth *= GetWidthScale();

	if (_borderwidth * 2.0f > Width)
	{
		_borderwidth = Width / 2.0f;
	}

	BorderWidth = _borderwidth;
	SetMaxWidth();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::SetBorderHeight(float _borderheight)
{
	_borderheight *= GetHeightScale();

	if (_borderheight * 2.0f > Height)
	{
		_borderheight = Height / 2.0f;
	}

	BorderHeight = _borderheight;
	SetMaxHeight();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::SetMaxWidth()
{
	if (g_HGooey)
	{
		MaxWidth = (int)floor((Width - (2.0f * BorderWidth)) / _FontWidth());

		if (MaxWidth < 0)
			MaxWidth = 0;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::SetMaxHeight()
{
	if (g_HGooey)
	{
		MaxHeight = (int)floor((Height - (2.0f * BorderHeight)) / _FontHeight());

		if (MaxHeight < 0)
			MaxHeight = 0;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::ReScale(float _wscale, float _hscale)
{
	SetTexture(_wscale, _hscale, Texture, Rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextOverImage::SetTexture(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
{
	float _borderwidth = BorderWidth / GetWidthScale();
	float _borderheight = BorderHeight / GetHeightScale();
	
	HGOImage::SetTexture(_wscale, _hscale, _texture, _rect);

	SetBorderWidth(_borderwidth);
	SetBorderHeight(_borderheight);
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGOListViewer : public HGOTextOverImage




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOListViewer::~HGOListViewer()
{
	if (HoriScroll)
	{
		delete HoriScroll;
		HoriScroll = NULL;
	}
	
	if (VertScroll)
	{
		delete VertScroll;
		VertScroll = NULL;
	}
	
	if (StringList)
	{
		delete StringList;
		StringList = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOListViewer::HGOListViewer(float _wscale, float _hscale, bool _duplicates, float _tscale)
 : HGOTextOverImage(NULL)
{
	Setup();

	Duplicates = _duplicates;
	TextScale = _tscale;

	RECT _rect;

	// load list.bmp from resource
	IDirect3DTexture8* ListTexture;
	D3DXCreateTextureFromResourceEx(_D3DDevice(), hInst, MAKEINTRESOURCE(BMP_LIST), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_RED, NULL, NULL, &ListTexture);
	IDDTexture* _texture = _CreateIDDTexture(512.0f, 512.0f, ListTexture);

	SetRect(&_rect, ListViewer_left, ListViewer_top, ListViewer_right, ListViewer_bottom);
	List = _CreateImage(
		_wscale,
		_hscale,  
		_texture, 
		&_rect);

	_AttachObject1(this, List);

	Width = List->Width;
	Height = List->Height;

	List->ReScale(
		_wscale * ((Width - VSB_MI_width ) / Width),
		_hscale * ((Height - HSB_MI_height) / Height));

	///

	HoriScroll = _CreateScrollBar(
		this,
		HORIZONTAL, 
		List->Width, 
		(float)HSB_LI_height);

	_AttachObject1(this, HoriScroll);

	HoriScroll->Move(0.0f, List->Height);

	///

	VertScroll = _CreateScrollBar(
		this,
		VERTICAL, 
		(float)VSB_UI_width, 
		List->Height);

	_AttachObject1(this, VertScroll);

	VertScroll->Move(List->Width, 0.0f);

	///

	ReScale(_wscale, _hscale);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::Setup()
{
	HGObj::SetAcceptAction(false);
	HoriScroll = NULL;
	VertScroll = NULL;
	StringList = NULL;
	Selected = NULL;
	VFirst = NULL;
	HFirst = 0;
	Longest = -1;
	AutoScroll = false;
	Duplicates = false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::LoadString(char* _str)
{
	if (!_str || (!Duplicates && ContainsString(_str)))
		return false;

	char* cTemp = _restring(_str, 0, strlen(_str));

	HGNode* Node = new HGNode(cTemp, true);

	if (!StringList)
		StringList = new HGNodeList();

	if (!VFirst)
	{
		VFirst = Node;
		HFirst = 0;
		Longest = 0;
	}

	if ((int)strlen(_str) > Longest)
		Longest = (int)strlen(_str);

	bool HitBottom = CheckHitBottom();

	bool bReturn = StringList->LoadNode(Node);

	if (bReturn && AutoScroll && HitBottom)
		ScrollDown();

	return bReturn;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::UnloadString(char* _str)
{
	if (!StringList || !_str)
		return false;

	return UnloadStringNode(StringList->Find(ContainsString(_str)));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::UnloadStringAt(int _index)
{
	if (!StringList || _index < 0)
		return false;

	return UnloadStringNode(StringList->NodeAt(_index));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::UnloadStringNode(HGNode* _node)
{
	if (!_node)
		return false;

	if (Selected == _node)
		Selected = NULL;

	HGNode* Node = VFirst;

	for (int i = 0; Node && i < MaxHeight; i++)
	{
		if (Node == _node)
		{
			if (Node->Prev)
				ScrollUp();
			else if (Node == VFirst)
				VFirst = VFirst->Next;
			else if (Node->Next)
				ScrollDown();

			break;
		}

		Node = Node->Next;
	}

	char* _str = (char*)(_node->Element);
	char* cTemp = new char[strlen(_str)+1];
	strncpy_s(cTemp, strlen(_str) + 1, _str, strlen(_str));

	bool bReturn = StringList->UnloadNode(_node, true);

	if ((int)strlen(cTemp) >= Longest)
	{
		Longest = -1;
		Node = StringList->Head;

		while (Node)
		{
			char* Str = (char*)(Node->Element);

			if (Str && Longest < (int)strlen(Str))
				Longest = (int)strlen(Str);

			Node = Node->Next;
		}

		if (HFirst > Longest)
			if ((HFirst = Longest - MaxWidth) < 0)
				HFirst = 0;
	}

	delete[] cTemp;
	cTemp = NULL;

	return bReturn;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
char* HGOListViewer::ContainsString(char* _str)
{
	if (!StringList || !_str)
		return NULL;

	HGNode* Node = (HGNode*)(StringList->Head);

	while (Node)
	{
		if (Node->Element && strcmp((char*)(Node->Element), _str) == 0)
			return (char*)(Node->Element);

		Node = (HGNode*)(Node->Next);
	}

	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
char* HGOListViewer::GetSelected()
{
	if (Selected)
		return (char*)(Selected->Element);
	
	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::UnSelect()
{
	Selected = NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::SetStringAt(int _index, char* _str)
{
	if (!StringList || _index < 0 || !_str)
		return false;

	HGNode* Node = StringList->NodeAt(_index);

	if (Node)
	{
		int _oldstrlen = strlen((char*)(Node->Element));

		char* _newstr = _restring(_str, 0, strlen(_str));
		int _newstrlen = strlen(_newstr);
		
		delete[] Node->Element;
		Node->Element = _newstr;

		if (_newstrlen > Longest)
		{
			Longest = _newstrlen;
		}
		else if (_newstrlen < Longest && _oldstrlen >= Longest)
		{
			Longest = -1;
			Node = StringList->Head;

			while (Node)
			{
				char* Str = (char*)(Node->Element);

				if (Str && Longest < (int)strlen(Str))
					Longest = strlen(Str);

				Node = Node->Next;
			}

			if (HFirst > Longest)
				if ((HFirst = Longest - MaxWidth) < 0)
					HFirst = 0;
		}

		return true;
	}

	return false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
char* HGOListViewer::StringAt(int _index)
{
	if (!StringList || _index < 0)
		return NULL;

	HGNode* Node = StringList->NodeAt(_index);

	if (Node)
		return (char*)Node->Element;

	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::Empty()
{
	while (StringAt(0))
		UnloadStringAt(0);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
float HGOListViewer::GetWidthScale()
{
	float _lwidth = List->Width / List->GetWidthScale();
	return Width / _lwidth;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
float HGOListViewer::GetHeightScale()
{
	float _lheight = List->Height / List->GetHeightScale();
	return Height / _lheight;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetBorderWidth(float _borderwidth)
{
	_borderwidth *= List->GetWidthScale();

	if (_borderwidth * 2.0f > Width)
	{
		_borderwidth = Width / 2.0f;
	}

	BorderWidth = _borderwidth;
	SetMaxWidth();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetBorderHeight(float _borderheight)
{
	_borderheight *= List->GetHeightScale();

	if (_borderheight * 2.0f > Height)
	{
		_borderheight = Height / 2.0f;
	}

	BorderHeight = _borderheight;
	SetMaxHeight();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetMaxWidth()
{
	if (g_HGooey)
	{
		float _width = List->Width;
		MaxWidth = (int)floor((_width - (2.0f * BorderWidth)) / (TextScale * _FontWidth()));

		if (MaxWidth < 0)
			MaxWidth = 0;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetMaxHeight()
{
	if (g_HGooey)
	{
		float _height = List->Height;
		MaxHeight = (int)floor((_height - (2.0f * BorderHeight)) / (TextScale * _FontHeight()));

		if (MaxHeight < 0)
			MaxHeight = 0;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::ReScale(float _wscale, float _hscale)
{
	SetTexture(_wscale, _hscale, List->Texture, List->Rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::ReScaleByLines(float _wscale, int _lines)
{
	float FontHeight = TextScale * _FontHeight();
	float _height = Height / GetHeightScale();
	float _border = BorderHeight / List->GetHeightScale();
	float _scroll = (float)HSB_MI_height;
	float _hscale = ((FontHeight * (float)_lines) + _scroll) / (_height - (_border * 2.0f));
	ReScale(_wscale, _hscale);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetTexture(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
{
	float _width = Width;
	float _height = Height;

	float _borderwidth = BorderWidth / List->GetWidthScale();
	float _borderheight = BorderHeight / List->GetHeightScale();
	
	List->SetTexture(_wscale, _hscale, _texture, _rect);

	Width = List->Width;
	Height = List->Height;

	List->ReScale(
		_wscale * ((Width - VSB_MI_width) / Width),
		_hscale * ((Height - HSB_MI_height) / Height));
	
	SetBorderWidth(_borderwidth);
	SetBorderHeight(_borderheight);

	if (HoriScroll)
	{
		HoriScroll->ReScale();
		HoriScroll->Move(0.0f, Height - _height);
	}

	if (VertScroll)
	{
		VertScroll->ReScale();
		VertScroll->Move(Width - _width, 0.0f);
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::CheckHitBottom()
{
	if (!StringList)
		return false;

	HGNode* Node = VFirst;
	for (int i = 0; Node && i < MaxHeight; i++)
	{
		if (Node == StringList->Last)
			return true;

		Node = Node->Next;
	}

	return false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::ScrollUp()
{
	if (StringList && 
		VFirst && 
		VFirst->Prev)
	{
		VFirst = VFirst->Prev;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::ScrollDown()
{
	if (StringList && VFirst)
	{
		HGNode* Node = VFirst;

		bool Stop = false;
		for (int i = 0; !Stop && i < MaxHeight+1; i++)
		{
			if (Node)
				Node = Node->Next;
			else
				Stop = true;
		}

		if (!Stop && VFirst->Next)
			VFirst = VFirst->Next;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::ScrollLeft()
{
	if (StringList && HFirst > 0)
		HFirst--;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::ScrollRight()
{
	if (StringList && HFirst < Longest - MaxWidth)
		HFirst++;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::GetIO()
{
	return List->GetIO();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetIO(bool _io)
{
	List->SetIO(_io);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::GetStickyIO()
{
	return List->GetStickyIO();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetStickyIO(bool _stickyIO)
{
	List->SetStickyIO(_stickyIO);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGOListViewer::GetCancelClick()
{
	return List->GetCancelClick();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetCancelClick(bool _cancelClick)
{
	List->SetCancelClick(_cancelClick);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::SetAcceptAction(bool _acceptAction)
{
	List->SetAcceptAction(_acceptAction);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOListViewer::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	HGObj::Display(true);

	List->Display();

	if (GetIO())
	{
		float lX = _Mouse().CursorX - List->X;
		float lY = _Mouse().CursorY - List->Y;

		if (lX > _W2X(BorderWidth) && lX < _W2X(List->Width - BorderWidth) && 
			lY > _H2Y(BorderHeight) && lY < _H2Y(List->Height - BorderHeight))
		{
			HGNode* SelNode = VFirst;

			int SelIndex = (int)floor((lY - _H2Y(BorderHeight)) / _H2Y(TextScale * _FontHeight()));

			for (int i = 0; SelNode && i < SelIndex && i < MaxHeight; i++)
				SelNode = SelNode->Next;

			if (StringList && SelNode)
				Selected = SelNode;
		}
	}

	if (!StringList || !VFirst || !g_HGooey)
		return;

	float FontWidth  = TextScale * _FontWidth();
	float FontHeight = TextScale * _FontHeight();

	HGNode* Node = VFirst;

	float _width = List->Width;
	float _height = List->Height;

	int LeftOverWidth  = (int)(_width - ((_W2X(BorderWidth) * 2.0f) + (float)(FontWidth * MaxWidth)));
	int LeftOverHeight = (int)(_height - ((_H2Y(BorderHeight) * 2.0f) + (float)(FontHeight * MaxHeight)));

	RECT Trim;

	for (int i = 0; Node && i < MaxHeight; i++)
	{
		if (Node->Element)
		{
			char* cTemp;

			UINT ColorToUse = COLOR_DEFAULT;

			if (Node == Selected)
				ColorToUse = COLOR_ORANGE;
			
			cTemp = _restring((char*)(Node->Element), HFirst, MaxWidth);
			_DrawText(cTemp, _X2W(X) + BorderWidth, _Y2H(Y) + BorderHeight + (FontHeight * i), TextScale, ColorToUse);

			delete[] cTemp;
			cTemp = NULL;
		}

		Node = Node->Next;
	}

	if (Node && Node->Element && LeftOverHeight > 0)
	{
		char* cTemp;

		UINT ColorToUse = COLOR_DEFAULT;

		if (Node == Selected)
			ColorToUse = COLOR_ORANGE;

		SetRect(&Trim, 0, 0, 0, (int)_H2Y(FontHeight - (float)LeftOverHeight));
		cTemp = _restring((char*)(Node->Element), HFirst, MaxWidth);
		_DrawText(cTemp, _X2W(X) + BorderWidth, _Y2H(Y) + BorderHeight + (FontHeight * MaxHeight), TextScale, ColorToUse, false, &Trim);

		delete[] cTemp;
		cTemp = NULL;
	}

	_ActivateFull2(Attachments, Active);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGNode




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNode::~HGNode()
{
	Prev = NULL;
	Next = NULL;

	if (!Copy && Element && Array)
		delete[] Element;
	else if (!Copy && Element && !Array)
		delete Element;

	Element = NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNode::HGNode(void* _element, bool _array, bool _copy)
{
	Prev = NULL;
	Next = NULL;
	Element = _element;
	Copy = _copy;
	Array = _array;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGExtNode : public HGObjNode




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGExtNode::~HGExtNode()
{
	if (Ext)
	{
		delete Ext;
		Ext = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGExtNode::HGExtNode(void* _element, bool _copy)
 : HGNode(_element, false, _copy)
{
	Ext = new HGNodeList();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGNodeList




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNodeList::~HGNodeList()
{
	if (Head)
	{
		while (Head)
		{
			HGNode* Node = Head;
			Head = Head->Next;
			UnloadNode(Node, true);
		}
			
		Head = NULL;
	}

	Last = NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNodeList::HGNodeList()
{
	Head = NULL;
	Last = NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGNodeList::LoadNode(HGNode* _node, bool _frontload)
{
	if (!_node)
		return false;

	if (!Head)
	{
		Head = _node;
		Last = _node;
	}
	else
	{
		if (_frontload)
		{
			_node->Next = Head;
			Head->Prev = _node;
			Head = _node;

		}
		else
		{
			_node->Prev = Last;
			Last->Next = _node;
			Last = Last->Next;
		}
	}

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGNodeList::UnloadNode(HGNode* _node, bool _delete)
{
	if (!_node)
		return false;

	HGNode* Node = Head;

	while (Node)
	{
		if (_node == Node)
		{
			if (_node->Prev)
				(_node->Prev)->Next = _node->Next;

			if (_node->Next)
				(_node->Next)->Prev = _node->Prev;

			if (_node == Last)
				Last = _node->Prev;

			if (_node == Head)
				Head = _node->Next;

			if (_delete)
			{
				delete _node;
				_node = NULL;
			}

			return true;
		}

		Node = Node->Next;
	}

	return false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNode* HGNodeList::Find(void* _element)
{
	if (!_element)
		return NULL;

	HGNode* Node = (HGNode*)Head;

	while (Node)
	{
		if (_element == Node->Element)
			return Node;

		Node = (HGNode*)(Node->Next);
	}

	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGExtNode* HGNodeList::Find(HGNodeList* _ext)
{
	if (!_ext)
		return NULL;

	HGExtNode* ExtNode = (HGExtNode*)Head;

	while (ExtNode)
	{
		if (_ext == ExtNode->Ext)
			return ExtNode;

		ExtNode = (HGExtNode*)(ExtNode->Next);
	}

	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNode* HGNodeList::NodeAt(int _index)
{
	if (_index < 0)
		return NULL;

	HGNode* Node = Head;

	int i;
	for (i = 0; Node && i < _index; i++)
		Node = Node->Next;

	return Node;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGMenu




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGMenu::~HGMenu()
{
	if (StartButton)
	{
		delete StartButton;
		StartButton = NULL;
	}

	if (ExtButtons)
	{
		delete ExtButtons;
		ExtButtons = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGMenu::HGMenu(HGOButton* _button, int _position, float _iwidth, float _iheight)
{
	ItemCount = 0;

	if (_iwidth < 0.0f)
		_iwidth = 0.0f;

	ItemWidth = _iwidth;

	if (_iheight < 0.0f)
		_iheight = 0.0f;

	ItemHeight = _iheight;

	Position = _position;

	StartButton = _button;
	StartButton->Moveable = MOVE_NEVER;
	StartButton->Active = true;

	ExtButtons = NULL;

	// load list.bmp from resource
	IDirect3DTexture8* ListTexture;
	D3DXCreateTextureFromResourceEx(_D3DDevice(), hInst, MAKEINTRESOURCE(BMP_LIST), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_RED, NULL, NULL, &ListTexture);
	IDDTexture* _texture = _CreateIDDTexture(512.0f, 512.0f, ListTexture);

	RECT _rect;

	//Move control
	SetRect(&_rect, 448, 448, 511, 511);
	MoveControl = AddMenuItem(_texture, &_rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGMenu::isOpen()
{
	return StartButton->GetIO();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGMenu::ActivateItems(bool _active)
{
	if (!ExtButtons)
		return;

	HGNode* ObjNode = ExtButtons->Head;

	while (ObjNode)
	{
		HGObj* Obj = ((HGObj*)(ObjNode->Element));

		if (ObjNode->Element)
			Obj->Active = _active;

		ObjNode = ObjNode->Next;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOImage* HGMenu::AddMenuItem(IDDTexture* _texture, RECT* _rect)
{
	if (!_texture)
		return NULL;

	if (!ExtButtons)
		ExtButtons = new HGNodeList();

	float WScale = ItemWidth / _texture->Width;
	float HScale = ItemHeight / _texture->Height;

	if (_rect)
	{
		WScale = ItemWidth / (float)(_rect->right - _rect->left) ;
		HScale = ItemHeight / (float)(_rect->bottom - _rect->top) ;
	}

	HGOImage* oTemp = new HGOImage(WScale, HScale, _texture, _rect);
	oTemp->SetStickyIO(true);
	oTemp->Moveable = MOVE_NEVER;

	if (ExtButtons->LoadNode(new HGNode(oTemp, false, true)))
		ItemCount++;

	return oTemp;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGMenu::RemoveMenuItem(HGObj* _obj)
{
	if (!_obj || !ExtButtons)
		return false;

	if (ExtButtons->UnloadNode(ExtButtons->Find(_obj), true))
	{
		ItemCount--;
		return true;
	}

	return false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGODefaultList : public HGOImage




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGODefaultList::~HGODefaultList()
{
	if (ListViewer)
	{
		delete ListViewer;
		ListViewer = NULL;
	}

	if (ButtonOK)
	{
		delete ButtonOK;
		ButtonOK = NULL;
	}

	if (ButtonCancel)
	{
		delete ButtonCancel;
		ButtonCancel = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGODefaultList::HGODefaultList(char* _title, float _scale, bool _duplicates)
 : HGOImage(0.0f, 0.0f, NULL, NULL)
{
	if (!g_HGooey)
		return;

	RECT _rect;

	// load list.bmp from resource
	IDirect3DTexture8* ListTexture;
	D3DXCreateTextureFromResourceEx(_D3DDevice(), hInst, MAKEINTRESOURCE(BMP_LIST), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_RED, NULL, NULL, &ListTexture);
	IDDTexture* _texture = _CreateIDDTexture(512.0f, 512.0f, ListTexture);


	////////////////////////////////////////////////////////////////////////////
	
	SetRect(&_rect, DefaultList_left, DefaultList_top, DefaultList_right, DefaultList_bottom);
	float BackWScale = 1.5f;
	float BackHScale = 1.5f;
	SetTexture(
		_scale * BackWScale, 
		_scale * BackHScale, 
		_texture, 
		&_rect);

	Moveable = MOVE_ALWAYS;


	////////////////////////////////////////////////////////////////////////////
	
	float TitleWDiff = (10.0f * BackWScale * _scale);
	float TitleHDiff = (10.0f * BackHScale * _scale);
	float TitleWScale = ((244.0f-10.0f) * BackWScale) / TextBar_width;
	float TitleHScale = 1.0f;
	HGOTextBar* Title = _CreateTextBar(
		_title, 
		_scale * TitleWScale, 
		_scale * TitleHScale);

	_AttachObject1(this, Title);

	Title->Move(TitleWDiff, TitleHDiff);
	Title->SetAcceptAction(false);


	////////////////////////////////////////////////////////////////////////////
	
	float ListViewerWDiff = (10.0f * BackWScale * _scale);
	float ListViewerHDiff = (TextBar_height * TitleHScale * _scale) + TitleHDiff;
	float ListViewerWScale = ((244.0f - 10.0f) * BackWScale) / ListViewer_width;
	float ListViewerHScale = 0.9f;
	ListViewer = _CreateListViewer(
		_scale * ListViewerWScale, 
		_scale * ListViewerHScale,
		_duplicates);

	_AttachObject1(this, ListViewer);

	ListViewer->Move(ListViewerWDiff, ListViewerHDiff);

	ListViewer->SetBorderWidth(6.0f);
	ListViewer->SetBorderHeight(6.0f);


	////////////////////////////////////////////////////////////////////////////
	
	ButtonOK = _CreateButton(
		"  OK  ", 
		_scale);

	_AttachObject1(this, ButtonOK);

	ButtonOK->Move(
		(Width / 4.0f) - (ButtonOK->Width / 2.0f),
		Height - ButtonOK->Height - (10.0f * BackHScale * _scale));


	////////////////////////////////////////////////////////////////////////////
	
	ButtonCancel = _CreateButton(
		"Cancel", 
		_scale);

	_AttachObject1(this, ButtonCancel);

	ButtonCancel->Move(
		3.0f * (Width / 4.0f) - (ButtonCancel->Width / 2.0f),
		Height - ButtonCancel->Height - (10.0f * BackHScale * _scale));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGODefaultList::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	HGOImage::Display(true);

	_ActivateFull2(Attachments, Active);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGOTextBar : public HGOTextOverImage




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOTextBar::HGOTextBar(char* _text, float _wscale, float _hscale)
 : HGOTextOverImage(_text)
{
	Setup();

	RECT _rect;

	// load list.bmp from resource
	IDirect3DTexture8* ListTexture;
	D3DXCreateTextureFromResourceEx(_D3DDevice(), hInst, MAKEINTRESOURCE(BMP_LIST), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_RED, NULL, NULL, &ListTexture);
	IDDTexture* _texture = _CreateIDDTexture(512.0f, 512.0f, ListTexture);

	SetRect(&_rect, TextBar_left, TextBar_top, TextBar_right, TextBar_bottom);
	SetTexture(
		_wscale,
		_hscale,  
		_texture, 
		&_rect);

	SetBorderWidth(6.0f);
	SetBorderHeight(6.0f);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextBar::Setup()
{
	TextColor = COLOR_DEFAULT;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOTextBar::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	HGOTextOverImage::Display(true); 

	float FontWScale = (float)MaxWidth / (float)strlen(Text);
	float FontHScale = (Height - (BorderHeight * 2.0f)) / _FontHeight();

	float TextScale = FontWScale;

	if (FontWScale > FontHScale)
		TextScale = FontHScale;

	float FontWidth = TextScale * _FontWidth();
	float FontHeight = TextScale * _FontHeight();

	float WordX = _X2W(X) + ((Width - (FontWidth * strlen(Text))) / 2.0f);
	float WordY = _Y2H(Y) + ((Height - FontHeight) / 2.0f);

	_DrawText(Text, WordX, WordY, TextScale, TextColor);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGOButton : public HGOTextBar




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOButton::HGOButton(char* _text, float _scale)
 : HGOTextBar(_text, -1.0f, -1.0f)
{	
	RECT _rect;

	// load list.bmp from resource
	IDirect3DTexture8* ListTexture;
	D3DXCreateTextureFromResourceEx(_D3DDevice(), hInst, MAKEINTRESOURCE(BMP_LIST), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_RED, NULL, NULL, &ListTexture);
	IDDTexture* _texture = _CreateIDDTexture(512.0f, 512.0f, ListTexture);

	SetRect(&_rect, Button_left, Button_top, Button_right, Button_bottom);
	SetTexture(
		_scale,
		_scale,  
		_texture, 
		&_rect);

	SetBorderWidth(15.0f);
	SetBorderHeight(0.0f);

	SetStickyIO(false);
	IdleColor = COLOR_BLUE;
	ActiveColor = COLOR_RED;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOButton::ReScale(float _wscale, float _hscale)
{
	HGOTextOverImage::ReScale(_wscale, _hscale);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOButton::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	TextColor = IdleColor;

	if (GetIO()) TextColor = ActiveColor;

	HGOTextBar::Display(true);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
//class HGOScrollBar : public HGOImage




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOScrollBar::~HGOScrollBar()
{
	if (LeftUpImage)
	{
		delete LeftUpImage;
		LeftUpImage = NULL;
	}

	if (RightDownImage)
	{
		delete RightDownImage;
		RightDownImage = NULL;
	}

	if (BackSlideImage)
	{
		delete BackSlideImage;
		BackSlideImage = NULL;
	}

	if (FrontSlideImage)
	{
		delete FrontSlideImage;
		FrontSlideImage = NULL;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOScrollBar::HGOScrollBar(HGOListViewer* _parent, UINT _orientation, float _width, float _height)
 : HGObj(_width, _height)
{
	Parent = _parent;

	Orientation = _orientation;

	RECT _rect;

	// load list.bmp from resource
	IDirect3DTexture8* ListTexture;
	D3DXCreateTextureFromResourceEx(_D3DDevice(), hInst, MAKEINTRESOURCE(BMP_LIST), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_RED, NULL, NULL, &ListTexture);
	IDDTexture* _texture = _CreateIDDTexture(512.0f, 512.0f, ListTexture);

	if (Orientation == HORIZONTAL)
	{
		float _sbScale = 1.0f;
		float _minWidth = (float)(HSB_LI_width + HSB_RI_width + HSB_MI_width);
		if (Width < _minWidth)
		{
			_sbScale = Width / _minWidth;
		}

		SetRect(&_rect, HSB_LI_left, HSB_LI_top, HSB_LI_right, HSB_LI_bottom);
		LeftUpImage = _CreateImage( 
			_sbScale, 
			1.0f, 
			_texture, 
			&_rect);

		_AttachObject1(this, LeftUpImage);

		///

		SetRect(&_rect, HSB_RI_left, HSB_RI_top, HSB_RI_right, HSB_RI_bottom);
		RightDownImage = _CreateImage(
			_sbScale, 
			1.0f, 
			_texture, 
			&_rect);

		_AttachObject1(this, RightDownImage);

		RightDownImage->Move(Width - RightDownImage->Width, 0.0f);

		///

		SetRect(&_rect, HSB_MI_left, HSB_MI_top, HSB_MI_right, HSB_MI_bottom);
		BackSlideImage = _CreateImage( 
			(Width - LeftUpImage->Width - RightDownImage->Width) / HSB_MI_width,
			1.0f, 
			_texture, 
			&_rect);

		_AttachObject1(this, BackSlideImage);

		BackSlideImage->Move(LeftUpImage->Width, 0.0f);
	}
	else
	{
		float _sbScale = 1.0f;
		float _minHeight = (float)(VSB_UI_height + VSB_DI_height + VSB_MI_height);
		if (Height < _minHeight)
		{
			_sbScale = Height / _minHeight;
		}

		SetRect(&_rect, VSB_UI_left, VSB_UI_top, VSB_UI_right, VSB_UI_bottom);
		LeftUpImage = _CreateImage( 
			1.0f, 
			_sbScale, 
			_texture, 
			&_rect);

		_AttachObject1(this, LeftUpImage);

		///

		SetRect(&_rect, VSB_DI_left, VSB_DI_top, VSB_DI_right, VSB_DI_bottom);
		RightDownImage = _CreateImage(
			1.0f, 
			_sbScale, 
			_texture, 
			&_rect);

		_AttachObject1(this, RightDownImage);

		RightDownImage->Move(0.0f, Height - RightDownImage->Height);

		///

		SetRect(&_rect, VSB_MI_left, VSB_MI_top, VSB_MI_right, VSB_MI_bottom);
		BackSlideImage = _CreateImage( 
			1.0f,
			(Height - LeftUpImage->Height - RightDownImage->Height) / VSB_MI_height, 
			_texture, 
			&_rect);

		_AttachObject1(this, BackSlideImage);

		BackSlideImage->Move(0.0f, LeftUpImage->Height);
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOScrollBar::ReScale()
{
	if (Orientation == HORIZONTAL)
	{
		float _wlist = Parent->Width - VSB_MI_width;

		float _sbScale = 1.0f;
		float _minWidth = (float)(HSB_LI_width + HSB_RI_width + HSB_MI_width);
		if (_wlist < _minWidth)
		{
			_sbScale = _wlist / _minWidth;
		}

		RightDownImage->Move(_wlist - Width, 0.0f);

		Width = _wlist;

		LeftUpImage->ReScale(
			_sbScale * LeftUpImage->GetWidthScale(),
			LeftUpImage->GetHeightScale());

		RightDownImage->ReScale(
			_sbScale * RightDownImage->GetWidthScale(),
			RightDownImage->GetHeightScale());

		RightDownImage->Move((RightDownImage->Width / _sbScale) - RightDownImage->Width, 0.0f);

		BackSlideImage->ReScale(
			(Width - LeftUpImage->Width - RightDownImage->Width) / HSB_MI_width,
			BackSlideImage->GetHeightScale());

		BackSlideImage->Move(LeftUpImage->Width - (LeftUpImage->Width / _sbScale), 0.0f);
	}
	else
	{
		float _hlist = Parent->Height - HSB_MI_height;

		float _sbScale = 1.0f;
		float _minHeight = (float)(VSB_UI_height + VSB_DI_height + VSB_MI_height);
		if (_hlist < _minHeight)
		{
			_sbScale = _hlist / _minHeight;
		}

		RightDownImage->Move(0.0f, _hlist - Height);

		Height = _hlist;

		LeftUpImage->ReScale(
			LeftUpImage->GetWidthScale(),
			_sbScale * LeftUpImage->GetHeightScale());
		
		RightDownImage->ReScale(
			RightDownImage->GetWidthScale(),
			_sbScale * RightDownImage->GetHeightScale());

		RightDownImage->Move(0.0f, (RightDownImage->Height / _sbScale) - RightDownImage->Height);

		BackSlideImage->ReScale(
			BackSlideImage->GetWidthScale(),
			(Height - LeftUpImage->Height - RightDownImage->Height) / VSB_MI_height);

		BackSlideImage->Move(0.0f, LeftUpImage->Height - (LeftUpImage->Height / _sbScale));
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGOScrollBar::Display(bool _ignore)
{
	if (Drawn && !_ignore) return;
	else Drawn = true;

	HGObj::Display(true);

	if (Orientation == HORIZONTAL)
	{
		if (Parent && LeftUpImage && LeftUpImage->GetIO())
			if (!_Mouse().LButtonDown)
				LeftUpImage->SetIO(false);
			else
				Parent->ScrollLeft();


		if (Parent && RightDownImage && RightDownImage->GetIO())
			if (!_Mouse().LButtonDown)
				RightDownImage->SetIO(false);
			else
				Parent->ScrollRight();
	}
	else
	{
		if (Parent && LeftUpImage && LeftUpImage->GetIO())
			if (!_Mouse().LButtonDown)
				LeftUpImage->SetIO(false);
			else
				Parent->ScrollUp();


		if (Parent && RightDownImage && RightDownImage->GetIO())
			if (!_Mouse().LButtonDown)
				RightDownImage->SetIO(false);
			else
				Parent->ScrollDown();
	}

	_ActivateFull2(Attachments, Active);
}

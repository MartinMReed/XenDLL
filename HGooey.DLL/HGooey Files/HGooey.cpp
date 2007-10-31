#define HGOOEY_EXPORTS
#include "HGIncDef_Private.h"

HINSTANCE hInst;


//-------------------------------------------------------
// 
//-------------------------------------------------------
BOOL APIENTRY DllMain(HINSTANCE hInstance, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				hInst = hInstance;
				if (!g_HGooey)
					g_HGooey = new HGooey();
			}
			break;
		case DLL_THREAD_ATTACH:
			{
			}
			break;
		case DLL_THREAD_DETACH:
			{
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				if (g_HGooey)
				{
					delete g_HGooey;
					g_HGooey = NULL;
				}
			}
			break;
		
    }

    return TRUE;
}



char* TypeChar[8][8] = {{ "A",  "B", "C", "D",  "E", "F", "G", "H" },
						{ "I",  "J", "K", "L",  "M", "N", "O", "P" },
						{ "Q",  "R", "S", "T",  "U", "V", "W", "X" },
						{ "Y",  "Z", "!", "\"", "#", "$", "%", "&" },
						{ "\'", "(", ")", "*",  "+", ",", "-", "." },
						{ "/",  "0", "1", "2",  "3", "4", "5", "6" },
						{ "7",  "8", "9", ":",  ";", "<", "=", ">" },
						{ "?",  "@", "[", "\\", "]", "^", "_", " " } };




//-------------------------------------------------------
// 
//-------------------------------------------------------
const char* HGooey::ReadConfigItem(char* _item, char* _filename)
{
	if (!_item || !_filename)
		return NULL;

	if (GetFileAttributes(_filename) == 0xFFFFFFFF) return NULL;

	TiXmlDocument document;

	if (!document.LoadFile(_filename)) return NULL;

	TiXmlElement* root;
	int version;

	if ((root = document.FirstChildElement("config")) == NULL ||
		root->Attribute("version", &version) == NULL || version != XML_VERSION) return NULL;

	TiXmlElement* element = root->FirstChildElement(_item);

	if (element)
	{
		return element->GetText();
	}

	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::WriteConfigItem(char* _item, char* _data, char* _filename)
{
	if (!_item || !_data || !_filename)
		return false;

	TiXmlDocument document;

	bool insertRoot = false;
	TiXmlElement* root = NULL;
	int version;

	if (document.LoadFile(_filename) == false || (root = document.FirstChildElement("config")) == NULL ||
		root->Attribute("version", &version) == NULL || version != XML_VERSION)
	{
		document.Clear();
		insertRoot = true;
		version = XML_VERSION;
		root = new TiXmlElement("config");
		root->SetAttribute("version", version);
	}

	FILE* out = NULL;
	fopen_s(&out, _filename, "w" );

	if (out)
	{
		bool insertElement = false;
		TiXmlElement* element = root->FirstChildElement(_item);

		if (element == NULL)
		{
			insertElement = true;
			element = new TiXmlElement(_item);
		}

		element->Clear();
		element->InsertEndChild(*(new TiXmlText(_data)));
		
		if (insertElement)
		{
			root->InsertEndChild(*element);
		}

		if (insertRoot)
		{
			document.InsertEndChild(*root);
		}

		document.Print(out, 0);

		fclose(out);
	}

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
RECT HGooey::Char2Rect(char _char)
{
	for (int i = 0; i < 8; i++)
		for (int n = 0; n < 8; n++)
			if (_stricmp(TypeChar[i][n], &_char) == 0)
				return FontChar[i][n];

	return FontChar[4][7];
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::DrawText(char* _str, float _x, float _y, float _scale, UINT _color, bool _vertical, RECT* _trim)
{
	if (!DrawEnabled) return;

	if (!_str || !h_oFontTexture)
		return;

	if (_scale * FontWidth == 0.0f || _scale * FontHeight == 0.0f)
		return;

	_x = _W2X(_x);
	_y = _H2Y(_y);

	int _length = strlen(_str);

	for (int i = 0; i < _length; i++)
	{
		RECT _rect = Char2Rect(_str[i]);

		if (_trim)
		{
			_rect.left -= _trim->left;
			_rect.top -= _trim->top;
			_rect.right -= _trim->right;
			_rect.bottom -= _trim->bottom;
		}

		if (!_vertical)
		{
			RenderSprite(_x + (i * _W2X(FontWidth) * _scale), _y, 
				h_oFontTexture->Width * _scale, h_oFontTexture->Height * _scale, 
				h_oFontTexture->Width, h_oFontTexture->Height, h_oFontTexture->Texture, 
				_color, &_rect);
		}
		else
		{
			RenderSprite(_x, _y + (i * _H2Y(FontHeight) * _scale), 
				h_oFontTexture->Width * _scale, h_oFontTexture->Height * _scale, 
				h_oFontTexture->Width, h_oFontTexture->Height, h_oFontTexture->Texture, 
				_color, &_rect);
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
IDDTexture* HGooey::CreateIDDTextureFromFile(float _width, float _height, char* _filename, UINT _color)
{
	if (!_filename || GetFileAttributes(_filename) == 0xFFFFFFFF)
		return NULL;

	IDirect3DTexture8* Texture;

	D3DXCreateTextureFromFileEx(h_pD3DDevice, _filename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, _color, NULL, NULL, &Texture);

	IDDTexture* TReturn = new IDDTexture(_width, _height, Texture, _filename, _color);

	if (!h_oHGTextureList)
		h_oHGTextureList = new HGNodeList();

	h_oHGTextureList->LoadNode(new HGNode(TReturn));

	return TReturn;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
IDDTexture* HGooey::CreateIDDTexture(float _width, float _height, IDirect3DTexture8* _texture)
{
	if (!_texture)
		return NULL;

	IDDTexture* TReturn = new IDDTexture(_width, _height, _texture);

	if (!h_oHGTextureList)
		h_oHGTextureList = new HGNodeList();

	h_oHGTextureList->LoadNode(new HGNode(TReturn));

	return TReturn;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::SetMenuOptions(int _position, float _iwidth, float _iheight)
{
	if (h_oHGMenu)
		return false;

	HGOButton* Button = CreateButton(" Menu ", 0.80f);
	Button->SetStickyIO(true);

	Button->X = _W2X(h_iScreenWidth - Button->Width);
	Button->Y = _H2Y(h_iScreenHeight - Button->Height);

	if (_position == TOP_LEFT || _position == TOP_RIGHT)
		Button->Y = 0.0f;
	
	if (_position == TOP_LEFT || _position == BOTTOM_LEFT)
		Button->X = 0.0f;

	h_oHGMenu = new HGMenu(Button, _position, _iwidth, _iheight);

	return h_oHGMenu != NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::ToggleMenu()
{
	if (!h_oHGMenu->StartButton)
		return false;

	bool active = !(h_oHGMenu->StartButton)->Active;

	(h_oHGMenu->StartButton)->Active = active;

	if (!active)
	{
		if (h_oHGMenu->MoveControl)
		{
			h_bMoveActive = false;
			(h_oHGMenu->MoveControl)->SetIO(false);
		}

		(h_oHGMenu->StartButton)->SetIO(false);
	}

	return active;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::ResetAllDrawn()
{
	HGObj* Obj;
	
	if (h_oHGExtList)
	{
		HGExtNode* ExtNode = (HGExtNode*)(h_oHGExtList->Head);

		while (ExtNode)
		{
			HGNodeList* Extension = ExtNode->Ext;

			if (Extension)
			{
				HGNode* ObjNode = Extension->Head;

				while (ObjNode)
				{
					Obj = (HGObj*)(ObjNode->Element);
					Obj->ResetDrawn();

					ObjNode = ObjNode->Next;
				}
			}

			ExtNode = (HGExtNode*)(ExtNode->Next);
		}
	}

	if (h_oHGMenu)
	{
		HGObj* StartButton = h_oHGMenu->StartButton;

		if (StartButton)
			StartButton->ResetDrawn();

		if (!h_oHGMenu->ExtButtons || h_oHGMenu->ItemCount <= 0)
			return;

		HGNode* ObjNode = (h_oHGMenu->ExtButtons)->Head;

		while (ObjNode)
		{
			Obj = (HGObj*)(ObjNode->Element);

			if (Obj)
			{
				Obj->ResetDrawn();
			}

			ObjNode = ObjNode->Next;
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::DrawAll()
{
	if (!DrawEnabled) return;

	ResetAllDrawn();

	HGObj* Obj;
	
	if (h_oHGExtList)
	{
		HGExtNode* ExtNode = (HGExtNode*)(h_oHGExtList->Head);

		while (ExtNode)
		{
			HGNodeList* Extension = ExtNode->Ext;

			if (Extension)
			{
				HGNode* ObjNode = Extension->Head;

				// extension object list
				while (ObjNode)
				{
					DrawObject(false, (HGObj*)(ObjNode->Element));
					ObjNode = ObjNode->Next;
				}
			}

			ExtNode = (HGExtNode*)(ExtNode->Next);
		}
	}

	if (h_oHGMenu)
	{
		HGObj* StartButton = h_oHGMenu->StartButton;

		// draw menu "start" button
		if (StartButton)
			DrawObject(false, StartButton);

		h_oHGMenu->ActivateItems(false);

		// if menu has been opened
		if (!StartButton->GetIO())
			return;

		h_oHGMenu->ActivateItems(true);

		if (!h_oHGMenu->ExtButtons || h_oHGMenu->ItemCount <= 0)
			return;

		int NodeID = 0;

		int iGridHeight = (int)ceil(sqrt(h_oHGMenu->ItemCount + (float)NodeID));

		HGNode* ObjNode = (h_oHGMenu->ExtButtons)->Head;

		while (ObjNode)
		{
			Obj = (HGObj*)(ObjNode->Element);

			if (Obj)
			{
				// find vertical slot for item placement
				int vSlot;
				for (vSlot = NodeID + 1; vSlot > iGridHeight; vSlot -= iGridHeight);
					
				// update x/y coords
				if (h_oHGMenu->Position == TOP_LEFT || h_oHGMenu->Position == BOTTOM_LEFT)
					Obj->X = StartButton->X + (floor(NodeID / (float)iGridHeight) * _W2X(Obj->Width));
				else
					Obj->X = StartButton->X + _W2X((StartButton->Width - Obj->Width)) - (floor(NodeID / (float)iGridHeight) * _W2X(Obj->Width));
				
				if (h_oHGMenu->Position == TOP_LEFT || h_oHGMenu->Position == TOP_RIGHT)
					Obj->Y = StartButton->Y + _H2Y(StartButton->Height) + ((vSlot - 1) * _H2Y(Obj->Height));
				else
					Obj->Y = StartButton->Y - (vSlot * _H2Y(Obj->Height));

				// draw the menu item
				DrawObject(Obj->GetIO(), Obj);
			}

			ObjNode = ObjNode->Next;
			NodeID++;
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::DrawIDDTexture(float _width, float _height, float _x, float _y, IDDTexture* _texture, RECT* _rect)
{
	if (!DrawEnabled) return;

	if (!_texture || !_texture->Texture || _width <= 0.0f || _height <= 0.0f)
		return;

	RenderSprite(_x, _y, 
		_width, _height, 
		_texture->Width, _texture->Height, 
		_texture->Texture, COLOR_DRAW, _rect);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::DrawObject(bool _highlight, HGObj* _obj)
{
	if (!DrawEnabled) return;

	if (!_obj)
		return;

	if (_obj->Active)
	{
		if (_highlight)
			COLOR_DRAW = COLOR_HGHLGHT;
		else
			COLOR_DRAW = COLOR_DEFAULT;

		_obj->Display();
	}

	HGNodeList* Attachment = _obj->Attachments;

	if (Attachment)
	{
		HGNode* ObjNode = Attachment->Head;

		while (ObjNode)
		{
			DrawObject(_highlight, (HGObj*)(ObjNode->Element));
			ObjNode = ObjNode->Next;
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj* HGooey::ActionObject(HGObj* _obj)
{
	if (_obj && _obj->Active)
	{
		if (_obj->RemoteAction)
			_obj->RemoteAction();
	}

	return _obj;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGNodeList* HGooey::CreateExtension(IDDTexture* _texture, RECT* _rect)
{
	if (!h_oHGExtList)
		h_oHGExtList = new HGNodeList();

	HGExtNode* ExtNode;

	if (_texture && h_oHGMenu)
		ExtNode = new HGExtNode(h_oHGMenu->AddMenuItem(_texture, _rect));
	else
		ExtNode = new HGExtNode();

	h_oHGExtList->LoadNode(ExtNode, true);

	return ExtNode->Ext;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::SetExtensionMenuItem(HGNodeList* _ext, IDDTexture* _texture, RECT* _rect)
{
	if (!_ext || !h_oHGExtList || !_texture)
		return false;

	HGExtNode* ExtNode = h_oHGExtList->Find(_ext);

	if (ExtNode)
	{
		ExtNode->Element = h_oHGMenu->AddMenuItem(_texture, _rect);
	}

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::UnloadExtension(HGNodeList* _ext)
{
	if (!_ext || !h_oHGExtList)
		return false;

	HGExtNode* ExtNode = h_oHGExtList->Find(_ext);

	if (!ExtNode)
		return false;

	if (h_oHGMenu)
		h_oHGMenu->RemoveMenuItem((HGObj*)(ExtNode->Element));

	return h_oHGExtList->UnloadNode(ExtNode, true);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::CheckExtMenuIO(HGNodeList* _ext)
{
	if (!_ext || !h_oHGExtList)
		return false;

	HGExtNode* ExtNode = h_oHGExtList->Find(_ext);

	if (ExtNode)
	{
		HGObj* Obj = (HGObj*)(ExtNode->Element);

		if (Obj)
			return Obj->GetIO();
	}

	return false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::SetExtMenuIO(HGNodeList* _ext, bool _io)
{
	if (!_ext || !h_oHGExtList)
		return;

	HGExtNode* ExtNode = h_oHGExtList->Find(_ext);

	if (ExtNode)
	{
		HGObj* Obj = (HGObj*)(ExtNode->Element);

		Obj->SetIO(_io);
	}

	return;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOImage* HGooey::CreateImage(float _wscale, float _hscale, IDDTexture* _texture, RECT* _rect)
{
	if (!_texture)
		return NULL;

	return (HGOImage*)UnAttached(new HGOImage(_wscale, _hscale, _texture, _rect));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOTextBar* HGooey::CreateTextBar(char* _text, float _wscale, float _hscale)
{
	return (HGOTextBar*)UnAttached(new HGOTextBar(_text, _wscale, _hscale));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOTextOverImage* HGooey::CreateTextOverImage(char* _text)
{
	return (HGOTextOverImage*)UnAttached(new HGOTextOverImage(_text));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj* HGooey::CreateAreaSelection(float _width, float _height)
{
	return (HGObj*)UnAttached(new HGObj(_width, _height));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOListViewer* HGooey::CreateListViewer(float _wscale, float _hscale, bool _duplicates, float _tscale)
{
	return (HGOListViewer*)UnAttached(new HGOListViewer(_wscale, _hscale, _duplicates, _tscale));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGODefaultList* HGooey::CreateDefaultList(char* _title, float _scale, bool _duplicates)
{
	return (HGODefaultList*)UnAttached(new HGODefaultList(_title, _scale, _duplicates));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOButton* HGooey::CreateButton(char* _text, float _scale)
{
	return (HGOButton*)UnAttached(new HGOButton(_text, _scale));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGOScrollBar* HGooey::CreateScrollBar(HGOListViewer* _parent, UINT _orientation, float _width, float _height)
{
	return (HGOScrollBar*)UnAttached(new HGOScrollBar(_parent, _orientation, _width, _height));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj* HGooey::UnAttached(HGObj* _obj)
{
	if (_obj)
	{
		if (!h_oHGUnAttachedList)
			h_oHGUnAttachedList = new HGNodeList();

		h_oHGUnAttachedList->LoadNode(new HGNode(_obj));

		_obj->AlreadyAttached = false;
	}

	return _obj;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj* HGooey::Attached(HGObj* _obj)
{
	if (_obj)
	{
		if (h_oHGUnAttachedList)
			h_oHGUnAttachedList->UnloadNode(h_oHGUnAttachedList->Find(_obj), false);
		
		_obj->AlreadyAttached = true;
	}

	return _obj;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::AttachObject(HGNodeList* _parent, HGObj* _obj)
{
	if (!_parent || !_obj || _obj->AlreadyAttached)
		return false;

	return _parent->LoadNode(new HGNode(Attached(_obj)));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::AttachObject(HGObj* _parent, HGObj* _obj)
{
	if (!_parent || !_parent->Attachments || !_obj || _obj->AlreadyAttached)
		return false;

	_obj->MoveByCoord(_obj->X + (_parent->X - _obj->X), _obj->Y + (_parent->Y - _obj->Y));
	
	return (_parent->Attachments)->LoadNode(new HGNode(Attached(_obj)));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::ActivateFull(HGObj* _obj, bool _active)
{
	if (!_obj)
		return;

	_obj->Active = _active;
	ActivateFull(_obj->Attachments, _active);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::ActivateFull(HGNodeList* _list, bool _active)
{
	if (!_list)
		return;

	HGNode* ObjNode = _list->Head;

	while (ObjNode)
	{
		HGObj* Obj = (HGObj*)(ObjNode->Element);

		if (Obj)
			ActivateFull(Obj, _active);

		ObjNode = ObjNode->Next;
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj* HGooey::ActionPerformed()
{
	HGObj* Obj;

	if (h_oHGMenu)
	{
		Obj = h_oHGMenu->StartButton;

		if (ObjEvent(Obj))
		{
			return Obj;
		}

		Obj = h_oHGMenu->MoveControl;

		if (ObjEvent(Obj))
		{
			h_bMoveActive = !h_bMoveActive;
			return Obj;
		}
	}

	if (h_oHGExtList)
	{
		HGExtNode* ExtNode = (HGExtNode*)(h_oHGExtList->Head);

		while (ExtNode)
		{
			HGObj* Obj = (HGObj*)(ExtNode->Element);

			// extension menu button
			if (ObjEvent(Obj))
				return Obj;

			ExtNode = (HGExtNode*)(ExtNode->Next);
		}

		ExtNode = (HGExtNode*)(h_oHGExtList->Head);

		HGObj* ObjReturn = NULL;

		while (ExtNode)
		{
			HGNodeList* Extension = ExtNode->Ext;

			if (Extension)
			{
				HGNode* ObjNode = Extension->Head;

				// extension object list
				while (ObjNode)
				{
					HGObj* Obj = (HGObj*)(ObjNode->Element);

					if (ObjEvent(Obj))
						ObjReturn = Obj;

					HGObj* Check = CheckListAction(Obj);

					if (Check)
						ObjReturn = Check;

					ObjNode = ObjNode->Next;
				}
			}

			ExtNode = (HGExtNode*)(ExtNode->Next);
		}

		if (ObjReturn)
			return ObjReturn;
	}

	return NULL;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGObj* HGooey::CheckListAction(HGObj* _obj)
{
	HGObj* ObjReturn = NULL;

	if (!_obj)
		return ObjReturn;

	HGNodeList* Attachment = _obj->Attachments;

	if (Attachment)
	{
		HGNode* ObjNode = Attachment->Head;

		while (ObjNode)
		{
			HGObj* Obj = (HGObj*)(ObjNode->Element);

			if (ObjEvent(Obj))
				ObjReturn = Obj;

			HGObj* Check = CheckListAction(Obj);

			if (Check)
				ObjReturn = Check;

			ObjNode = ObjNode->Next;
		}
	}

	return ObjReturn;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::ObjEvent(HGObj* _obj)
{
	return (_obj && 
		_obj->Active && 
		_obj->GetAcceptAction() && 
		CursorOver(_obj->X, _obj->Y, _W2X(_obj->Width), _H2Y(_obj->Height)));
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::CursorOver(float _x, float _y, float _xdifference, float _ydifference)
{
	return xMouse.CursorX >= _x && xMouse.CursorX <= _x + _xdifference && 
		xMouse.CursorY >= _y && xMouse.CursorY <= _y + _ydifference;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGooey::HGooey()
{
	h_oCursorTexture = NULL;
	h_oFontTexture = NULL;
	h_pSprite = NULL;
	h_pD3DDevice = NULL;

	h_oHGMenu = NULL; // menu button, and list of menu items
	h_oHGExtList = NULL; // list of extensions created
	h_oHGUnAttachedList = NULL; // objects created, but not yet attached to anything
	h_oHGTextureList = NULL; // list of textures that have been loaded
	h_iObjClicked = NULL; // object (id) clicked on
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
HGooey::~HGooey()
{
	if (h_oHGMenu)
	{
		delete h_oHGMenu;
		h_oHGMenu = NULL;
	}

	if (h_oHGTextureList)
	{
		delete h_oHGTextureList;
		h_oHGTextureList = NULL;
	}

	if (h_oHGUnAttachedList)
	{
		delete h_oHGUnAttachedList;
		h_oHGUnAttachedList = NULL;
	}

	if (h_oHGExtList)
	{
		delete h_oHGExtList;
		h_oHGExtList = NULL;
	}

	h_iObjClicked = NULL;

	if (h_pSprite)
	{
		h_pSprite->Release();
		h_pSprite = NULL;
	}
}

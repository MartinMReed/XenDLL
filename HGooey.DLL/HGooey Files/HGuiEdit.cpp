#define HGOOEY_EXPORTS
#include "HGIncDef_Private.h"




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::Initialize(IDirect3DDevice8* _device) 
{
	if (!_device)
		return false;

	h_pD3DDevice = _device;

	xMouse.cState = 0;
	xMouse.LButtonDown = false;
	xMouse.MButtonDown = false;
	xMouse.RButtonDown = false;
	xMouse.CursorX = 0.0f;
	xMouse.CursorY = 0.0f;

	xKey.bDown = false;
	xKey.bRepeat = false;
	xKey.cKey = 0x00;

	D3DXCreateSprite(h_pD3DDevice, &h_pSprite);

	D3DVIEWPORT8 Viewport;
	h_pD3DDevice->GetViewport(&Viewport);
	h_fGuiScaleX = (float)Viewport.Width / 1024.0f;
	h_fGuiScaleY = (float)Viewport.Height / 768.0f;
	h_iScreenWidth = (float)Viewport.Width / h_fGuiScaleX;
	h_iScreenHeight = (float)Viewport.Height / h_fGuiScaleY;


	////////////////////////////////////////////////
	/// GUI SYSTEM
	////////////////////////////////////////////////
	DrawEnabled = true;

	h_bMoveActive = false; // currently moving an object

	// load cursor.bmp from resource
	IDirect3DTexture8* CursorTexture;
	D3DXCreateTextureFromResourceEx(h_pD3DDevice, hInst, MAKEINTRESOURCE(BMP_CURSOR), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_BLACK, NULL, NULL, &CursorTexture);
	h_oCursorTexture = CreateIDDTexture(128.0f, 128.0f, CursorTexture);

	// load font.bmp from resource
	IDirect3DTexture8* FontTexture;
	D3DXCreateTextureFromResourceEx(h_pD3DDevice, hInst, MAKEINTRESOURCE(BMP_FONT), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, COLOR_BLACK, NULL, NULL, &FontTexture);
	h_oFontTexture = CreateIDDTexture(128.0f, 128.0f, FontTexture);

	FontHeight = 16.0f;
	FontWidth = 16.0f;

	for (int i = 0; i < 8; i++)
		for (int n = 0; n < 8; n++)
			SetRect(&FontChar[i][n], n * (int)FontWidth, i * (int)FontHeight, (n + 1) * (int)FontWidth, (i + 1) * (int)FontHeight);

	////////////////////////////////////////////////

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::LostDevice(void)
{
	if (h_oHGTextureList)
	{
		HGNode* Node = (HGNode*)(h_oHGTextureList->Head);

		while (Node)
		{
			IDDTexture* Element = (IDDTexture*)(Node->Element);

			if(Element && ((IDDTexture*)Element)->Filename)
			{
				IDirect3DTexture8* Texture = ((IDDTexture*)Element)->Texture;
				
				if (Texture)
				{
					Texture->Release();
					Texture = NULL;
				}
			}

			Node = (HGNode*)(Node->Next);
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::ResetDevice(void)
{
	D3DXCreateSprite(h_pD3DDevice, &h_pSprite);

	D3DVIEWPORT8 Viewport;
	h_pD3DDevice->GetViewport(&Viewport);
	h_fGuiScaleX = (float)Viewport.Width / 1024.0f;
	h_fGuiScaleY = (float)Viewport.Height / 768.0f;
	h_iScreenWidth = (float)Viewport.Width / h_fGuiScaleX;
	h_iScreenHeight = (float)Viewport.Height / h_fGuiScaleY;

	if (h_oHGTextureList)
	{
		HGNode* Node = (HGNode*)(h_oHGTextureList->Head);

		while (Node)
		{
			IDDTexture* Element = (IDDTexture*)(Node->Element);

			if(Element && ((IDDTexture*)Element)->Filename)
			{
				D3DXCreateTextureFromFileEx(h_pD3DDevice, ((IDDTexture*)Element)->Filename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
					0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, ((IDDTexture*)Element)->Color, NULL, NULL, &((IDDTexture*)Element)->Texture);
			}

			Node = (HGNode*)(Node->Next);
		}
	}
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::EndScene(float _cursorX, float _cursorY) 
{
	float _oldX = xMouse.CursorX;
	float _oldY = xMouse.CursorY;

	if (_cursorX == -1 || _cursorY == -1)
	{
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		xMouse.CursorX = (float)ptCursor.x;
		xMouse.CursorY = (float)ptCursor.y;
	}
	else
	{
		xMouse.CursorX = _cursorX;
		xMouse.CursorY = _cursorY;
	}


	////////////////////////////////////////////////
	/// GUI SYSTEM
	////////////////////////////////////////////////
	if (h_iObjClicked && ((h_bMoveActive && h_iObjClicked->Moveable == MOVE_LIMITED) || (h_iObjClicked->Moveable == MOVE_ALWAYS)))
		h_iObjClicked->MoveByCoord(
		xMouse.CursorX - _oldX, 
		xMouse.CursorY - _oldY);

	DrawAll();

	// redraw the cursor on top of everything else
	if (h_oCursorTexture)
		RenderSprite(
		xMouse.CursorX, 
		xMouse.CursorY, 
		85.0f, 
		85.0f, 
		h_oCursorTexture->Width, 
		h_oCursorTexture->Height, 
		h_oCursorTexture->Texture, 
		COLOR_DEFAULT);
	////////////////////////////////////////////////
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void HGooey::RenderSprite(float X, float Y, float Width, float Height,
							 float U, float V,							// texture width/height
							 IDirect3DTexture8* pTexture,
							 D3DCOLOR nColor, RECT* rTexture,
							 bool bCenteredX, bool bCenteredY)	// optional
{
	if (!pTexture)
		return;
	
	if (bCenteredX)
		X = _W2X((h_iScreenWidth - Width) / 2.0f);
	
	if (bCenteredY)
		Y =  _H2Y((h_iScreenHeight - Height) / 2.0f);
	

	D3DXVECTOR2* SpriteTranslation = new D3DXVECTOR2();
	SpriteTranslation->x = X;
	SpriteTranslation->y = Y;
	
	D3DXVECTOR2* SpriteScale = new D3DXVECTOR2();
	SpriteScale->x = _W2X(Width) / U;
	SpriteScale->y = _H2Y(Height) / V;
	
	h_pSprite->Begin();
	
	h_pSprite->Draw(pTexture, rTexture,
		SpriteScale, NULL, 0.0f,
		SpriteTranslation, nColor);
	
	h_pSprite->End();

	delete SpriteTranslation;
	delete SpriteScale;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::HandleKeyboard(BYTE cKey, char* buffer, bool bDown, bool bRepeat)
{
	xKey.bDown = bDown;
	xKey.bRepeat = bRepeat;
	xKey.cKey = cKey;

	if (h_bMoveActive) return false;

	if (bDown)
	{
	}
	else if (!bRepeat)
	{
	}

	return true;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool HGooey::HandleMouse(int cState, DIMOUSESTATE2* mouse)
{
	xMouse.cState = cState;

	if (cState == 0) // mouse move
	{
	}
	else if (cState == 1) // lbutton down
	{
		bool PreLButtonSet = xMouse.LButtonDown;
		xMouse.LButtonDown = true;

		// check object actions
		if (h_iObjClicked || (!h_iObjClicked && (h_iObjClicked = ActionPerformed())))
		{
			if (!PreLButtonSet)
				h_iObjClicked->SetIO(!(h_iObjClicked->GetIO()));

			ActionObject(h_iObjClicked);
		}

		// cancel click
		if (xMouse.MButtonDown || xMouse.RButtonDown || h_bMoveActive || 
			(h_iObjClicked && h_iObjClicked->GetCancelClick()))
		{
			return false;
		}
	}
	else if (cState == 2) // lbutton up
	{
		xMouse.LButtonDown = false;

		h_iObjClicked = NULL;
	}
	else if (cState == 3) // mbutton down
	{
		xMouse.MButtonDown = true;

		// cancel click
		if (xMouse.LButtonDown || xMouse.RButtonDown || h_bMoveActive || 
			(h_iObjClicked && h_iObjClicked->GetCancelClick()))
		{
			return false;
		}
	}
	else if (cState == 4) // mbutton up
	{
		xMouse.MButtonDown = false;
	}
	else if (cState == 5) // rbutton down
	{
		xMouse.RButtonDown = true;

		// cancel click
		if (xMouse.LButtonDown || xMouse.MButtonDown || h_bMoveActive || 
			(h_iObjClicked && h_iObjClicked->GetCancelClick()))
		{
			return false;
		}
	}
	else if (cState == 6) // rbutton up
	{
		xMouse.RButtonDown = false;
	}
	else // mouse scroll
	{
	}

	return true;
}

#ifndef HGINCDEF_PUBLIC_H
#define HGINCDEF_PUBLIC_H

#pragma once

#define HGVersion 71223

#define TOP_LEFT		0x00000001
#define TOP_RIGHT		0x00000002
#define BOTTOM_LEFT		0x00000003
#define BOTTOM_RIGHT	0x00000004

// used for moving windows / objects
// (ie HGObj->SetMoveable(MOVE_ALWAYS))
#define MOVE_NEVER		0x00000005
#define MOVE_LIMITED	0x00000006
#define MOVE_ALWAYS		0x00000007

// used for scrollbar orientation
#define HORIZONTAL		0x00000008
#define VERTICAL		0x00000009

// a few color codes
// 0x [00-ff transparency] [00-ff red] [00-ff green] [00-ff blue]
#define COLOR_DEFAULT	D3DCOLOR_ARGB(255, 255, 255, 255)
#define COLOR_HGHLGHT	0xff00ff00
#define COLOR_BLACK		0xff000000
#define COLOR_BLUE		0xff0000ff
#define COLOR_GREEN		0xff00ff00
#define COLOR_RED		0xffff0000
#define COLOR_PURPLE	0xffff00ff
#define COLOR_ORANGE	0xffffb649
#define COLOR_WHITE		0xffffffff

struct KeyInfo
{
	bool bDown;
	bool bRepeat;
	unsigned char cKey;
};

struct MouseInfo
{
	int cState;
	bool LButtonDown; //mouse left button
	bool MButtonDown; //mouse middle button
	bool RButtonDown; //mouse right button
	float CursorX;
	float CursorY;
};

class IDDTexture;
class HGObj;
class HGOImage;
class HGOTextOverImage;
class HGOListViewer;
class HGNode;
class HGExtNode;
class HGNodeList;
class HGMenu;
class HGODefaultList;
class HGOTextBar;
class HGOButton;
class HGOScrollBar;

#define DIRECTINPUT_VERSION DIRECTINPUT_HEADER_VERSION

#include <dinput.h>
#include <d3dx8.h>

typedef void (CALLBACK* ACTIONPROC)(void);

#include "HGuiBase.h"
#include "HGuiRect.h"

#include "HGuiExport.h"

#endif

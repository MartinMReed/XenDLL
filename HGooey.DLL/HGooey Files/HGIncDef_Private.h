#ifndef HGINCDEF_PRIVATE_H
#define HGINCDEF_PRIVATE_H

#pragma once

#define MOUSEWHEEL_DELTA    120
#define MOUSEBUTTON_RIGHT   1
#define MOUSEBUTTON_MIDDLE  2
#define MOUSEBUTTON_LEFT    0

/*#define HGFriendGroup \
friend class HGooey; \
friend class IDDTexture; \
friend class HGObj; \
friend class HGOImage; \
friend class HGOTextOverImage; \
friend class HGOListViewer; \
friend class HGNode; \
friend class HGExtNode; \
friend class HGNodeList; \
friend class HGMenu; \
friend class HGODefaultList; \
friend class HGOTextBar; \
friend class HGOButton; \
friend class HGOScrollBar*/;

class HGooey;
class HGNode;
class HGExtNode;
class HGNodeList;
class HGMenu;

extern HGooey* g_HGooey;

#include "tinyxml/tinyxml.h"

#define XML_VERSION 2

#include "../Resource.h"

#include "HGIncDef_Public.h"

#include "HGooey.h"

extern UINT COLOR_DRAW;

extern HINSTANCE hInst;

#endif

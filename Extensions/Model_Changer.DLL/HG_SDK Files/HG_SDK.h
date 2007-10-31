#ifndef HG_SDK_H
#define HG_SDK_H

#pragma once

#include "..\..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"

#include <stdio.h>

#define BODY 0
#define HELMET 1
#define WEAPON 2
#define SHIELD 3
#define SPELL 4

int Skip_Block(int _type, int _number, bool _plus);

int*** limits;
int* count;

HGNodeList* Model_Changer;
HGOImage* ConfigWindow;

HGOTextBar* cnfg_Title;

bool bBody_Model;
DWORD* dBody_Model;
int BodyID;
HGOTextBar* tbBody_Title;
HGOButton* b_BodyMinus;
HGOButton* b_BodyPlus;

bool bHelmet_Model;
DWORD* dHelmet_Model;
int HelmetID;
HGOTextBar* tbHelmet_Title;
HGOButton* tb_HelmetMinus;
HGOButton* tb_HelmetPlus;

bool bWeapon_Model;
DWORD* dWeapon_Model;
int WeaponID;
HGOTextBar* tbWeapon_Title;
HGOButton* tb_WeaponMinus;
HGOButton* tb_WeaponPlus;

bool bShield_Model;
DWORD* dShield_Model;
int ShieldID;
HGOTextBar* tbShield_Title;
HGOButton* tb_ShieldMinus;
HGOButton* tb_ShieldPlus;

bool bSpell_Model;
DWORD* dSpell_Model;
int SpellID;
HGOTextBar* tbSpell_Title;
HGOButton* tb_SpellMinus;
HGOButton* tb_SpellPlus;

HGOButton* cnfg_CLOSE;

bool Loaded;

bool ConfigOn;

#endif

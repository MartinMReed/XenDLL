#include "HG_SDK.h"




//-------------------------------------------------------
// 
//-------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				Loaded = false;
				ConfigOn = false;

				// do a version check to make sure users dont have older versions of HGooey
				if (!HGVersionCheck(HGVersion))
					return FALSE;

				// load and create stuff here (ie load extensions, create textures/images/objects/etc)

				// make sure the no one moved the extension
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Model_Changer/HG_SDK.dll") == 0xFFFFFFFF) return FALSE;

				// make sure the image is there
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.bmp") == 0xFFFFFFFF) return FALSE;

				// set the default options
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml") == 0xFFFFFFFF)
				{
					WriteConfigItem("Critical Body Low 0", "0", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Body High 0", "3999", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Body Low 1", "4242", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Body High 1", "4242", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Body Low 2", "4422", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Body High 2", "*", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Helmet Low 0", "51", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Helmet High 0", "*", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Weapon Low 0", "682", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Weapon High 0", "*", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Shield Low 0", "51", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					WriteConfigItem("Critical Shield High 0", "*", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
				}

				limits = new int**[5];
				count = new int[5];

				char bufffffffer[256];
				const char* Data;

				for (int j = 0; j < 5; j++)
				{
					char* tempc;
					if (j == BODY) tempc = "Body";
					else if (j == HELMET) tempc = "Helmet";
					else if (j == WEAPON) tempc = "Weapon";
					else if (j == SHIELD) tempc = "Shield";
					else if (j == SPELL) tempc = "Spell";

					count[j] = 0;

					while (sprintf_s(bufffffffer, 256, "Critical %s Low %d", tempc, count[j]) &&
						ReadConfigItem(bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml") &&
						sprintf_s(bufffffffer, 256, "Critical %s High %d", tempc, count[j]) &&
						ReadConfigItem(bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
						count[j]++;

					if (count[j] > 0)
					{
						limits[j] = new int*[count[j]];

						for (int i = 0; i < count[j]; i++)
						{
							limits[j][i] = new int[2];

							sprintf_s(bufffffffer, 256, "Critical %s Low %d", tempc, i);

							if (Data = ReadConfigItem(bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
							{
								if (_stricmp(Data, "*") == 0)
									limits[j][i][0] = INT_MAX;
								else
								{
									int tempi = atoi(Data);
									if (tempi < 0) tempi = 0;
									limits[j][i][0] =  tempi;
								}

								sprintf_s(bufffffffer, 256, "Critical %s High %d", tempc, i);

								if (Data = ReadConfigItem(bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
								{
									if (_stricmp(Data, "*") == 0)
										limits[j][i][1] = INT_MAX;
									else
									{
										int tempi = atoi(Data);
										if (tempi < 0) tempi = 0;
										limits[j][i][1] =  tempi;
									}
								}
							}
						}
					}
					else
					{
						limits[j] = NULL;
					}
				}

				///////////////////////////////

				if (Data = ReadConfigItem("Body Model", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the Body Model memory address
					try { dBody_Model = (DWORD*)atoi(Data); }
					catch (...) { bBody_Model = false; }
					 bBody_Model = true;
				}
				else
				{
					bBody_Model = false;
				}

				if (Data = ReadConfigItem("Helmet Model", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the Helmet Model memory address
					try { dHelmet_Model = (DWORD*)atoi(Data); }
					catch (...) { bHelmet_Model = false; }
					 bHelmet_Model = true;
				}
				else
				{
					bHelmet_Model = false;
				}

				if (Data = ReadConfigItem("Weapon Model", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the Weapon Model memory address
					try { dWeapon_Model = (DWORD*)atoi(Data); }
					catch (...) { bWeapon_Model = false; }
					 bWeapon_Model = true;
				}
				else
				{
					bWeapon_Model = false;
				}

				if (Data = ReadConfigItem("Shield Model", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the Shield Model memory address
					try { dShield_Model = (DWORD*)atoi(Data); }
					catch (...) { bShield_Model = false; }
					 bShield_Model = true;
				}
				else
				{
					bShield_Model = false;
				}

				if (Data = ReadConfigItem("Spell Model", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the Spell Model memory address
					try { dSpell_Model = (DWORD*)atoi(Data); }
					catch (...) { bSpell_Model = false; }
					 bSpell_Model = true;
				}
				else
				{
					bSpell_Model = false;
				}
				
				///

				Model_Changer = CreateExtension(
					CreateIDDTextureFromFile("HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.bmp", COLOR_RED));

				RECT rect;

				float cw_Scale = 1.0f;
				SetRect(&rect, DefaultList_left, DefaultList_top, DefaultList_right, DefaultList_bottom);
				ConfigWindow = CreateImage(
					cw_Scale, 
					cw_Scale,  
					CreateIDDTextureFromFile("HGooey Files/Images/list.bmp", COLOR_RED), 
					&rect);

				if (!AttachObject(Model_Changer, ConfigWindow))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				ConfigWindow->MoveBySize(
					((ScreenWidth() / 2.0f) / GuiScaleX()) - (ConfigWindow->GetWidth() / 2.0f),
					((ScreenHeight() / 2.0f) / GuiScaleY()) - (ConfigWindow->GetHeight() / 2.0f));

				ConfigWindow->SetMoveable(MOVE_ALWAYS);

				///

				float tbc_oborder = 20.0f;
				float tbc_WScale = (cw_Scale * (DefaultList_width - tbc_oborder)) / TextBar_width;

				cnfg_Title = CreateTextBar(
					"Model_Changer Config", 
					tbc_WScale, 
					1.0f);

				if (!AttachObject(ConfigWindow, cnfg_Title))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				cnfg_Title->SetBorderWidth(6.0f * tbc_WScale);
				cnfg_Title->SetBorderHeight(5.0f);

				cnfg_Title->MoveBySize(
					cw_Scale * (tbc_oborder / 2.0f), 
					cw_Scale * 10.0f);

				cnfg_Title->SetAcceptAction(false);

				///////////////////////////////

				float tb_WScale = (cw_Scale * ((DefaultList_width / 2.0f) - 10.0f)) / TextBar_width;

				tbBody_Title = CreateTextBar(
					"Body : ", 
					tb_WScale, 
					1.0f);

				if (!AttachObject(ConfigWindow, tbBody_Title))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				//tbBody_Title->SetTexture(1.0f, 1.0f, NULL, NULL);

				tbBody_Title->SetBorderWidth(6.0f * tb_WScale);
				tbBody_Title->SetBorderHeight(5.0f);

				tbBody_Title->MoveBySize(
					cw_Scale * 10.0f, 
					cw_Scale * ((DefaultList_height / 6.0f) + 10.0f));

				tbBody_Title->SetAcceptAction(false);

				///

				float b_WScale = (((tb_WScale * TextBar_width) / 2.0f) - 5.0f) / Button_width;

				b_BodyMinus = CreateButton(
					"-", 
					b_WScale);

				if (!AttachObject(ConfigWindow, b_BodyMinus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				b_BodyMinus->MoveBySize(
					cw_Scale * ((DefaultList_width / 2.0f) + 5.0f), 
					cw_Scale * ((DefaultList_height / 6.0f) + 10.0f));

				b_BodyMinus->MoveBySize(
					0.0f, 
					(tbBody_Title->GetHeight() / 2.0f) - (b_BodyMinus->GetHeight() / 2.0f));

				///

				b_BodyPlus = CreateButton(
					"+", 
					b_WScale);

				if (!AttachObject(ConfigWindow, b_BodyPlus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				b_BodyPlus->MoveBySize(
					cw_Scale * ((3.0f * (DefaultList_width / 4.0f)) + 0.0f), 
					cw_Scale * ((DefaultList_height / 6.0f) + 10.0f));

				b_BodyPlus->MoveBySize(
					0.0f, 
					(tbBody_Title->GetHeight() / 2.0f) - (b_BodyPlus->GetHeight() / 2.0f));

				///////////////////////////////

				tbHelmet_Title = CreateTextBar(
					"Helmet : ", 
					tb_WScale, 
					1.0f);

				if (!AttachObject(ConfigWindow, tbHelmet_Title))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				//tbBody_Title->SetTexture(1.0f, 1.0f, NULL, NULL);

				tbHelmet_Title->SetBorderWidth(6.0f * tb_WScale);
				tbHelmet_Title->SetBorderHeight(5.0f);

				tbHelmet_Title->MoveBySize(
					cw_Scale * 10.0f, 
					cw_Scale * ((2.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tbHelmet_Title->SetAcceptAction(false);

				///

				tb_HelmetMinus = CreateButton(
					"-", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_HelmetMinus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_HelmetMinus->MoveBySize(
					cw_Scale * ((DefaultList_width / 2.0f) + 5.0f), 
					cw_Scale * ((2.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_HelmetMinus->MoveBySize(
					0.0f, 
					(tbHelmet_Title->GetHeight() / 2.0f) - (tb_HelmetMinus->GetHeight() / 2.0f));

				///

				tb_HelmetPlus = CreateButton(
					"+", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_HelmetPlus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_HelmetPlus->MoveBySize(
					cw_Scale * ((3.0f * (DefaultList_width / 4.0f)) + 0.0f), 
					cw_Scale * ((2.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_HelmetPlus->MoveBySize(
					0.0f, 
					(tbHelmet_Title->GetHeight() / 2.0f) - (tb_HelmetPlus->GetHeight() / 2.0f));

				///////////////////////////////

				tbWeapon_Title = CreateTextBar(
					"Weapon : ", 
					tb_WScale, 
					1.0f);

				if (!AttachObject(ConfigWindow, tbWeapon_Title))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				//tbBody_Title->SetTexture(1.0f, 1.0f, NULL, NULL);

				tbWeapon_Title->SetBorderWidth(6.0f * tb_WScale);
				tbWeapon_Title->SetBorderHeight(5.0f);

				tbWeapon_Title->MoveBySize(
					cw_Scale * 10.0f, 
					cw_Scale * ((3.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tbWeapon_Title->SetAcceptAction(false);

				///

				tb_WeaponMinus = CreateButton(
					"-", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_WeaponMinus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_WeaponMinus->MoveBySize(
					cw_Scale * ((DefaultList_width / 2.0f) + 5.0f), 
					cw_Scale * ((3.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_WeaponMinus->MoveBySize(
					0.0f, 
					(tbWeapon_Title->GetHeight() / 2.0f) - (tb_WeaponMinus->GetHeight() / 2.0f));

				///

				tb_WeaponPlus = CreateButton(
					"+", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_WeaponPlus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_WeaponPlus->MoveBySize(
					cw_Scale * ((3.0f * (DefaultList_width / 4.0f)) + 0.0f), 
					cw_Scale * ((3.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_WeaponPlus->MoveBySize(
					0.0f, 
					(tbWeapon_Title->GetHeight() / 2.0f) - (tb_WeaponPlus->GetHeight() / 2.0f));

				///////////////////////////////

				tbShield_Title = CreateTextBar(
					"Shield : ", 
					tb_WScale, 
					1.0f);

				if (!AttachObject(ConfigWindow, tbShield_Title))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				//tbBody_Title->SetTexture(1.0f, 1.0f, NULL, NULL);

				tbShield_Title->SetBorderWidth(6.0f * tb_WScale);
				tbShield_Title->SetBorderHeight(5.0f);

				tbShield_Title->MoveBySize(
					cw_Scale * 10.0f, 
					cw_Scale * ((4.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tbShield_Title->SetAcceptAction(false);

				///

				tb_ShieldMinus = CreateButton(
					"-", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_ShieldMinus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_ShieldMinus->MoveBySize(
					cw_Scale * ((DefaultList_width / 2.0f) + 5.0f), 
					cw_Scale * ((4.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_ShieldMinus->MoveBySize(
					0.0f, 
					(tbShield_Title->GetHeight() / 2.0f) - (tb_ShieldMinus->GetHeight() / 2.0f));

				///

				tb_ShieldPlus = CreateButton(
					"+", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_ShieldPlus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_ShieldPlus->MoveBySize(
					cw_Scale * ((3.0f * (DefaultList_width / 4.0f)) + 0.0f), 
					cw_Scale * ((4.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_ShieldPlus->MoveBySize(
					0.0f, 
					(tbShield_Title->GetHeight() / 2.0f) - (tb_ShieldPlus->GetHeight() / 2.0f));

				///////////////////////////////

				tbSpell_Title = CreateTextBar(
					"Spell : ", 
					tb_WScale, 
					1.0f);

				if (!AttachObject(ConfigWindow, tbSpell_Title))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				//tbBody_Title->SetTexture(1.0f, 1.0f, NULL, NULL);

				tbSpell_Title->SetBorderWidth(6.0f * tb_WScale);
				tbSpell_Title->SetBorderHeight(5.0f);

				tbSpell_Title->MoveBySize(
					cw_Scale * 10.0f, 
					cw_Scale * ((5.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tbSpell_Title->SetAcceptAction(false);

				///

				tb_SpellMinus = CreateButton(
					"-", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_SpellMinus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_SpellMinus->MoveBySize(
					cw_Scale * ((DefaultList_width / 2.0f) + 5.0f), 
					cw_Scale * ((5.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_SpellMinus->MoveBySize(
					0.0f, 
					(tbSpell_Title->GetHeight() / 2.0f) - (tb_SpellMinus->GetHeight() / 2.0f));

				///

				tb_SpellPlus = CreateButton(
					"+", 
					b_WScale);

				if (!AttachObject(ConfigWindow, tb_SpellPlus))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				tb_SpellPlus->MoveBySize(
					cw_Scale * ((3.0f * (DefaultList_width / 4.0f)) + 0.0f), 
					cw_Scale * ((5.0f * (DefaultList_height / 6.0f)) + 10.0f));

				tb_SpellPlus->MoveBySize(
					0.0f, 
					(tbSpell_Title->GetHeight() / 2.0f) - (tb_SpellPlus->GetHeight() / 2.0f));

				///////////////////////////////

				cnfg_CLOSE = CreateButton("CLOSE", cw_Scale);

				if (!AttachObject(ConfigWindow, cnfg_CLOSE))
				{
					UnloadExtension(Model_Changer);
					Model_Changer = NULL;

					return FALSE;
				}

				cnfg_CLOSE->MoveBySize(
					(cw_Scale * (DefaultList_width / 2.0f)) - (cnfg_CLOSE->GetWidth() / 2.0f), 
					cw_Scale * ((6.0f * (DefaultList_height / 6.0f)) + 10.0f));

				///

				float cw_HScale = (cnfg_Title->GetHeight() + tbBody_Title->GetHeight() + tbHelmet_Title->GetHeight() + tbWeapon_Title->GetHeight() + tb_ShieldPlus->GetHeight() + tb_SpellPlus->GetHeight() + cnfg_CLOSE->GetHeight() + (16.0f * 5.0f)) / DefaultList_height;

				ConfigWindow->ReScale(
					cw_Scale,
					cw_Scale * cw_HScale);

				///

				BodyID = 4000;
				HelmetID = 0;
				WeaponID = 0;
				ShieldID = 0;
				SpellID = 0;

				if (Data = ReadConfigItem("BodyID", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
				{
					int itemp = atoi(Data);
					if (itemp == Skip_Block(BODY, itemp, true)) BodyID = itemp;
				}

				if (Data = ReadConfigItem("HelmetID", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
				{
					int itemp = atoi(Data);
					if (itemp == Skip_Block(HELMET, itemp, true)) HelmetID = itemp;
				}

				if (Data = ReadConfigItem("WeaponID", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
				{
					int itemp = atoi(Data);
					if (itemp == Skip_Block(WEAPON, itemp, true)) WeaponID = itemp;
				}

				if (Data = ReadConfigItem("ShieldID", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
				{
					int itemp = atoi(Data);
					if (itemp == Skip_Block(SHIELD, itemp, true)) ShieldID = itemp;
				}

				if (Data = ReadConfigItem("SpellID", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
				{
					int itemp = atoi(Data);
					if (itemp == Skip_Block(SPELL, itemp, true)) SpellID = itemp;
				}

				///

				bool write = false;

				if (Data = ReadConfigItem("MenuItemIO", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml"))
				{
					if (strcmp(Data, "true") == 0)
						SetExtMenuIO(Model_Changer, true);
					else if (strcmp(Data, "false") == 0)
						SetExtMenuIO(Model_Changer, false);
					else
					{
						SetExtMenuIO(Model_Changer, false);
						write = true;
					}
				}
				else
				{
					write = true;
				}

				if (write)
				{
					WriteConfigItem("MenuItemIO", "false", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
					write = false;
				}

				Loaded = true;
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				if (!Loaded) return TRUE;

				Loaded = false;

				// unload and destroy stuff here (ie unload extensions)

				if (CheckExtMenuIO(Model_Changer))
					WriteConfigItem("MenuItemIO", "true", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
				else
					WriteConfigItem("MenuItemIO", "false", "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");

				char bufffffffer[256];

				sprintf_s(bufffffffer, 256, "%d", BodyID);
				WriteConfigItem("BodyID", bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
				
				sprintf_s(bufffffffer, 256, "%d", HelmetID);
				WriteConfigItem("HelmetID", bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
				
				sprintf_s(bufffffffer, 256, "%d", WeaponID);
				WriteConfigItem("WeaponID", bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
				
				sprintf_s(bufffffffer, 256, "%d", ShieldID);
				WriteConfigItem("ShieldID", bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");
				
				sprintf_s(bufffffffer, 256, "%d", SpellID);
				WriteConfigItem("SpellID", bufffffffer, "HGooey Files/XenDLL/Extensions/Model_Changer/Model_Changer.xml");

				UnloadExtension(Model_Changer);
				Model_Changer = NULL;
			}
			break;
		
    }

    return TRUE;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
int Skip_Block(int _type, int _number, bool _plus)
{
	int iReturn = _number;

	for (int i = 0; i < count[_type]; i++)
	{
		if (_number >= limits[_type][i][0] && _number <= limits[_type][i][1])
		{
			if (_plus && limits[_type][i][1] != INT_MAX && limits[_type][i][1]+1 > iReturn) iReturn = limits[_type][i][1]+1;
			else if (_plus && limits[_type][i][1] == INT_MAX) return _number-1;
			else if (!_plus && limits[_type][i][0] != 0 && limits[_type][i][0]-1 < iReturn) iReturn = limits[_type][i][0]-1;
			else if (!_plus && limits[_type][i][0] == 0) return _number+1;
		}
	}

	return iReturn;
}




//-------------------------------------------------------
// the device was lost (ie ALT+TAB to minimize the game)
//-------------------------------------------------------
void LostDevice(void)
{
	if (!Loaded) return;

	// any textures created using CreateIDDTextureFromFile(float,float,char*,UINT)
	// are automatically released by HGooey. this does not, however, cover
	// textures created using CreateIDDTexture(float,float,IDirect3DTexture8*)
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void ResetDevice(void)
{
	if (!Loaded) return;

	// any textures created using CreateIDDTextureFromFile(float,float,char*,UINT)
	// are automatically restored by HGooey. this does not, however, cover
	// textures created using CreateIDDTexture(float,float,IDirect3DTexture8*)
}




//-------------------------------------------------------
// called before any extensions draw on the screen
//-------------------------------------------------------
void PreRender(void)
{
	if (!Loaded) return;

	// DO NOT DRAW IN HERE, ALL DRAWING IS DISABLED.
	// ONLY DRAW IN Render()
}




//-------------------------------------------------------
// do your drawing / call your main code in here
//-------------------------------------------------------
void Render(void)
{
	if (!Loaded) return;

	// draw text or execute/call main code

	if (ConfigOn)
	{
		char bufffffffer[256];

		sprintf_s(bufffffffer, 256, "Body : %d", BodyID);
		tbBody_Title->SetText(bufffffffer);

		sprintf_s(bufffffffer, 256, "Helmet : %d", HelmetID);
		tbHelmet_Title->SetText(bufffffffer);

		sprintf_s(bufffffffer, 256, "Weapon : %d", WeaponID);
		tbWeapon_Title->SetText(bufffffffer);

		sprintf_s(bufffffffer, 256, "Shield : %d", ShieldID);
		tbShield_Title->SetText(bufffffffer);

		sprintf_s(bufffffffer, 256, "Spell : %d", SpellID);
		tbSpell_Title->SetText(bufffffffer);

		if (b_BodyMinus->GetIO())
		{
			if (bBody_Model && BodyID > 0) BodyID = Skip_Block(BODY, --BodyID, false);
			b_BodyMinus->SetIO(false);
		}

		if (tb_HelmetMinus->GetIO())
		{
			if (bHelmet_Model && HelmetID > 0) HelmetID = Skip_Block(HELMET, --HelmetID, false);
			tb_HelmetMinus->SetIO(false);
		}

		if (tb_WeaponMinus->GetIO())
		{
			if (bWeapon_Model && WeaponID > 0) WeaponID = Skip_Block(WEAPON, --WeaponID, false);
			tb_WeaponMinus->SetIO(false);
		}

		if (tb_ShieldMinus->GetIO())
		{
			if (bShield_Model && ShieldID > 0) ShieldID = Skip_Block(SHIELD, --ShieldID, false);
			tb_ShieldMinus->SetIO(false);
		}

		if (tb_SpellMinus->GetIO())
		{
			if (bSpell_Model && SpellID > 0) SpellID = Skip_Block(SPELL, --SpellID, false);
			tb_SpellMinus->SetIO(false);
		}

		if (b_BodyPlus->GetIO())
		{
			if (bBody_Model) BodyID = Skip_Block(BODY, ++BodyID, true);
			b_BodyPlus->SetIO(false);
		}

		if (tb_HelmetPlus->GetIO())
		{
			if (bHelmet_Model) HelmetID = Skip_Block(HELMET, ++HelmetID, true);
			tb_HelmetPlus->SetIO(false);
		}

		if (tb_WeaponPlus->GetIO())
		{
			if (bWeapon_Model) WeaponID = Skip_Block(WEAPON, ++WeaponID, true);
			tb_WeaponPlus->SetIO(false);
		}

		if (tb_ShieldPlus->GetIO())
		{
			if (bShield_Model) ShieldID = Skip_Block(SHIELD, ++ShieldID, true);
			tb_ShieldPlus->SetIO(false);
		}

		if (tb_SpellPlus->GetIO())
		{
			if (bSpell_Model) SpellID = Skip_Block(SPELL, ++SpellID, true);
			tb_SpellPlus->SetIO(false);
		}

		if (cnfg_CLOSE->GetIO())
		{
			ConfigOn = false;
			cnfg_CLOSE->SetIO(false);
		}
	}

	ActivateFull(Model_Changer, ConfigOn);

	if (CheckExtMenuIO(Model_Changer))
	{
		if (bBody_Model) *dBody_Model = BodyID;
		if (bHelmet_Model) *dHelmet_Model = HelmetID;
		if (bWeapon_Model) *dWeapon_Model = WeaponID;
		if (bShield_Model) *dShield_Model = ShieldID;
		if (bSpell_Model)  *dSpell_Model = SpellID;
	}
}




//-------------------------------------------------------
// handle mouse input
//
// return true = send command to game
// return false = do not send the command to the game
//-------------------------------------------------------
bool HandleMouse()
{	
	if (!Loaded) return true;

	MouseInfo xMouse = Mouse();

	if (xMouse.cState == 0) // mouse move
	{
	}
	else if (xMouse.cState == 1) // left button down
	{
		/**
		// (optional) cancel click if other buttons are being pressed
		if (xMouse.MButtonDown || xMouse.RButtonDown)
			return false;
		/**/
	}
	else if (xMouse.cState == 2) // left button up
	{
	}
	else if (xMouse.cState == 3) // middle button down
	{
		/**
		// (optional) cancel click if other buttons are being pressed
		if (xMouse.LButtonDown || xMouse.RButtonDown)
			return false;
		/**/
	}
	else if (xMouse.cState == 4) // middle button up
	{
	}
	else if (xMouse.cState == 5) // right button down
	{
		/**
		// (optional) cancel click if other buttons are being pressed
		if (xMouse.LButtonDown || xMouse.MButtonDown)
			return false;
		/**/
	}
	else if (xMouse.cState == 6) // right button up
	{
	}
	else // mouse scroll
	{
	}

	return true;
}




//-------------------------------------------------------
// handle keyboard input
//
// cKey = DIK_[Key]
// bDown = pressed down if true, released if false
//
// return true = send command to game
// return false = do not send the command to the game
//
// DirectInput Key-Identifier Table:
// http://calc.xentales.com/HGooey/DIK.html or
// http://www.xentales.com/calc/HGooey/DIK.html
//-------------------------------------------------------
bool HandleKeyboard()
{
	if (!Loaded) return true;

	KeyInfo xKey = Key();

	if (xKey.bDown) // key is pressed down
	{
	}
	else if (!xKey.bRepeat) // key has been released
	{
	}

	return true;
}




//-------------------------------------------------------
// turn on the config window
//
// there is no reason to edit this, build your
// config window when the .dll is first loaded.
// however, there is nothing critical here, so
// you do what you want here.
//-------------------------------------------------------
void Config(void)
{
	ConfigOn = true;
}




//-------------------------------------------------------
// version check called by XenDLL
//-------------------------------------------------------
UINT HGSDKVersionCheck(void)
{
	// DO NOT EDIT THIS
	return 61225;
}

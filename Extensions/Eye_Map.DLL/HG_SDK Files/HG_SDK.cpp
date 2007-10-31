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
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Eye_Map/HG_SDK.dll") == 0xFFFFFFFF) return FALSE;

				// make sure the menu item texture exists
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.bmp") == 0xFFFFFFFF) return FALSE;

				// make sure the menu memory file exists
				if (GetFileAttributes("HGooey Files/XenDLL/Common/Memory.xml") == 0xFFFFFFFF) return FALSE;

				///
	
				const char* Data;
				bool write = false;
				bool not_found = false;

				if (Data = ReadConfigItem("Location X", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the location x memory address
					try { Location_X = (DWORD*)atoi(Data); }
					catch (...) { return FALSE; }
				}
				else
				{
					return FALSE;
				}

				if (Data = ReadConfigItem("Location Y", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the location y memory address
					try { Location_Y = (DWORD*)atoi(Data); }
					catch (...) { return FALSE; }
				}
				else
				{
					return FALSE;
				}

				if (Data = ReadConfigItem("Screen", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the screen memory address
					try { Screen = (DWORD*)atoi(Data); }
					catch (...) { return FALSE; }
				}
				else
				{
					return FALSE;
				}

				if (Data = ReadConfigItem("Chatmode", "HGooey Files/XenDLL/Common/Memory.xml"))
				{
					// check the chatmode memory address
					try { Chatmode = (DWORD*)atoi(Data); }
					catch (...) { return FALSE; }
				}
				else
				{
					return FALSE;
				}
	
				///
				
				bZoomIN = false;
				bZoomOUT = false;

				Eye_Map = CreateExtension();

				IDDTexture* texture = CreateIDDTextureFromFile("HGooey Files/Images/list.bmp", COLOR_RED);

				// set the menu item
				SetExtensionMenuItem(
					Eye_Map,
					CreateIDDTextureFromFile("HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.bmp", COLOR_RED));

				///

				if (Data = ReadConfigItem("Hide/Show Using DIK_[Key]", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					if ((HIDESHOW_KEY = convertDIK_Name(Data)) == 0x00)
							not_found = true;
				}
				else
				{
					not_found = true;
				}

				if (not_found)
				{
					HIDESHOW_KEY = 0x00;
					WriteConfigItem("Hide/Show Using DIK_[Key]", "DIK_", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				}

				///

				if (Data = ReadConfigItem("MenuItemIO", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					if (strcmp(Data, "true") == 0)
						SetExtMenuIO(Eye_Map, true);
					else if (strcmp(Data, "false") == 0)
						SetExtMenuIO(Eye_Map, false);
					else
					{
						SetExtMenuIO(Eye_Map, false);
						write = true;
					}
				}
				else
				{
					write = true;
				}

				if (write)
				{
					WriteConfigItem("MenuItemIO", "false", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					write = false;
				}

				///

				RECT rect;

				SetRect(&rect, DefaultList_left, DefaultList_top, DefaultList_right, DefaultList_bottom);
				Background = CreateImage(
					1.0f, 
					1.0f,  
					texture, 
					&rect);

				if (!AttachObject(Eye_Map, Background))
				{
					UnloadExtension(Eye_Map);
					Eye_Map = NULL;

					return FALSE;
				}

				///
	
				ZoomIN = CreateButton(" + ", 1.0f);

				if (!AttachObject(Background, ZoomIN))
				{
					UnloadExtension(Eye_Map);
					Eye_Map = NULL;

					return FALSE;
				}

				///

				ZoomOUT = CreateButton(" - ", 1.0f);

				if (!AttachObject(Background, ZoomOUT))
				{
					UnloadExtension(Eye_Map);
					Eye_Map = NULL;

					return FALSE;
				}

				///

				ZoomRESET = CreateButton("reset", 1.0f);

				if (!AttachObject(Background, ZoomRESET))
				{
					UnloadExtension(Eye_Map);
					Eye_Map = NULL;

					return FALSE;
				}

				///

				MapButtonText[0] = "Main";
				MapButtonText[1] = "2";
				MapButtonText[2] = "3";

				///

				// set a default and then read the config file
				bool DisplayMoveAlways = false;

				if (Data = ReadConfigItem("DisplayMoveAlways", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					if (_stricmp(Data, "true") == 0)
						DisplayMoveAlways = true;
					else if (_stricmp(Data, "false") != 0)
						write = true;
				}
				else
				{
					write = true;
				}

				if (write)
				{
					WriteConfigItem("DisplayMoveAlways", "false", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					write = false;
				}

				if (DisplayMoveAlways)
					Background->SetMoveable(MOVE_ALWAYS);
				else
					Background->SetMoveable(MOVE_LIMITED);

				float DisplaySizeOriginal = 128.0f;
				DisplaySize = DisplaySizeOriginal;

				if (Data = ReadConfigItem("DisplaySizeScale", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					DisplaySize *= (float)atof(Data) / 100.0f;
				}
				else
				{
					char buffer[64];
					sprintf_s(buffer, 64, "%f", 100.0f);
					WriteConfigItem("DisplaySizeScale", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				}

				DisplayInitialScale = DisplaySizeOriginal / DisplaySize;

				///

				if (Data = ReadConfigItem("MapButtonText[0]", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					MapButtonText[0] = restring(Data, 0, strlen(Data));
				}
				else
				{
					WriteConfigItem("MapButtonText[0]", MapButtonText[0], "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				}

				MapFile[0] = "xenmap0.bmp";

				if (GetFileAttributes(MapFile[0]) == 0xFFFFFFFF)
					MapFile[0] = "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.bmp";

				if (Data = ReadConfigItem("Map[0] Filename", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					if (GetFileAttributes(Data) != 0xFFFFFFFF)
							MapFile[0] = restring(Data, 0, strlen(Data));
				}
				else
				{
					WriteConfigItem("MapFile[0] Filename", MapFile[0], "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				}

				MapF2GX[0] = 6.2890625f;

				if (Data = ReadConfigItem("Map[0] File2Game X", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					MapF2GX[0] = (float)atof(Data);
				}
				else
				{
					char buffer[64];
					sprintf_s(buffer, 64, "%f", MapF2GX[0]);
					WriteConfigItem("Map[0] File2Game X", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				}

				MapF2GY[0] = 6.2890625f;

				if (Data = ReadConfigItem("Map[0] File2Game Y", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					MapF2GY[0] = (float)atof(Data);
				}
				else
				{
					char buffer[64];
					sprintf_s(buffer, 64, "%f", MapF2GY[0]);
					WriteConfigItem("Map[0] File2Game Y", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				}

				MapTexture[0] = CreateIDDTextureFromFile(MapFile[0], COLOR_DEFAULT);

				SetRect(&Zoom, 0, 0, (int)(DisplaySize * DisplayInitialScale), (int)(DisplaySize * DisplayInitialScale));
				Map[0] = CreateImage(
					1.0f, 
					1.0f,  
					MapTexture[0], 
					&Zoom);
				
				if (!AttachObject(Background, Map[0]))
				{
					UnloadExtension(Eye_Map);
					Eye_Map = NULL;

					return FALSE;
				}

				///

				for (int i = 1; i < 3; i++)
				{
					char bufffffer[128];
					
					sprintf_s(bufffffer, 128, "MapButtonText[%d]", i);

					if (Data = ReadConfigItem(bufffffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
					{
						MapButtonText[i] = restring(Data, 0, strlen(Data));
					}
					else
					{
						WriteConfigItem(bufffffer, MapButtonText[i], "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					}

					MapFile[i] = "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.bmp";
					
					sprintf_s(bufffffer, 128, "Map[%d] Filename", i);

					if (Data = ReadConfigItem(bufffffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
					{
						if (GetFileAttributes(Data) != 0xFFFFFFFF)
							MapFile[i] = restring(Data, 0, strlen(Data));
					}
					else
					{
						WriteConfigItem(bufffffer, MapFile[i], "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					}

					MapF2GX[i] = 0.0f;
					
					sprintf_s(bufffffer, 128, "Map[%d] File2Game X", i);

					if (Data = ReadConfigItem(bufffffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
					{
						MapF2GX[i] = (float)atof(Data);
					}
					else
					{
						char buffer[64];
						sprintf_s(buffer, 64, "%f", MapF2GX[i]);
						WriteConfigItem(bufffffer, buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					}

					MapF2GY[i] = 0.0f;
					
					sprintf_s(bufffffer, 128, "Map[%d] File2Game Y", i);

					if (Data = ReadConfigItem(bufffffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
					{
						MapF2GY[i] = (float)atof(Data);
					}
					else
					{
						char buffer[64];
						sprintf_s(buffer, 64, "%f", MapF2GY[i]);
						WriteConfigItem(bufffffer, buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					}

					MapTexture[i] = CreateIDDTextureFromFile(MapFile[i], COLOR_DEFAULT);

					SetRect(&Zoom, 0, 0, (int)(DisplaySize * DisplayInitialScale), (int)(DisplaySize * DisplayInitialScale));
					Map[i] = CreateImage(
						1.0f, 
						1.0f,  
						MapTexture[i], 
						&Zoom);
					
					if (!AttachObject(Background, Map[i]))
					{
						UnloadExtension(Eye_Map);
						Eye_Map = NULL;

						return FALSE;
					}
				}

				///
				
				for (int i = 0; i < 3; i++)
				{
					MapButton[i] = CreateButton(MapButtonText[i], 1.0f);

					if (!AttachObject(Background, MapButton[i]))
					{
						UnloadExtension(Eye_Map);
						Eye_Map = NULL;

						return FALSE;
					}

					MapButton[i]->SetStickyIO(true);
				}

				MapButton[0]->SetIO(true);

				///

				Location = CreateImage(
					1.0f, 
					1.0f,  
					CreateIDDTextureFromFile("HGooey Files/XenDLL/Extensions/Eye_Map/location.bmp", COLOR_RED));
				
				if (!AttachObject(Background, Location))
				{
					UnloadExtension(Eye_Map);
					Eye_Map = NULL;

					return FALSE;
				}

				Location->SetAcceptAction(false);
				Location->SetCancelClick(false);

				///

				float DisplayBorder = 7.5f * (DisplaySize / DisplaySizeOriginal);
				float MapButtonBorder = 2.5f  * (DisplaySize / DisplaySizeOriginal);

				for (int i = 0; i < 3; i++)
					MapButton[i]->ReScale((DisplaySize / 3.0f) / MapButton[i]->GetWidth(), (DisplaySize / 3.0f) / MapButton[i]->GetWidth());

				ZoomIN->ReScale((DisplaySize / 3.0f) / ZoomIN->GetWidth(), (DisplaySize / 3.0f) / ZoomIN->GetWidth());
				ZoomOUT->ReScale((DisplaySize / 3.0f) / ZoomOUT->GetWidth(), (DisplaySize / 3.0f) / ZoomOUT->GetWidth());
				ZoomRESET->ReScale((DisplaySize / 3.0f) / ZoomRESET->GetWidth(), (DisplaySize / 3.0f) / ZoomRESET->GetWidth());

				// should all be the same, but... whatever...
				float largerHeight0 = MapButton[0]->GetHeight();
				if (MapButton[1]->GetHeight() > largerHeight0) largerHeight0 = MapButton[1]->GetHeight();
				if (MapButton[2]->GetHeight() > largerHeight0) largerHeight0 = MapButton[2]->GetHeight();

				float largerHeight1 = ZoomIN->GetHeight();
				if (ZoomOUT->GetHeight() > largerHeight1) largerHeight1 = ZoomOUT->GetHeight();
				if (ZoomRESET->GetHeight() > largerHeight1) largerHeight1 = ZoomRESET->GetHeight();

				float largerHeight2 = largerHeight0 + largerHeight1;

				Background->ReScale(
					(DisplaySize + (DisplayBorder*2.0f)) / (float)DefaultList_width,
					(DisplaySize + (DisplayBorder*2.0f) + MapButtonBorder + largerHeight2) / (float)DefaultList_height);

				DisplayX = 0.0f;

				if (Data = ReadConfigItem("DisplayX", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					DisplayX = (float)atof(Data);

					if (DisplayX < 0.0f)
					{
						DisplayX = 0.0f;
						write = true;
					}
					else if ((DisplayX + Background->GetWidth()) * GuiScaleX() > ScreenWidth())
					{
						DisplayX = (ScreenWidth() / GuiScaleX()) - Background->GetWidth();
						write = true;
					}
				}
				else
				{
					write = true;
				}

				if (write)
				{
					char buffer[64];
					sprintf_s(buffer, 64, "%f", DisplayX);
					WriteConfigItem("DisplayX", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					write = false;
				}

				DisplayY = 0.0f;

				if (Data = ReadConfigItem("DisplayY", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml"))
				{
					DisplayY = (float)atof(Data);

					if (DisplayY < 0.0f)
					{
						DisplayY = 0.0f;
						write = true;
					}
					else if ((DisplayY + Background->GetHeight()) * GuiScaleY() > ScreenHeight())
					{
						DisplayY = (ScreenHeight() / GuiScaleY()) - Background->GetHeight();
						write = true;
					}
				}
				else
				{
					write = true;
				}

				if (write)
				{
					char buffer[64];
					sprintf_s(buffer, 64, "%f", DisplayY);
					WriteConfigItem("DisplayY", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					write = false;
				}

				Background->MoveBySize(
					DisplayX, 
					DisplayY);

				MapButton[0]->MoveBySize(
					DisplayBorder, 
					DisplayBorder);

				MapButton[1]->MoveBySize(
					DisplayBorder + (DisplaySize / 2.0f) - (MapButton[1]->GetWidth() / 2.0f), 
					DisplayBorder);

				MapButton[2]->MoveBySize(
					DisplayBorder + DisplaySize - MapButton[2]->GetWidth(), 
					DisplayBorder);

				ZoomIN->MoveBySize(
					DisplayBorder, 
					DisplayBorder + largerHeight0);

				ZoomOUT->MoveBySize(
					DisplayBorder + (DisplaySize / 2.0f) - (ZoomOUT->GetWidth() / 2.0f), 
					DisplayBorder + largerHeight0);

				ZoomRESET->MoveBySize(
					DisplayBorder + DisplaySize - ZoomRESET->GetWidth(), 
					DisplayBorder + largerHeight0);

				for (int i = 0; i < 3; i++)
				{
					Map[i]->MoveBySize(
						DisplayBorder, 
						DisplayBorder + MapButtonBorder + largerHeight2);
				}

				Location->MoveBySize(
					DisplayBorder,
					DisplayBorder + MapButtonBorder + largerHeight2);

				Loaded = true;
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				if (!Loaded) return TRUE;

				Loaded = false;

				// unload and destroy stuff here (ie unload extensions)
				
				char buffer[64];

				sprintf_s(buffer, 64, "%f", Background->GetX() / GuiScaleX());
				WriteConfigItem("DisplayX", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");

				sprintf_s(buffer, 64, "%f", Background->GetY() / GuiScaleY());
				WriteConfigItem("DisplayY", buffer, "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");

				if (CheckExtMenuIO(Eye_Map))
					WriteConfigItem("MenuItemIO", "true", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
				else
					WriteConfigItem("MenuItemIO", "false", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");

				UnloadExtension(Eye_Map);
				Eye_Map = NULL;
			}
			break;
		
    }

    return TRUE;
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

	ActivateFull(Eye_Map, false);

	// return if not ingame
	if (*Screen != 0)
		return;

	bool bIO = CheckExtMenuIO(Eye_Map);
	ActivateFull(Eye_Map, bIO);

	if (!bIO)
		return;

	Map[0]->SetActive(false);
	Map[1]->SetActive(false);
	Map[2]->SetActive(false);

	///

	int countIO = MapButton[0]->GetIO() + MapButton[1]->GetIO() + MapButton[2]->GetIO();

	static int lastIO = -1;

	int Show = -1;
	HGOImage* ShowMap = NULL;
	IDDTexture* ShowMapTexture = NULL;

	bool changed = false;

	// only allow one map button to be selected. one and always one. no more, no less.
	for (int i = 0; i < 3; i++)
	{
		if ((countIO == 0 && lastIO == i) || (MapButton[i]->GetIO() && (countIO == 1 || lastIO != i)))
		{
			if (lastIO != i)
				changed = true;

			lastIO = i;
			Map[i]->SetActive(true);

			MapButton[0]->SetIO(false);
			MapButton[1]->SetIO(false);
			MapButton[2]->SetIO(false);

			MapButton[i]->SetIO(true);

			Show = i;
			ShowMap = Map[i];
			ShowMapTexture = MapTexture[i];

			break;
		}
	}

	///

	if (Show < 0 || !ShowMap || !ShowMapTexture) return;

	int locationX = *Location_X;
	int locationY = *Location_Y;
	
	static int last_locx = locationX;
	static int last_locy = locationY;

	static bool loggedin = false;

	// not logged into the game yet, do not display any maps
	if (!loggedin && locationX == 0 && locationY == 0)
	{
		Map[0]->SetActive(false);
		Map[1]->SetActive(false);
		Map[2]->SetActive(false);
		Location->SetActive(false);

		return;
	}

	loggedin = true;

	// if the image is not a map
	if (_stricmp(MapFile[Show], "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.bmp") == 0)
	{
		Location->SetActive(false);

		SetRect(&Zoom, 0, 0, (int)ShowMapTexture->GetWidth(), (int)ShowMapTexture->GetHeight());
		ShowMap->SetTexture(
			DisplaySize / (float)(Zoom.right - Zoom.left), 
			DisplaySize / (float)(Zoom.bottom - Zoom.top), 
			ShowMapTexture, &Zoom);

		return;
	}

	float MapX = 0.0f;
	float MapY = 0.0f;

	// convert the game coordinates to map coordinates
	if (MapF2GX[Show] != 0 && MapF2GY[Show] != 0)
	{
		MapX = (float)locationX / MapF2GX[Show];
		MapY = (float)locationY / MapF2GY[Show];
	}
	else
	{
		Location->SetActive(false);
	}

	bool zoomed = false;

	static bool oncourse = true;

	///

	// zoom the map in
	if (bZoomIN && Zoom.left + 4 < Zoom.right - 4 && Zoom.top + 4 < Zoom.bottom - 4)
	{
		bZoomIN = false;
		zoomed = true;
		oncourse = true;

		SetRect(&Zoom, Zoom.left + 4, Zoom.top + 4, Zoom.right - 4, Zoom.bottom - 4);
		ShowMap->SetTexture(
			DisplaySize / (float)(Zoom.right - Zoom.left), 
			DisplaySize / (float)(Zoom.bottom - Zoom.top), 
			ShowMapTexture, &Zoom);
	}

	///
	
	// zoom the map out
	if (bZoomOUT)
	{
		bZoomOUT = false;
		zoomed = true;
		oncourse = true;

		int def = 4;

		int inL, inT, inR, inB;

		if (Zoom.left - def >= 0 && Zoom.right + def <= ShowMapTexture->GetWidth())
		{
			inL = def;
			inR = def;
		}
		else if (Zoom.left - def < 0 && Zoom.right + (def * 2) <= ShowMapTexture->GetWidth())
		{
			inL = Zoom.left;
			inR = def * 2;
		}
		else if (Zoom.left - (def * 2) >= 0 && Zoom.right + def > ShowMapTexture->GetWidth())
		{
			inL = def * 2;
			inR = (int)ShowMapTexture->GetWidth() - Zoom.right;
		}
		else //if (Zoom.left - def < 0 && Zoom.right + def > ShowMapTexture->GetWidth())
		{
			inL = Zoom.left;
			inR = (int)ShowMapTexture->GetWidth() - Zoom.right;
		}

		if (Zoom.top - def >= 0 && Zoom.bottom + def <= ShowMapTexture->GetHeight())
		{
			inT = def;
			inB = def;
		}
		else if (Zoom.top - def < 0 && Zoom.bottom + (def * 2) <= ShowMapTexture->GetHeight())
		{
			inT = Zoom.top;
			inB = def * 2;
		}
		else if (Zoom.top - (def * 2) >= 0 && Zoom.bottom + def > ShowMapTexture->GetHeight())
		{
			inT = def * 2;
			inB = (int)ShowMapTexture->GetHeight() - Zoom.bottom;
		}
		else //if (Zoom.top - def < 0 && Zoom.bottom + def > ShowMapTexture->GetHeight())
		{
			inT = Zoom.top;
			inB = (int)ShowMapTexture->GetHeight() - Zoom.bottom;
		}

		SetRect(&Zoom, Zoom.left - inL, Zoom.top - inT, Zoom.right + inR, Zoom.bottom + inB);
		ShowMap->SetTexture(
			DisplaySize / (float)(Zoom.right - Zoom.left), 
			DisplaySize / (float)(Zoom.bottom - Zoom.top), 
			ShowMapTexture, &Zoom);
	}

	///
	
	// reset the zoom
	if (changed || ZoomRESET->GetIO())
	{
		zoomed = true;
		oncourse = true;

		float halfD = (DisplaySize * DisplayInitialScale) / 2.0f;

		SetRect(&Zoom, (int)(MapX - halfD), (int)(MapY - halfD), (int)(MapX + halfD), (int)(MapY + halfD));
		ShowMap->SetTexture(
			DisplaySize / (float)(Zoom.right - Zoom.left), 
			DisplaySize / (float)(Zoom.bottom - Zoom.top), 
			ShowMapTexture, &Zoom);
	}

	///

	// adjust the map so it follows the character
	if (changed || zoomed || last_locx != locationX || last_locy != locationY)
	{
		oncourse = true;

		Location->MoveByCoord(
			ShowMap->GetX() - Location->GetX(),
			ShowMap->GetY() - Location->GetY());

		last_locx = locationX;
		last_locy = locationY;

		float halfW = (float)(Zoom.right - Zoom.left) / 2.0f;
		float halfH = (float)(Zoom.bottom - Zoom.top) / 2.0f;

		int inL, inT, inR, inB;

		if (MapX - halfW >= 0.0f && MapX + halfW <= ShowMapTexture->GetWidth())
		{
			inL = (int)(MapX - halfW);
			inR = (int)(MapX + halfW);

			Location->MoveBySize(
				(DisplaySize / 2.0f) - (Location->GetWidth() / 2.0f), 
				0.0f);
		}
		else if (MapX - halfW < 0.0f && MapX + halfW <= ShowMapTexture->GetWidth())
		{
			inL = 0;
			inR = (int)(halfW * 2.0f);

			Location->MoveBySize(
				(DisplaySize / 2.0f) - ((0.0f - (MapX - halfW)) * (DisplaySize / (halfW * 2.0f))) - (Location->GetWidth() / 2.0f), 
				0.0f);
		}
		else if (MapX - halfW >= 0.0f && MapX + halfW > ShowMapTexture->GetWidth())
		{
			inL = (int)(ShowMapTexture->GetWidth() - (halfW * 2.0f));
			inR = (int)ShowMapTexture->GetWidth();

			Location->MoveBySize(
				(DisplaySize / 2.0f) + (((MapX + halfW) - ShowMapTexture->GetWidth()) * (DisplaySize / (halfW * 2.0f))) - (Location->GetWidth() / 2.0f), 
				0.0f);
		}/* // not reached
		else //if (MapX - halfW < 0.0f && MapX + halfW > ShowMapTexture->GetWidth())
		{
			inL = 0;
			inR = (int)ShowMapTexture->GetWidth();
		}*/

		if (MapY - halfH >= 0.0f && MapY + halfH <= ShowMapTexture->GetHeight())
		{
			inT = (int)(MapY - halfH);
			inB = (int)(MapY + halfH);

			Location->MoveBySize(
				0.0f, 
				(DisplaySize / 2.0f) - (Location->GetHeight() / 2.0f));
		}
		else if (MapY - halfH < 0.0f && MapY + halfH <= ShowMapTexture->GetHeight())
		{
			inT = 0;
			inB = (int)(halfH * 2.0f);

			Location->MoveBySize(
				0.0f, 
				(DisplaySize / 2.0f) - ((halfH - MapY) * (DisplaySize / (halfH * 2.0f))) - (Location->GetHeight() / 2.0f));
		}
		else if (MapY - halfH >= 0.0f && MapY + halfH > ShowMapTexture->GetHeight())
		{
			inT = (int)(ShowMapTexture->GetHeight() - (halfH * 2.0f));
			inB = (int)ShowMapTexture->GetHeight();

			Location->MoveBySize(
				0.0f, 
				(DisplaySize / 2.0f) + (((MapY + halfH) - ShowMapTexture->GetHeight()) * (DisplaySize / (halfH * 2.0f))) - (Location->GetHeight() / 2.0f));
		}/* // not reached
		else //if (MapY - halfH < 0.0f && MapY + halfH > ShowMapTexture->GetHeight())
		{
			inT = 0;
			inB = (int)ShowMapTexture->GetHeight();
		}*/
		
		SetRect(&Zoom, inL, inT, inR, inB);
		ShowMap->SetTexture(
			DisplaySize / (float)(Zoom.right - Zoom.left), 
			DisplaySize / (float)(Zoom.bottom - Zoom.top), 
			ShowMapTexture, &Zoom);
	}

	///

	MouseInfo xMouse = Mouse();

	static float old_cursorX = xMouse.CursorX;
	static float old_cursorY = xMouse.CursorY;

	int cX = (int)((xMouse.CursorX - old_cursorX) / GuiScaleX());
	int cY = (int)((xMouse.CursorY - old_cursorY) / GuiScaleY());

	old_cursorX = xMouse.CursorX;
	old_cursorY = xMouse.CursorY;

	// the mouse has been released, it is no longer being dragged
	if (ShowMap->GetIO() && !xMouse.LButtonDown)
		ShowMap->SetIO(false);

	// move the map around if it has been dragged
	if (ShowMap->GetIO())
	{
		oncourse = false;

		int bcX = cX;
		int bcY = cY;

		cX = (int)(cX * ((float)(Zoom.right - Zoom.left) / DisplaySize));
		cY = (int)(cY * ((float)(Zoom.bottom - Zoom.top) / DisplaySize));

		if (cX == 0 && bcX < 0) cX = -1;
		else if (cX == 0 && bcX > 0) cX = 1;

		if (cY == 0 && bcY < 0) cY = -1;
		else if (cY == 0 && bcY > 0) cY = 1;

		if (Zoom.left - cX < 0 || Zoom.right - cX > ShowMapTexture->GetWidth()) cX = 0;
		if (Zoom.top - cY < 0 || Zoom.bottom - cY > ShowMapTexture->GetHeight()) cY = 0;
		
		SetRect(&Zoom, Zoom.left - cX, Zoom.top - cY, Zoom.right - cX, Zoom.bottom - cY);
		ShowMap->SetTexture(
			DisplaySize / (float)(Zoom.right - Zoom.left), 
			DisplaySize / (float)(Zoom.bottom - Zoom.top), 
			ShowMapTexture, &Zoom);
	}

	///

	// hide the locater if the map has been moved
	if (!oncourse) Location->SetActive(false);

	///

	if (ConfigOn)
	{
		/**
		if (Config window's 'close button' was pushed)
		{
			ConfigOn = false;
		}
		else
		{
			// draw your config window
		}
		/**/
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
		// standardize the zooming by time rather then just frame rate
		static UINT lastime = timeGetTime();
		UINT thistime = timeGetTime();
		UINT timediff = thistime - lastime;
		
		if (ZoomIN->GetIO() && timediff >= 50)
		{
			lastime = timeGetTime();
			bZoomIN = true;
		}
		else
			bZoomIN = false;

		if (ZoomOUT->GetIO() && timediff >= 50)
		{
			lastime = timeGetTime();
			bZoomOUT = true;
		}
		else
			bZoomOUT = false;

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

	// return if in chatmode
	if (*Chatmode == 2)
		return true;

	KeyInfo xKey = Key();

	if (xKey.bDown) // key is pressed down
	{
	}
	else if (!xKey.bRepeat) // key has been released
	{
		if (HIDESHOW_KEY != 0x00 && xKey.cKey == HIDESHOW_KEY)
		{
			SetExtMenuIO(Eye_Map, !CheckExtMenuIO(Eye_Map));
			return false;
		}
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

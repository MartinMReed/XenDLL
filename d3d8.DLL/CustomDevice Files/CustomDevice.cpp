#include "CustomDevice.h"

CustomDevice* g_CustomDevice = NULL;

//-------------------------------------------------------
// 
//-------------------------------------------------------
bool CustomDevice::Initialize(IDirect3DDevice8* device) 
{
	initialized = false;

	d3dDevice = device;

	//InitializeCriticalSection(&m_critical_sec);
	
	////////////////////////////////////////////////////////////////////////////

	directInputHook = new IDirectInput8Hook();
	directInputHook->InstallDInputHook();

	////////////////////////////////////////////////////////////////////////////

	// do a version check to make sure users dont have older versions of HGooey
	if (_HGVersionCheck(HGVersion) == false)
		return Uninitialize();

	if (_Initialize(device) == false)
		return Uninitialize();

	////////////////////////////////////////////////////////////////////////////

	const char* data = NULL;

	// load a memory address
	if (data = _ReadConfigItem("cursorX", "XenDLL Files/Memory.xml"))
	{
		try
		{
			g_cursorX = (float*)atoi(data);
		}
		catch (...) { g_cursorX = NULL; }
	}
	else
	{
		g_cursorX = NULL;
	}

	// load a memory address
	if (data = _ReadConfigItem("cursorY", "XenDLL Files/Memory.xml"))
	{
		try
		{
			g_cursorY = (float*)atoi(data);
		}
		catch (...) { g_cursorY = NULL; }
	}
	else
	{
		g_cursorY = NULL;
	}

	// set a default and then read the config file
	int menuLocation = BOTTOM_RIGHT;

	data = _ReadConfigItem("menuLocation", "XenDLL Files/XenDLL.xml");
	if (data && _stricmp(data, "TOP_LEFT") == 0)
	{
		menuLocation = TOP_LEFT;
	}
	else if (data && _stricmp(data, "TOP_RIGHT") == 0)
	{
		menuLocation = TOP_RIGHT;
	}
	else if (data && _stricmp(data, "BOTTOM_LEFT") == 0)
	{
		menuLocation = BOTTOM_LEFT;
	}
	else if (!data || _stricmp(data, "BOTTOM_RIGHT") != 0)
	{
		_WriteConfigItem("menuLocation", "BOTTOM_RIGHT", "XenDLL Files/XenDLL.xml");
	}

	RECT rect;

	IDDTexture* bmpList = _CreateIDDTextureFromFile(512.0f, 512.0f, "XenDLL Files/Images/list.bmp", COLOR_RED);

	_SetMenuOptions(		// Function
		menuLocation,		// Menu Location (TOP_LEFT / TOP_RIGHT / BOTTOM_LEFT / BOTTOM_RIGHT)
		64.0f,				// Menu Item Width
		64.0f);				// Menu Item Height

	////////////////////////////////////////////////////////////////////////////

	// set a default and then read the config file
	menuToggleKey = 0x00;

	data = _ReadConfigItem("menuToggleKey", "XenDLL Files/XenDLL.xml");
	if (data == NULL || ((menuToggleKey = _convertDIK_Name(data)) == 0x00 && _stricmp(data, "0x00") != 0))
	{
		menuToggleKey = 0x00;
		_WriteConfigItem("menuToggleKey", "0x00", "XenDLL Files/XenDLL.xml");
	}

	////////////////////////////////////////////////////////////////////////////

	SetRect(&rect, 384, 384, 447, 447);
	loader = _CreateExtension(	// Function
		bmpList,				// Texture
		&rect);					// RECT

	///

	SetRect(&rect, DefaultList_left, DefaultList_top, DefaultList_right, DefaultList_bottom);
	float loaderBackWScale = 450.0f / DefaultList_width;
	float loaderBackHScale = 100.0f / DefaultList_height;
	float loaderBackX = (_ScreenWidth() / 2.0f) - ((loaderBackWScale * DefaultList_width) / 2.0f);
	float loaderBackY = 0.0f;
	loaderBackground = _CreateImage(	// Function
		loaderBackWScale,				// Width Scale 0.0f - 1.0f+
		loaderBackHScale,				// Height Scale 0.0f - 1.0f+
		bmpList,						// Texture
		&rect);							// RECT

	_AttachObject2(loader, loaderBackground);

	_Move(loaderBackground, loaderBackX, loaderBackY);

	_SetCancelClick(loaderBackground, true);

	///

	float loaderTitleWScale = ((loaderBackWScale * DefaultList_width) * 0.75f) / TextBar_width;
	float loaderTitleHScale = ((loaderBackHScale * DefaultList_height) * 0.25f) / TextBar_height;
	float loaderTitleX = ((loaderBackWScale * DefaultList_width) - (loaderTitleWScale * TextBar_width)) / 2.0f;
	float loaderTitleY = (loaderTitleHScale * TextBar_height) / 2.0f;
	loaderTitle = _CreateTextBar(	// Function
		"DLL Control", 				// Description
		loaderTitleWScale, 			// Width Scale 0.0f - 1.0f+
		loaderTitleHScale); 		// Height Scale 0.0f - 1.0f+

	_AttachObject1(loaderBackground, loaderTitle);

	_Move(loaderTitle, loaderTitleX, loaderTitleY);

	_SetCancelClick(loaderTitle, true);
	_SetAcceptAction(loaderTitle, false);

	///

	float loadButtonScale = 1.0f;
	loadButton = _CreateButton(	// Function
		" Load ", 				// Description
		loadButtonScale);		// Scale 0.0f - 1.0f+

	_AttachObject1(loaderBackground, loadButton);

	_SetStickyIO(loadButton, true);

	_Move(loadButton,
		(1.0f * ((loaderBackWScale * DefaultList_width) / 6.0f)) - (_GetWidth(loadButton) / 2.0f),
		(loaderBackHScale * DefaultList_height) - (((loaderBackHScale * DefaultList_height) - loaderTitleY - (loaderTitleHScale * TextBar_height)) / 2.0f) - (_GetHeight(loadButton) / 2.0f));

	///

	float unloadButtonScale = 1.0f;
	unloadButton = _CreateButton(	// Function
		"Unload", 					// Description
		unloadButtonScale);			// Scale 0.0f - 1.0f+

	_AttachObject1(loaderBackground, unloadButton);

	_SetStickyIO(unloadButton, true);

	_Move(unloadButton,
		(3.0f * ((loaderBackWScale * DefaultList_width) / 6.0f)) - (_GetWidth(unloadButton) / 2.0f),
		(loaderBackHScale * DefaultList_height) - (((loaderBackHScale * DefaultList_height) - loaderTitleY - (loaderTitleHScale * TextBar_height)) / 2.0f) - (_GetHeight(unloadButton) / 2.0f));

	///

	float configButtonScale = 1.0f;
	configButton = _CreateButton(	// Function
		"Config", 					// Description
		configButtonScale);			// Scale 0.0f - 1.0f+

	_AttachObject1(loaderBackground, configButton);

	_SetStickyIO(configButton, true);

	_Move(configButton,
		(5.0f * ((loaderBackWScale * DefaultList_width)/6.0f)) - (_GetWidth(configButton) / 2.0f),
		(loaderBackHScale * DefaultList_height) - (((loaderBackHScale * DefaultList_height) - loaderTitleY - (loaderTitleHScale * TextBar_height)) / 2.0f) - (_GetHeight(configButton) / 2.0f));

	///

	loadList = _CreateDefaultList(	// Function
		"Load DLL",					// Description
		1.0f, 						// Scale 0.0f - 1.0f+
		false);						// Allow Duplicates

	_AttachObject2(loader, loadList);

	_SetMoveable(loadList, MOVE_LIMITED);

	_Move(loadList,
		(_ScreenWidth() / 2.0f) - (_GetWidth(loadList) / 2.0f),
		(_ScreenHeight() / 2.0f) - (_GetHeight(loadList) / 2.0f));

	///

	unloadList = _CreateDefaultList(	// Function
		"Unload DLL",					// Description
		1.0f, 							// Scale 0.0f - 1.0f+
		false);							// Allow Duplicates

	_AttachObject2(loader, unloadList);

	_SetMoveable(unloadList, MOVE_LIMITED);

	_Move(unloadList,
		(_ScreenWidth() / 2.0f) - (_GetWidth(unloadList) / 2.0f),
		(_ScreenHeight() / 2.0f) - (_GetHeight(unloadList) / 2.0f));

	///

	configList = _CreateDefaultList(	// Function
		"DLL Settings",					// Description
		1.0f, 							// Scale 0.0f - 1.0f+
		false);							// Allow Duplicates

	_AttachObject2(loader, configList);

	_SetMoveable(configList, MOVE_LIMITED);

	_Move(configList,
		(_ScreenWidth() / 2.0f) - (_GetWidth(configList) / 2.0f),
		(_ScreenHeight() / 2.0f) - (_GetHeight(configList) / 2.0f));

	////////////////////////////////////////////////////////////////////////////

	data = _ReadConfigItem("Computron", "XenDLL Files/XenDLL.xml");
	if (data && _stricmp(data, "true") == 0)
	{
		g_Computron = new Computron();
		if (g_Computron && g_Computron->Initialize() == false)
		{
			delete g_Computron;
			g_Computron = NULL;
		}
	}
	
	if (data == NULL)
	{
		_WriteConfigItem("Computron", "false", "XenDLL Files/XenDLL.xml");
	}


	data = _ReadConfigItem("Screenshot", "XenDLL Files/XenDLL.xml");
	if (data == NULL || _stricmp(data, "true") == 0)
	{
		g_Screenshot = new Screenshot();
		if (g_Screenshot && g_Screenshot->Initialize() == false)
		{
			delete g_Screenshot;
			g_Screenshot = NULL;
		}
	}
	
	if (data == NULL)
	{
		_WriteConfigItem("Screenshot", "true", "XenDLL Files/XenDLL.xml");
	}


	data = _ReadConfigItem("AutoClick", "XenDLL Files/XenDLL.xml");
	if (data == NULL || _stricmp(data, "true") == 0)
	{
		g_AutoClick = new AutoClick();
		if (g_AutoClick && g_AutoClick->Initialize() == false)
		{
			delete g_AutoClick;
			g_AutoClick = NULL;
		}
	}
	
	if (data == NULL)
	{
		_WriteConfigItem("AutoClick", "true", "XenDLL Files/XenDLL.xml");
	}

	////////////////////////////////////////////////////////////////////////////

	initialized = true;

	LoadSavedList();

	SearchDLL();

	return true;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::LoadSavedList(void)
{
	if (initialized == false) return;

	TiXmlDocument document;
	TiXmlElement* root = NULL;
	int version;

	if (!document.LoadFile("XenDLL Files/Extensions/load.xml") ||
		!(root = document.FirstChildElement("loader")) ||
		!root->Attribute("version", &version) || version != XML_VERSION)
	{
		return;
	}

	TiXmlElement* element = root->FirstChildElement("dll");

	while (element)
	{
		if (element->GetText())
			LoadDLL((char*)(element->GetText()));

		element = element->NextSiblingElement();
	}
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::SaveLoadedList(void)
{
	if (initialized == false) return;

	TiXmlDocument document;
	TiXmlElement* root = new TiXmlElement("loader");
	root->SetAttribute("version", XML_VERSION);

	FILE* out = NULL;
	fopen_s(&out, "XenDLL Files/Extensions/load.xml", "w");

	if (out)
	{
		//EnterCriticalSection(&m_critical_sec);
		
			FuncList* extListLoop = extListFirst;

			while (extListLoop)
			{
				TiXmlElement* element = new TiXmlElement("dll");
				TiXmlText* text = new TiXmlText(extListLoop->GetName());
				
				element->InsertEndChild(*text);
				root->InsertEndChild(*element);

				extListLoop = extListLoop->GetNext();
			}

		//LeaveCriticalSection(&m_critical_sec);

		document.InsertEndChild(*root);

		document.Print(out, 0);

		fclose(out);
	}
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::LostDevice(void)
{
	if (initialized == false) return;

	//EnterCriticalSection(&m_critical_sec);

		_LostDevice();
	
		FuncList* extListLoop = extListFirst;

		while (extListLoop)
		{
			extListLoop->f_LostDevice();
			extListLoop = extListLoop->GetNext();
		}

		if (g_Screenshot && g_Screenshot->initialized)
			g_Screenshot->LostDevice();

	//LeaveCriticalSection(&m_critical_sec);
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::ResetDevice(void)
{
	if (initialized == false) return;

	//EnterCriticalSection(&m_critical_sec);

		_ResetDevice();
	
		FuncList* extListLoop = extListFirst;

		while (extListLoop)
		{
			extListLoop->f_ResetDevice();
			extListLoop = extListLoop->GetNext();
		}

		if (g_Screenshot && g_Screenshot->initialized)
			g_Screenshot->ResetDevice();

	//LeaveCriticalSection(&m_critical_sec);
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::UnloadDLL(char* _filename)
{
	if (initialized == false) return;

	if (_filename == NULL) return;

	//EnterCriticalSection(&m_critical_sec);

		FuncList* extListLoop = extListFirst;

		while (extListLoop)
		{
			if (_stricmp(extListLoop->GetName(), _filename) == 0)
			{
				if (extListFirst == extListLoop)
					extListFirst = extListLoop->GetNext();

				if (extListLast == extListLoop)
					extListLast = extListLoop->GetPrev();
				
				delete extListLoop;
				extListLoop = NULL;

				char* temp = _restring(_filename, 0, strlen(_filename));
				_LoadString(_GetListViewer(loadList), temp);
				_UnloadString(_GetListViewer(unloadList), temp);
				_UnloadString(_GetListViewer(configList), temp);

				break;
			}
			else
			{
				extListLoop = extListLoop->GetNext();
			}
		}
	
	//LeaveCriticalSection(&m_critical_sec);

	SaveLoadedList();
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::LoadDLL(char* _filename)
{
	if (initialized == false) return;

	if (_filename == NULL || _stricmp(_filename, "HG_SDK") == 0) return;

	//EnterCriticalSection(&m_critical_sec);

		char dllPath[MAX_PATH];
		GetCurrentDirectory(sizeof(dllPath), dllPath);
		
		sprintf_s(dllPath, sizeof(dllPath), "%s/XenDLL Files/Extensions/%s/HG_SDK.dll", dllPath, _filename);

		if (GetFileAttributes(dllPath) == 0xFFFFFFFF)
		{
			//LeaveCriticalSection(&m_critical_sec);
			return;
		}

		FuncList* extListNew = new FuncList();

		extListNew->SetMod(dllPath);
		if (extListNew->GetMod() == NULL)
		{
			delete extListNew;
			//LeaveCriticalSection(&m_critical_sec);
			return;
		}

		VersionFunc f_HGSDKVersionCheck = (VersionFunc)GetProcAddress(extListNew->GetMod(), "HGSDKVersionCheck");

		if (f_HGSDKVersionCheck == NULL || (f_HGSDKVersionCheck() < HGSDKVersion))
		{
			delete extListNew;
			//LeaveCriticalSection(&m_critical_sec);
			return;
		}

		extListNew->SetName(_restring(_filename, 0, strlen(_filename)));

		extListNew->f_Config = (voidFunc)GetProcAddress(extListNew->GetMod(), "Config");

		extListNew->f_LostDevice = (voidFunc)GetProcAddress(extListNew->GetMod(), "LostDevice");
		extListNew->f_ResetDevice = (voidFunc)GetProcAddress(extListNew->GetMod(), "ResetDevice");

		extListNew->f_PreRender = (voidFunc)GetProcAddress(extListNew->GetMod(), "PreRender");
		extListNew->f_Render = (voidFunc)GetProcAddress(extListNew->GetMod(), "Render");

		extListNew->f_HandleMouse = (boolFunc)GetProcAddress(extListNew->GetMod(), "HandleMouse");
		extListNew->f_HandleKeyboard = (boolFunc)GetProcAddress(extListNew->GetMod(), "HandleKeyboard");

		if (!extListNew->GetName() ||
			!extListNew->f_Config ||
			!extListNew->f_LostDevice ||
			!extListNew->f_ResetDevice ||
			!extListNew->f_PreRender ||
			!extListNew->f_Render ||
			!extListNew->f_HandleMouse ||
			!extListNew->f_HandleKeyboard)
		{
			delete extListNew;
			//LeaveCriticalSection(&m_critical_sec);
			return;
		}

		if (extListFirst == NULL)
		{
			extListNew->SetPrev(NULL);
			extListFirst = extListNew;
			extListLast = extListNew;
		}
		else
		{
			extListNew->SetPrev(extListLast);
			extListLast->SetNext(extListNew);
			extListLast = extListNew;
		}

		char* temp = _restring(_filename, 0, strlen(_filename));
		_LoadString(_GetListViewer(unloadList), temp);
		_LoadString(_GetListViewer(configList), temp);
		_UnloadString(_GetListViewer(loadList), temp);

	//LeaveCriticalSection(&m_critical_sec);

	SaveLoadedList();
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::EndScene(void) 
{
	if (initialized == false) return;

	//////////////////////////////////////////

	FuncList* extListLoop = NULL;

	//EnterCriticalSection(&m_critical_sec);

		_EnableDraw(false);
	
		extListLoop = extListFirst;

		while (extListLoop)
		{
			extListLoop->f_PreRender();
			extListLoop = extListLoop->GetNext();
		}

		if (g_Screenshot && g_Screenshot->initialized && !g_Screenshot->includeGUI)
			g_Screenshot->PreRender();

		_EnableDraw(true);

		///

		extListLoop = extListFirst;
		
		while (extListLoop)
		{
			extListLoop->f_Render();
			extListLoop = extListLoop->GetNext();
		}

		if (g_Computron && g_Computron->initialized)
		{
			g_Computron->Render();
		}

		if (g_Screenshot && g_Screenshot->initialized && !g_Screenshot->includeGUI)
			g_Screenshot->Render();

		///

		extListLoop = extListFirst;

		while (extListLoop)
		{
			if (extListLoop->GetConfigOn())
			{
				extListLoop->f_Config();
				extListLoop->SetConfigOn(false);
			}

			extListLoop = extListLoop->GetNext();
		}

	//LeaveCriticalSection(&m_critical_sec);


	//////////////////////////////////////////


	bool displayLoader = _CheckExtMenuIO(loader) && _isOpen();
	if (!displayLoader) _SetExtMenuIO(loader, false);


	//////////////////////////////////////////


	int countIO = 0;

	if (_GetIO(loadButton))
		countIO++;
	if (_GetIO(unloadButton))
		countIO++;
	if (_GetIO(configButton))
		countIO++;

	static int lastIO = 0;

	if (_GetIO(loadButton) && (countIO == 1 || lastIO == 2 || lastIO == 3))
	{
		lastIO = 1;
		_ActivateFull1(loadList, true);
		_ActivateFull1(unloadList, false);
		_ActivateFull1(configList, false);
		_SetIO(loadButton, true);
		_SetIO(unloadButton, false);
		_SetIO(configButton, false);
	}
	else if (_GetIO(unloadButton) && (countIO == 1 || lastIO == 1 || lastIO == 3))
	{
		lastIO = 2;
		_ActivateFull1(unloadList, true);
		_ActivateFull1(loadList, false);
		_ActivateFull1(configList, false);
		_SetIO(unloadButton, true);
		_SetIO(loadButton, false);
		_SetIO(configButton, false);
	}
	else if (_GetIO(configButton) && (countIO == 1 || lastIO == 1 || lastIO == 2))
	{
		lastIO = 3;
		_ActivateFull1(configList, true);
		_ActivateFull1(loadList, false);
		_ActivateFull1(unloadList, false);
		_SetIO(configButton, true);
		_SetIO(loadButton, false);
		_SetIO(unloadButton, false);
	}
	else
	{
		lastIO = 0;
		_ActivateFull1(loadList, false);
		_ActivateFull1(unloadList, false);
		_ActivateFull1(configList, false);
		_SetIO(loadButton, false);
		_SetIO(unloadButton, false);
		_SetIO(configButton, false);
	}


	//////////////////////////////////////////


	// show .dll control
	if (displayLoader)
	{
		_ActivateFull1(loaderBackground, true);
	}
	else if (displayLoader == false)
	{
		_ActivateFull2(loader, false);
		_SetIO(loadButton, false);
		_SetIO(unloadButton, false);
		_SetIO(configButton, false);
	}


	//////////////////////////////////////////


	// load .dll
	if (_GetIO(loadButton) && _GetIO(_GetButtonOK(loadList)))
	{
		LoadDLL(_GetSelected(_GetListViewer(loadList)));
		_SetIO(_GetButtonOK(loadList), false);
		
	}
	else if (_GetIO(loadButton) && _GetIO(_GetButtonCancel(loadList)))
	{
		_SetIO(loadButton, false);
		_ActivateFull1(loadList, false);
		_SetIO(_GetButtonCancel(loadList), false);
	}

	// unload .dll
	if (_GetIO(unloadButton) && _GetIO(_GetButtonOK(unloadList)))
	{
		UnloadDLL(_GetSelected(_GetListViewer(unloadList)));
		_SetIO(_GetButtonOK(unloadList), false);
	}
	else if (_GetIO(unloadButton) && _GetIO(_GetButtonCancel(unloadList)))
	{
		_SetIO(unloadButton, false);
		_ActivateFull1(unloadList, false);
		_SetIO(_GetButtonCancel(unloadList), false);
	}

	// .dll config
	if (_GetIO(configButton) && _GetIO(_GetButtonOK(configList)))
	{
		char* temp = _GetSelected(_GetListViewer(configList));

		//EnterCriticalSection(&m_critical_sec);
		
			extListLoop = extListFirst;

			while (extListLoop)
			{
				if (_stricmp(temp,extListLoop->GetName()) == 0)
				{
					extListLoop->SetConfigOn(true);
					break;
				}

				extListLoop = extListLoop->GetNext();
			}

		//LeaveCriticalSection(&m_critical_sec);

		_SetIO(_GetButtonOK(configList), false);
	}
	else if (_GetIO(configButton) && _GetIO(_GetButtonCancel(configList)))
	{
		_SetIO(configButton, false);
		_ActivateFull1(configList, false);
		_SetIO(_GetButtonCancel(configList), false);
	}


	//////////////////////////////////////////

	if (g_cursorX && g_cursorY)
	{
		_EndScene(*g_cursorX, *g_cursorY);
	}
	else
	{
		_EndScene();
	}

	// include the GUI in the screenshot
	_EnableDraw(false);

	if (g_Screenshot && g_Screenshot->initialized && g_Screenshot->includeGUI)
		g_Screenshot->PreRender();

	_EnableDraw(true);

	if (g_Screenshot && g_Screenshot->initialized && g_Screenshot->includeGUI)
		g_Screenshot->Render();

	//////////////////////////////////////////
 }



//-------------------------------------------------------
// 
//-------------------------------------------------------
bool CustomDevice::HandleKeyboard(BYTE cKey, char* buffer, bool bDown, bool bRepeat)
{
	if (initialized == false) return true;

	bool bReturn = true;

	//EnterCriticalSection(&m_critical_sec);

		if (_HandleKeyboard(cKey, buffer, bDown, bRepeat) == false)
			bReturn = false;

		FuncList* extListLoop = extListFirst;

		while (extListLoop)
		{
			if (extListLoop->f_HandleKeyboard() == false)
				bReturn = false;

			extListLoop = extListLoop->GetNext();
		}

		if (g_Computron && g_Computron->initialized)
		{
			if (g_Computron->HandleKeyboard() == false)
				bReturn = false;
		}

		if (g_Screenshot && g_Screenshot->initialized)
		{
			if (g_Screenshot->HandleKeyboard() == false)
				bReturn = false;
		}

		if (g_AutoClick && g_AutoClick->initialized)
		{
			if (g_AutoClick->HandleKeyboard() == false)
				bReturn = false;
		}

	//LeaveCriticalSection(&m_critical_sec);

	if (!bDown && !bRepeat && cKey == menuToggleKey)
	{
		_SetExtMenuIO(loader, false);
		_ToggleMenu();
		bReturn = false;
	}

	if (bReturn == false)
	{
		buffer[cKey] = 0x0;

		//memset(buffer, 0, sizeof(buffer)); // suppress all actions
	}

	return bReturn;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
bool CustomDevice::HandleMouse(int cState, DIMOUSESTATE2* mouse)
{
	if (initialized == false) return true;

	bool bReturn = true;

	//EnterCriticalSection(&m_critical_sec);

		if (_HandleMouse(cState, mouse) == false)
			bReturn = false;

		FuncList* extListLoop = extListFirst;

		while (extListLoop)
		{
			if (extListLoop->f_HandleMouse() == false)
				bReturn = false;

			extListLoop = extListLoop->GetNext();
		}

	//LeaveCriticalSection(&m_critical_sec);

	if (bReturn == false)
	{
		if (cState == 0) // mouse move
		{
			mouse->lX = 0;
			mouse->lY = 0;
		}
		else if (cState == 1) // left button down
		{
			mouse->rgbButtons[MOUSEBUTTON_LEFT] = 0x0;
		}
		else if (cState == 2) // left button up
		{
			mouse->rgbButtons[MOUSEBUTTON_LEFT] = 0x0;
		}
		else if (cState == 3) // middle button down
		{
			mouse->rgbButtons[MOUSEBUTTON_MIDDLE] = 0x0;
		}
		else if (cState == 4) // middle button up
		{
			mouse->rgbButtons[MOUSEBUTTON_MIDDLE] = 0x0;
		}
		else if (cState == 5) // right button down
		{
			mouse->rgbButtons[MOUSEBUTTON_RIGHT] = 0x0;
		}
		else if (cState == 6) // right button up
		{
			mouse->rgbButtons[MOUSEBUTTON_RIGHT] = 0x0;
		}
		else // mouse scroll
		{
			mouse->lZ = 0;
		}

		//memset(mouse, 0, sizeof(DIMOUSESTATE2));
	}

	return bReturn;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::SearchDLL(void)
{
	char* fileType = "XenDLL Files/Extensions/*";

    // Declare all needed handles     
    WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(fileType, &findFileData);
	
    // Use a do/while so we process whatever FindFirstFile returned     
    do     
    {        
        // Is it valid?         
        if (hFind != INVALID_HANDLE_VALUE)         
        {           
            // Is it a . or .. directory? If it is, skip, or we'll go forever.             
            if ((_stricmp(findFileData.cFileName, ".") == 0) || 
				(_stricmp(findFileData.cFileName, "..") == 0))
                continue;
			
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char path[MAX_PATH];
				sprintf_s(path, MAX_PATH, "XenDLL Files/Extensions/%s/HG_SDK.dll", findFileData.cFileName);

				if (!_ContainsString(_GetListViewer(loadList), findFileData.cFileName) &&
					!_ContainsString(_GetListViewer(unloadList), findFileData.cFileName) &&
					_stricmp(findFileData.cFileName, "HG_SDK") != 0 && 
					GetFileAttributes(path) != 0xFFFFFFFF)
				{
					_LoadString(_GetListViewer(loadList), findFileData.cFileName);
				}
			}
        }  
    }
    while (FindNextFile(hFind, &findFileData) && hFind != INVALID_HANDLE_VALUE);

    FindClose(hFind);
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::OnKeyEvent(char* buffer) 
{
   unsigned int nDelay = 300, nRepeat = 100;	// 500, 30 is my windows setting 
												// use getsystemparametersinfo() 
												// to get windows settings 

	for (int n = INPUT_OFFSET; n < INPUT_COUNT; n++) // read entire buffer
	{
		if (KEYDOWN(buffer, n - INPUT_OFFSET)) // key down
		{
			inputInfo[n].bReleased = false;

			if (inputInfo[n].nTime == 0)
				inputInfo[n].nTime = timeGetTime();

			if (inputInfo[n].bRepeat)
			{
				if (timeGetTime() - inputInfo[n].nTime > nRepeat) // repeat delay
				{
					inputInfo[n].nTime = timeGetTime();
					
					if (HandleKeyboard((BYTE)(n - INPUT_OFFSET), buffer, false, true) == false)
					{
						return;
					}
				}
			}
			else if (timeGetTime() - inputInfo[n].nTime > nDelay) // start delay
			{
				inputInfo[n].bRepeat = true;
			}

			HandleKeyboard((BYTE)(n - INPUT_OFFSET), buffer, true, false); // so you can suppress keys"
		}
		else if (inputInfo[n].bReleased == false) // key up
		{
			inputInfo[n].bReleased = true;
			inputInfo[n].bRepeat = false;
			inputInfo[n].nTime = 0;

			HandleKeyboard((BYTE)(n - INPUT_OFFSET), buffer, false, false);
		}
	}
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void CustomDevice::OnMouseEvent(DIMOUSESTATE2* mouse)
{
	bool bReturn = true;

	if (mouse->lX != 0 || mouse->lY != 0)
	{
		bReturn = HandleMouse(0, mouse); // mouse move
	}

	if (bReturn == false) return;

	if (MOUSEDOWN(*mouse, MOUSEBUTTON_LEFT))
	{
		inputInfo[0].bReleased = false;
		bReturn = HandleMouse(1, mouse); // left button down
	}
	else if (inputInfo[0].bReleased == false)
	{
		inputInfo[0].bReleased = true;
		bReturn = HandleMouse(2, mouse); // left button up
	}

	if (bReturn == false) return;

	if (MOUSEDOWN(*mouse, MOUSEBUTTON_MIDDLE))
	{
		inputInfo[1].bReleased = false;
		bReturn = HandleMouse(3, mouse); // middle button down
	}
	else if (inputInfo[1].bReleased == false)
	{
		inputInfo[1].bReleased = true;
		bReturn = HandleMouse(4, mouse); // middle button up
	}

	if (bReturn == false) return;

	if (MOUSEDOWN(*mouse, MOUSEBUTTON_RIGHT))
	{
		inputInfo[2].bReleased = false;
		bReturn = HandleMouse(5, mouse); // right button down
	}
	else if (inputInfo[2].bReleased == false)
	{
		inputInfo[2].bReleased = true;
		bReturn = HandleMouse(6, mouse); // right button up
	}

	if (bReturn == false) return;

	if (mouse->lZ != 0)
		HandleMouse(((int)mouse->lZ / MOUSEWHEEL_DELTA) * 7, mouse);
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
CustomDevice::CustomDevice(void)
{
	initialized = false;

	g_cursorX = NULL;
	g_cursorY = NULL;

	extListFirst = NULL;
	extListLast = NULL;

	extListFirst = NULL;
	extListLast = NULL;

	loader = NULL;
	loaderBackground = NULL;
	loaderTitle = NULL;
	loadButton = NULL;
	unloadButton = NULL;
	configButton = NULL;
	loadList = NULL;
	unloadList = NULL;
	configList = NULL;

	directInputHook = NULL;
	d3dDevice = NULL;

	for (int i = 0; i < INPUT_COUNT; i++)
	{
		inputInfo[i].nTime = 0;
		inputInfo[i].bRepeat = false;
		inputInfo[i].bReleased = true;
	}
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
bool CustomDevice::Uninitialize(void)
{
	initialized = false;
	
	///

	if (g_Computron)
	{
		delete g_Computron;
		g_Computron = NULL;
	}

	if (g_Screenshot)
	{
		delete g_Screenshot;
		g_Screenshot = NULL;
	}

	if (g_AutoClick)
	{
		delete g_AutoClick;
		g_AutoClick = NULL;
	}

	///

	//EnterCriticalSection(&m_critical_sec);

		FuncList* extListLoop = extListFirst;

		while (extListLoop)
		{
			extListFirst = extListLoop->GetNext();
			delete extListLoop;
			extListLoop = extListFirst;
		}

	//LeaveCriticalSection(&m_critical_sec);

	//DeleteCriticalSection(&m_critical_sec);

	if (directInputHook)
	{
		delete directInputHook;
		directInputHook = NULL;
	}

	if (d3dDevice)
	{
		d3dDevice->Release();
		d3dDevice = NULL;
	}

	return initialized;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
CustomDevice::~CustomDevice(void)
{
	Uninitialize();
}

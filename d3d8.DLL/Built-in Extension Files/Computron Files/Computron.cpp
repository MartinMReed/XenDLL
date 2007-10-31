#include "WinPcap Files/WinPcap.h"
#include "../../CustomDevice Files/CustomDevice.h"

unsigned threadA;
unsigned __stdcall RunThreadA(void* params);

unsigned threadB;
unsigned __stdcall RunThreadB(void* params);

Computron* g_Computron = NULL;


//-------------------------------------------------------
// 
//-------------------------------------------------------
bool Computron::Initialize(void)
{
	initialized = false;

	const char* data = NULL;
	char buffer[256];

	// make sure the menu item texture exists
	if (GetFileAttributes("XenDLL Files/Extensions/Computron/Computron.bmp") == 0xFFFFFFFF)
		return Uninitialize();

	// create the texture for the menu item
	IDDTexture* bmpComputron = _CreateIDDTextureFromFile(64.0f, 64.0f, "XenDLL Files/Extensions/Computron/Computron.bmp", COLOR_RED);

	// create the extension
	extComputron = _CreateExtension(	// Function
		bmpComputron,						// Texture
		NULL);							// Rect

	// load the extension menu item io information
	data = _ReadConfigItem("MenuItemIO", "XenDLL Files/Extensions/Computron/Computron.xml");
	if (data && _stricmp(data, "true") == 0)
	{
		_SetExtMenuIO(extComputron, true);
	}
	else
	{
		_SetExtMenuIO(extComputron, false);
	}

	// if the folder does not exist, create it
	if (GetFileAttributes("XenDLL Files/Extensions/Computron/Scripts/") == 0xFFFFFFFF)
		CreateDirectory("XenDLL Files/Extensions/Computron/Scripts/", NULL);

	// open an internet connection
	g_hInet = InternetOpen(
		"computron",
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0);

	// connect to the server
	if (g_hInet)
	{
		g_hInetConnection = InternetConnect(
			g_hInet,
			"calc.xentales.com",
			INTERNET_DEFAULT_HTTP_PORT,
			NULL,
			NULL,
			INTERNET_SERVICE_HTTP,
			INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE,
			0);
	}

	if (g_hInet == NULL || g_hInetConnection == NULL)
	{
		return Uninitialize();
	}

	// load the script filename
	if (data = _ReadConfigItem("scriptFilename", "XenDLL Files/Extensions/Computron/Computron.xml"))
	{
		scriptFilename = _restring(data, 0, strlen(data));
	}
	else
	{
		_WriteConfigItem("scriptFilename", scriptFilename, "XenDLL Files/Extensions/Computron/Computron.xml");
	}

	// load the user handle
	if (data = _ReadConfigItem("scriptHandle", "XenDLL Files/Extensions/Computron/Computron.xml"))
	{
		scriptHandle = _restring(data, 0, strlen(data));
	}
	else
	{
		_WriteConfigItem("scriptHandle", scriptHandle, "XenDLL Files/Extensions/Computron/Computron.xml");
	}

	// load the user password
	if (data = _ReadConfigItem("scriptPassword", "XenDLL Files/Extensions/Computron/Computron.xml"))
	{
		scriptPassword = _restring(data, 0, strlen(data));
	}
	else
	{
		_WriteConfigItem("scriptPassword", scriptPassword, "XenDLL Files/Extensions/Computron/Computron.xml");
	}

	char* badLogin = NULL;

	// send login information to the server
	sprintf_s(buffer, sizeof(buffer), "xendll/computron/login.game.php?handle=%s&password=%s", scriptHandle, scriptPassword);
	string receivedData = ReceiveData(buffer);
	const char* loginData = receivedData.c_str();

	// check the response
	if (_stricmp(loginData, "b63265c00d6eaf05a6a6b20511247bfd") == 0)
	{
		// send script information to the server
		sprintf_s(buffer, sizeof(buffer), "xendll/computron/download.script.php?script=%s&handle=%s&password=%s", scriptFilename, scriptHandle, scriptPassword);
		receivedData = ReceiveData(buffer);
		const char* scriptData = receivedData.c_str();

		if (_stricmp(scriptData, "Failed to connect!") == 0)
		{
			badLogin = "Failed to connect!";
		}
		else if (_stricmp(scriptData, "Incorrect login.") == 0)
		{
			badLogin = "Incorrect login.";
		}
		else if (_stricmp(scriptData, "Script not found.") == 0)
		{
			badLogin = "Script not found.";
		}
		else
		{
			// write the script to a file
			FILE* out = NULL;
			sprintf_s(buffer, sizeof(buffer), "XenDLL Files/Extensions/Computron/Scripts/%s.xml", scriptFilename);
			fopen_s(&out, buffer, "w");

			if (out)
			{
				fprintf(out, "%s", scriptData);
				fclose(out);
			}
		}
	}
	else
	{
		if (_stricmp(loginData, "Failed to connect!") == 0)
		{
			badLogin = "Failed to connect!";
		}
		else
		{
			badLogin = "Incorrect login.";
		}
	}

	// check the response
	if (badLogin != NULL)
	{
		// tell the user what went wrong
		p1TextBar1 = _CreateTextBar(
			badLogin,
			1.0f,
			1.0f);

		_AttachObject2(extComputron, p1TextBar1);
		
		_Move(
			p1TextBar1, 
			(_ScreenWidth() - _GetWidth(p1TextBar1)) / 2.0f, 
			(_ScreenHeight() - _GetHeight(p1TextBar1)) / 2.0f);

		logout = true;

		initialized = true;
	}
	else
	{

		// load a memory address
		if (data = _ReadConfigItem("progressXP", "XenDLL Files/Memory.xml"))
		{
			try
			{
				g_progressXP = (BYTE*)atoi(data);
			}
			catch (...) { return Uninitialize(); }
		}
		else
		{
			return Uninitialize();
		}

		// load a memory address
		if (data = _ReadConfigItem("progressMP", "XenDLL Files/Memory.xml"))
		{
			try
			{
				g_progressMP = (BYTE*)atoi(data);
			}
			catch (...) { return Uninitialize(); }
		}
		else
		{
			return Uninitialize();
		}

		// load a memory address
		if (data = _ReadConfigItem("progressHP", "XenDLL Files/Memory.xml"))
		{
			try
			{
				g_progressHP = (BYTE*)atoi(data);
			}
			catch (...) { return Uninitialize(); }
		}
		else
		{
			return Uninitialize();
		}

		// load a memory address
		if (data = _ReadConfigItem("locationX", "XenDLL Files/Memory.xml"))
		{
			try
			{
				g_locationX = (DWORD*)atoi(data);
			}
			catch (...) { return Uninitialize(); }
		}
		else
		{
			return Uninitialize();
		}

		// load a memory address
		if (data = _ReadConfigItem("locationY", "XenDLL Files/Memory.xml"))
		{
			try
			{
				g_locationY = (DWORD*)atoi(data);
			}
			catch (...) { return Uninitialize(); }
		}
		else
		{
			return Uninitialize();
		}

		// load start/play key
		data = _ReadConfigItem("pauseRestartKey", "XenDLL Files/Extensions/Computron/Computron.xml");
		if (data == NULL || (pauseRestartKey = _convertDIK_Name(data)) == 0x00)
		{
			pauseRestartKey = 0x00;
			_WriteConfigItem("pauseRestartKey", "0x00", "XenDLL Files/Extensions/Computron/Computron.xml");
		}

		// load pause/play key
		data = _ReadConfigItem("pauseResumeKey", "XenDLL Files/Extensions/Computron/Computron.xml");
		if (data == NULL || (pauseResumeKey = _convertDIK_Name(data)) == 0x00)
		{
			pauseResumeKey = 0x00;
			_WriteConfigItem("pauseResumeKey", "0x00", "XenDLL Files/Extensions/Computron/Computron.xml");
		}

		////////////////////////////////////////////////////////////////////////////

		char currentDirectory[MAX_PATH];
		GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);

		char WanPacketDll[MAX_PATH];
		char PacketDll[MAX_PATH];
		char pthreadVCDll[MAX_PATH];
		char wpcapDll[MAX_PATH];
			
		sprintf_s(WanPacketDll, sizeof(WanPacketDll), "%s/XenDLL Files/Extensions/Computron/WinPcap/WanPacket.dll", currentDirectory);
		sprintf_s(PacketDll, sizeof(PacketDll), "%s/XenDLL Files/Extensions/Computron/WinPcap/Packet.dll", currentDirectory);
		sprintf_s(pthreadVCDll, sizeof(pthreadVCDll), "%s/XenDLL Files/Extensions/Computron/WinPcap/pthreadVC.dll", currentDirectory);
		sprintf_s(wpcapDll, sizeof(wpcapDll), "%s/XenDLL Files/Extensions/Computron/WinPcap/wpcap.dll", currentDirectory);

		if (GetFileAttributes(WanPacketDll) == 0xFFFFFFFF ||
			GetFileAttributes(PacketDll) == 0xFFFFFFFF ||
			GetFileAttributes(pthreadVCDll) == 0xFFFFFFFF ||
			GetFileAttributes(wpcapDll) == 0xFFFFFFFF)
			return Uninitialize();
		
		if (!(WanPacketMod = LoadLibrary(WanPacketDll)) ||
			!(PacketMod = LoadLibrary(PacketDll)) ||
			!(pthreadVCMod = LoadLibrary(pthreadVCDll)) ||
			!(wpcapMod = LoadLibrary(wpcapDll)))
			return Uninitialize();

		if (!(_pcap_createsrcstr = (f_pcap_createsrcstr)GetProcAddress(wpcapMod, "pcap_createsrcstr")) ||
			!(_pcap_open = (f_pcap_open)GetProcAddress(wpcapMod, "pcap_open")) ||
			!(_pcap_datalink = (f_pcap_datalink)GetProcAddress(wpcapMod, "pcap_datalink")) ||
			!(_pcap_sendqueue_alloc = (f_pcap_sendqueue_alloc)GetProcAddress(wpcapMod, "pcap_sendqueue_alloc")) ||
			!(_pcap_next_ex = (f_pcap_next_ex)GetProcAddress(wpcapMod, "pcap_next_ex")) ||
			!(_pcap_sendqueue_queue = (f_pcap_sendqueue_queue)GetProcAddress(wpcapMod, "pcap_sendqueue_queue")) ||
			!(_pcap_sendqueue_destroy = (f_pcap_sendqueue_destroy)GetProcAddress(wpcapMod, "pcap_sendqueue_destroy")) ||
			!(_pcap_sendqueue_transmit = (f_pcap_sendqueue_transmit)GetProcAddress(wpcapMod, "pcap_sendqueue_transmit")) ||
			!(_pcap_geterr = (f_pcap_geterr)GetProcAddress(wpcapMod, "pcap_geterr")) ||
			!(_pcap_close = (f_pcap_close)GetProcAddress(wpcapMod, "pcap_close")))
			return Uninitialize();

		////////////////////////////////////////////////////////////////////////////

		IDDTexture* bmpList = _CreateIDDTextureFromFile(512.0f, 512.0f, "XenDLL Files/Images/list.bmp", COLOR_RED);

		RECT rect;

		SetRect(&rect, DefaultList_left, DefaultList_top, DefaultList_right, DefaultList_bottom);
		panel1 = _CreateImage(
			0.85f, 
			1.0f,  
			bmpList, 
			&rect);

		_AttachObject2(extComputron, panel1);
		
		float p1Width = _GetWidth(panel1) / _GetWidthScale(panel1);
		float p1Height = _GetHeight(panel1) / _GetHeightScale(panel1);

		_SetMoveable(panel1, MOVE_ALWAYS);
		
		_Move(
			panel1, 
			10.0f, 
			10.0f);
		
		p1Width = _GetWidth(panel1);
		p1Height = _GetHeight(panel1);

		///

		p1TextBar1 = _CreateTextBar(	// Function
			"Computron", 				// Description
			1.0f, 						// Width Scale 0.0f - 1.0f+
			1.0f); 						// Height Scale 0.0f - 1.0f+

		_AttachObject1(panel1, p1TextBar1);
		
		float tb1Width = _GetWidth(p1TextBar1) / _GetWidthScale(p1TextBar1);
		float tb1Height = _GetHeight(p1TextBar1) / _GetHeightScale(p1TextBar1);

		_SetStickyIO(p1TextBar1, true);
		
		_Move(
			p1TextBar1, 
			10.0f, 
			10.0f);

		_ReScale(
			p1TextBar1,
			(tb1Width - abs(p1Width - tb1Width) - 20.0f) / (p1Width + abs(p1Width - tb1Width)),
			0.5f);
		
		tb1Width = _GetWidth(p1TextBar1);
		tb1Height = _GetHeight(p1TextBar1);

		///

		float textScale = 0.5f;

		p1ListViewer1 = _CreateListViewer(2.0f, 2.0f, true, textScale);
		
		_AttachObject1(panel1, p1ListViewer1);
		
		float lv1Width = _GetWidth(p1ListViewer1) / _GetWidthScale(p1ListViewer1);
		float lv1Height = _GetHeight(p1ListViewer1) / _GetHeightScale(p1ListViewer1);

		_SetAcceptAction(p1ListViewer1, false);
		
		_SetBorderWidth(p1ListViewer1, 6.0f);
		_SetBorderHeight(p1ListViewer1, 6.0f);
		
		_Move(
			p1ListViewer1, 
			10.0f, 
			_GetHeight(p1TextBar1) + 15.0f);
		
		_ReScaleByLines(
			p1ListViewer1,
			(lv1Width - abs(p1Width - lv1Width) - 20.0f) / (p1Width + abs(p1Width - lv1Width)),
			17);
		
		lv1Width = _GetWidth(p1ListViewer1);
		lv1Height = _GetHeight(p1ListViewer1);

		_LoadString(p1ListViewer1, "Actual Surf ::");		// 0
		_LoadString(p1ListViewer1, "HP ::");				// 1
		_LoadString(p1ListViewer1, "MP ::");				// 2
		_LoadString(p1ListViewer1, "XP ::");				// 3
		_LoadString(p1ListViewer1, "LOC ::");				// 4
		_LoadString(p1ListViewer1, "");					// 5
		_LoadString(p1ListViewer1, "Previous ::");			// 6
		_LoadString(p1ListViewer1, "Next ::");				// 7
		_LoadString(p1ListViewer1, "Comment ::");			// 8
		_LoadString(p1ListViewer1, "Command ::");			// 9
		_LoadString(p1ListViewer1, "Expected Surf ::");	// 10
		_LoadString(p1ListViewer1, "Expected LOC ::");		// 11
		_LoadString(p1ListViewer1, "");					// 12
		_LoadString(p1ListViewer1, "% Per Hour");			// 13
		_LoadString(p1ListViewer1, "Hours till 100%");		// 14
		_LoadString(p1ListViewer1, "% in %f hours");		// 15
		_LoadString(p1ListViewer1, "Running for the last %f hours");	// 16

		///

		p1ListViewer2 = _CreateListViewer(2.0f, 2.0f, true, textScale);
		
		_AttachObject1(panel1, p1ListViewer2);
		
		float lv2Width = _GetWidth(p1ListViewer2) / _GetWidthScale(p1ListViewer2);
		float lv2Height = _GetHeight(p1ListViewer2) / _GetHeightScale(p1ListViewer2);

		_SetAcceptAction(p1ListViewer2, false);
		
		_SetBorderWidth(p1ListViewer2, 6.0f);
		_SetBorderHeight(p1ListViewer2, 6.0f);
		
		_Move(
			p1ListViewer2, 
			10.0f, 
			_GetHeight(p1TextBar1) + _GetHeight(p1ListViewer1) + 20.0f);
		
		_ReScaleByLines(
			p1ListViewer2,
			(lv2Width - abs(p1Width - lv2Width) - 20.0f) / (p1Width + abs(p1Width - lv2Width)),
			5);
		
		lv2Width = _GetWidth(p1ListViewer2);
		lv2Height = _GetHeight(p1ListViewer2);

		///

		float innerHeight = tb1Height + 5.0f + lv1Height + 5.0f + lv2Height;
		
		p1Width = _GetWidth(panel1) / _GetWidthScale(panel1);
		p1Height = _GetHeight(panel1) / _GetHeightScale(panel1);
		
		_ReScale(
			panel1,
			0.85f,
			(p1Height - abs(innerHeight - p1Height) + 20.0f) / (innerHeight + abs(innerHeight - p1Height)));

		////////////////////////////////////////////////////////////////////////////

		// open and read the script file
		ReadScriptFile();

		// reset the warning log
		ClearWarnLog();

		initialized = true;

		// run the script
		_beginthreadex(NULL, 0, RunThreadA, NULL, 0, &threadA);

		// do a continuous memory check
		_beginthreadex(NULL, 0, RunThreadB, NULL, 0, &threadB);
	}

	return initialized;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::Render(void) 
{
	if (logout)
	{
		_ActivateFull2(extComputron, _CheckExtMenuIO(extComputron));
		return;
	}

	// take a screenshot
	if (g_Screenshot && g_Screenshot->initialized && captureScreen)
		g_Screenshot->CaptureScreen();

	_ActivateFull2(extComputron, _CheckExtMenuIO(extComputron));

	float p1Height = _GetHeight(panel1) / _GetHeightScale(panel1);

	if (_CheckExtMenuIO(extComputron) && _GetIO(p1TextBar1))
	{
		_ActivateFull1(p1ListViewer1, false);
		_ActivateFull1(p1ListViewer2, false);

		_ReScale(
			panel1,
			_GetWidthScale(panel1),
			(_GetHeight(p1TextBar1) + 20.0f) / p1Height);
	}
	else if (_CheckExtMenuIO(extComputron))
	{
		_ReScale(
			panel1,
			_GetWidthScale(panel1),
			(_GetHeight(p1TextBar1) + _GetHeight(p1ListViewer1) + _GetHeight(p1ListViewer2) + 30.0f) / p1Height);

		char buffer[256];

		sprintf_s(buffer, sizeof(buffer), "Actual Surf :: %i", surfSaved);
		_SetStringAt(p1ListViewer1, 0, buffer);

		sprintf_s(buffer, sizeof(buffer), "HP :: %f %%", progressHP);
		_SetStringAt(p1ListViewer1, 1, buffer);

		sprintf_s(buffer, sizeof(buffer), "MP :: %f %%", progressMP);
		_SetStringAt(p1ListViewer1, 2, buffer);

		sprintf_s(buffer, sizeof(buffer), "XP :: %f %%", progressXP);
		_SetStringAt(p1ListViewer1, 3, buffer);

		sprintf_s(buffer, sizeof(buffer), "LOC :: %i , %i", locationX, locationY);
		_SetStringAt(p1ListViewer1, 4, buffer);

		///

		if (instrucPrevious)
			sprintf_s(buffer, sizeof(buffer), "Previous :: %i of %i", instrucPrevious->GetInstrucPosition() + 1, countTotal);
		else
			sprintf_s(buffer, sizeof(buffer), "Previous :: ? of %i", countTotal);

		_SetStringAt(p1ListViewer1, 6, buffer);
		
		///

		if (!transform && ((int)((float)timeGetTime() / 100.0f) % 3) != 0)
		{
			if (instrucCurrent)
				_SetStringAt(p1ListViewer1, 7, "Next :: stopped");
			else
				_SetStringAt(p1ListViewer1, 7, "Next :: zero instructions");
		}
		else
		{
			_SetStringAt(p1ListViewer1, 7, "-");
		}

		///

		if (instrucCurrent)
		{
			if (transform)
			{
				if (!okayHP && !okayMP)
				{
					sprintf_s(buffer, sizeof(buffer), "Next :: %i%% HP, %i%% MP", 100 - (int)progressHP, 100 - (int)progressMP);
				}
				else if (okayHP == false)
				{
					sprintf_s(buffer, sizeof(buffer), "Next :: %i%% HP", 100 - (int)progressHP);
				}
				else if (okayMP == false)
				{
					sprintf_s(buffer, sizeof(buffer), "Next :: %i%% MP", 100 - (int)progressMP);
				}
				else if (instrucPrevious && instrucPrevious->GetInstrucPauseA() != instrucPrevious->GetInstrucPauseB())
				{
					sprintf_s(buffer, sizeof(buffer), "Next :: (r)%f", pauseLength - (((float)timeGetTime() / 1000.0f) - pauseStart));
				}
				else
				{
					sprintf_s(buffer, sizeof(buffer), "Next :: %f", pauseLength - (((float)timeGetTime() / 1000.0f) - pauseStart));
				}

				_SetStringAt(p1ListViewer1, 7, buffer);
			}

			//
			
			sprintf_s(buffer, sizeof(buffer), "Comment :: %s", instrucCurrent->GetInstrucComment());
			_SetStringAt(p1ListViewer1, 8, buffer);
			
			///

			if (instrucCurrent->GetInstrucUseKeystroke())
			{
				sprintf_s(buffer, sizeof(buffer), "Keystroke :: %s", _convertDIK_Key(instrucCurrent->GetInstrucKeystrokeCode()));
			}
			else if (instrucCurrent->GetInstrucUseMouseClick())
			{
				sprintf_s(buffer, sizeof(buffer), "Mouse Click :: (%i , %i)",
					instrucCurrent->GetInstrucMouseClickX(),
					instrucCurrent->GetInstrucMouseClickY());
			}
			else if (instrucCurrent->GetInstrucSendPacket())
			{
				sprintf_s(buffer, sizeof(buffer), "Send Packet :: %s.cap", instrucCurrent->GetInstrucPacketFile());
			}
			else
				sprintf_s(buffer, sizeof(buffer), "Command :: Pause Only");
			
			_SetStringAt(p1ListViewer1, 9, buffer);

			///

			if (instrucCurrent->GetSafetyUseSurfCheck())
			{
				sprintf_s(buffer, sizeof(buffer), "Expected Surf :: %i - %i",
					instrucCurrent->GetSafetySurfMin(), instrucCurrent->GetSafetySurfMax());
			}
			else
				sprintf_s(buffer, sizeof(buffer), "Expected Surf :: none");
			
			_SetStringAt(p1ListViewer1, 10, buffer);

			///

			if (instrucCurrent->GetSafetyUseCoordCheck())
			{
				sprintf_s(buffer, sizeof(buffer), "Expected LOC :: %i , %i",
					instrucCurrent->GetSafetyCoordX(), instrucCurrent->GetSafetyCoordY());
			}
			else
				sprintf_s(buffer, sizeof(buffer), "Expected LOC :: none");

			_SetStringAt(p1ListViewer1, 11, buffer);

			///

			float running_for = ((((float)timeGetTime() / 1000.0f) - startPerc) / 3600.0f);

			if (transform && lastXP != initPerc  && percPerHr != 0.0f)
				sprintf_s(buffer, sizeof(buffer), "%% Per Hour = %f", percPerHr);
			else if (transform == false)
				sprintf_s(buffer, sizeof(buffer), "%% Per Hour = ?");
			else
				sprintf_s(buffer, sizeof(buffer), "[will start after next %% hits]");
			
			_SetStringAt(p1ListViewer1, 13, buffer);

			///

			if (transform && lastXP != initPerc && hrTillHundert != 0.0f)
				sprintf_s(buffer, sizeof(buffer), "Hours till 100%% = %f", (hrTillHundert - (running_for - endPerc)));
			else if (transform == false)
				sprintf_s(buffer, sizeof(buffer), "Hours till 100%% = ?");
			else
				sprintf_s(buffer, sizeof(buffer), "[will start after next %% hits]");

			_SetStringAt(p1ListViewer1, 14, buffer);

			///

			if (transform && lastXP != initPerc)
				sprintf_s(buffer, sizeof(buffer), "%f%% in %f hours", totalPerc, endPerc);
			else if (transform == false)
				sprintf_s(buffer, sizeof(buffer), "?%% in ? hours");
			else
				sprintf_s(buffer, sizeof(buffer), "[will start after next %% hits]");

			_SetStringAt(p1ListViewer1, 15, buffer);

			///

			if (transform && lastXP != initPerc)
				sprintf_s(buffer, sizeof(buffer), "Running for the last %f hours", running_for);
			else if (transform == false)
				sprintf_s(buffer, sizeof(buffer), "Running for the last ? hours");
			else
				sprintf_s(buffer, sizeof(buffer), "[will start after next %% hits]");
			
			_SetStringAt(p1ListViewer1, 16, buffer);
		}
		else
		{
			_SetStringAt(p1ListViewer1, 8, "-");
			_SetStringAt(p1ListViewer1, 9, "-");
			_SetStringAt(p1ListViewer1, 10, "-");
			_SetStringAt(p1ListViewer1, 11, "-");

			_SetStringAt(p1ListViewer1, 13, "-");
			_SetStringAt(p1ListViewer1, 14, "-");
			_SetStringAt(p1ListViewer1, 15, "-");
			_SetStringAt(p1ListViewer1, 16, "-");
		}

		///

		float textScale = 0.5f;
		float fontWidth = textScale * _FontWidth();
		float fontHeight = textScale * _FontHeight();

		_DrawText(
			scriptFilename,
			_ScreenWidth() - (fontWidth * strlen(scriptFilename)),
			fontHeight * 0.0f,
			textScale,
			COLOR_WHITE);

		sprintf_s(buffer, sizeof(buffer), "script has run %i times full through", ranTimes);
		_DrawText(
			buffer,
			_ScreenWidth() - (fontWidth * strlen(buffer)),
			fontHeight * 1.0f,
			textScale,
			COLOR_WHITE);

		///

		if (jreturnStack)
		{
			sprintf_s(buffer, sizeof(buffer), "Jump and Link Stack ");
			_DrawText(
				buffer,
				_ScreenWidth() - (fontWidth * strlen(buffer)),
				250.0f + (fontHeight * 0.0f),
				textScale,
				COLOR_WHITE);

			sprintf_s(buffer, sizeof(buffer), "(Top 10 Instructions)");
			_DrawText(
				buffer,
				_ScreenWidth() - (fontWidth * strlen(buffer)),
				250.0f + (fontHeight * 1.0f),
				textScale,
				COLOR_WHITE);

			sprintf_s(buffer, sizeof(buffer), "---------------------");
			_DrawText(
				buffer,
				_ScreenWidth() - (fontWidth * strlen(buffer)),
				250.0f + (fontHeight * 2.0f),
				textScale,
				COLOR_WHITE);
				
			JReturnStack* temp = jreturnStack;

			for (int i = 0; temp != NULL && i < 10; i++)
			{
				if ((temp->GetJReturn())->GetWarningsJAL() == false)
					sprintf_s(buffer, sizeof(buffer), "no return");
				else
					sprintf_s(buffer, sizeof(buffer), "return");

				if (i == 0)
					sprintf_s(buffer, sizeof(buffer), "%s (%s, in %i)", (temp->GetJReturn())->GetInstrucComment(), buffer, (temp->GetJReturnFrom() - instrucCurrent->GetInstrucPosition()));
				else
					sprintf_s(buffer, sizeof(buffer), "%s (%s, in x)", (temp->GetJReturn())->GetInstrucComment(), buffer);

				_DrawText(
					buffer,
					_ScreenWidth() - (fontWidth * strlen(buffer)),
					250.0f + (fontHeight * (3.0f + i)),
					textScale,
					COLOR_WHITE);

				temp = temp->GetNext();
			}
		}

		DrawCursor();
	}

	captureScreen = false;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::DrawCursor()
{
	char cursorXText[256];
	char cursorYText[256];
	
	sprintf_s(cursorXText, sizeof(cursorXText), "x:  %i", (int)_Mouse().CursorX);
	sprintf_s(cursorYText, sizeof(cursorYText), "y:  %i", (int)_Mouse().CursorY);
	
	float cursorX = _X2W(_Mouse().CursorX);
	float cursorY = _Y2H(_Mouse().CursorY);

	float x = 0.0f;
	float xy = 0.0f;
	float yy = 0.0f;

	float textScale = 0.5f;
	float fontWidth = textScale * _FontWidth();
	float fontHeight = textScale * _FontHeight();
	
	if (cursorY + 85.0f >= _ScreenHeight())
	{
		xy = _ScreenHeight() - (fontHeight * 2.0f);
		yy = _ScreenHeight() - (fontHeight * 1.0f);
	}
	else
	{
		xy = cursorY + 85.0f - (fontHeight * 2.0f);
		yy = cursorY + 85.0f - (fontHeight * 1.0f);
	}
	
	int longer = strlen(cursorYText);
	if (strlen(cursorXText) > strlen(cursorYText))
	{
		longer = strlen(cursorXText);
	}

	if (cursorX + (fontWidth * longer) >= _ScreenWidth())
	{
		x = _ScreenWidth() - (fontWidth * longer);
	}
	else
	{
		x = cursorX;
	}
	
	_DrawText(cursorXText, x, xy, textScale, COLOR_WHITE);
	_DrawText(cursorYText, x, yy, textScale, COLOR_WHITE);
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
unsigned __stdcall RunThreadA(void* params)
{
	while (g_Computron && g_Computron->initialized)
	{
		g_Computron->ScriptCycleProc();
		Sleep(1);
	}

	return 0;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
unsigned __stdcall RunThreadB(void* params)
{
	while (g_Computron && g_Computron->initialized)
	{
		g_Computron->MemoryCheckProc();
		Sleep(1);
	}

	return 0;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
string Computron::ReceiveData(char* threadcBuff)
{
	string receivedData = "Failed to connect!";

	HINTERNET g_hInetRequest;

	if (g_hInetConnection)
	{
		g_hInetRequest = HttpOpenRequest(
			g_hInetConnection,
			"GET",
			threadcBuff,
			HTTP_VERSION,
			NULL,
			NULL,
			INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE,
			0);

		if (g_hInetRequest && HttpSendRequest(g_hInetRequest,NULL, 0, NULL, 0) == TRUE)
		{
			char buffer[256];
			DWORD byteCount;
			BOOL received = InternetReadFile(g_hInetRequest, buffer, 256, &byteCount);
				
			if (received)
				receivedData = "";
	
			while (received && (byteCount > 0))
			{
				receivedData.append(buffer, byteCount);
				received = InternetReadFile(g_hInetRequest, buffer, 256, &byteCount);
			}

			InternetCloseHandle(g_hInetRequest);
			g_hInetRequest = NULL;
		}
	}

	return receivedData;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::AddWarnLog(int _position, char* _log)
{
	FILE* out = NULL;
	fopen_s(&out, "XenDLL Files/Extensions/Computron/warnLog.txt", "a");

	if (out)
	{		
		time_t rawTime;
		tm timeInfo;

		time(&rawTime);
		localtime_s(&timeInfo, &rawTime);

		fprintf(out, "%i/%i/%i %i:%i:%i - %i -> %s\n\n",
			_position,
			timeInfo.tm_year + 1900,
			timeInfo.tm_mon,
			timeInfo.tm_mday,
			timeInfo.tm_hour,
			timeInfo.tm_min,
			timeInfo.tm_sec,
			_log);

		fclose(out);
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ClearWarnLog(void)
{
	// clear the list of warnings
	_Empty(p1ListViewer2);
	_LoadString(p1ListViewer2, "Warnings ::");
	
	// reset warning log
	FILE* out = NULL;
	fopen_s(&out, "XenDLL Files/Extensions/Computron/warnLog.txt", "w");
	if (out) fclose(out);
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ClearInstructions(void)
{
	ClearJReturnStack();

	Instruction* prev = NULL;
	Instruction* curr = instrucHeader;

	while (curr)
	{
		prev = curr;
		curr = curr->GetNext();

		delete prev;
	}

	instrucHeader = NULL;
	instrucCurrent = NULL;
	instrucPrevious = NULL;

	countTotal = 0;
	countSpecial = 0;
	countNormal = 0;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ClearJReturnStack(void)
{
	JReturnStack* temp = NULL;
	while (temp = jreturnStack)
	{
		jreturnStack = jreturnStack->GetNext();
		delete temp;
	}

	jreturnStack = NULL;
	jjump = false;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ResetTransform(void)
{	
	instrucPrevious = NULL;
	instrucCurrent = instrucHeader;

	pauseStart = 0.0f;
	pauseLength = 0.0f;

	ClearJReturnStack();
	ClearWarnLog();

	minuteJumpTriggered = false;
	NextNormalInstruction();

	totalPerc = 0.0f;
	
	try { lastXP = (*g_progressXP / 63.0f) * 100.0f; }
	catch (...) { }

	hrTillHundert = 0.0f;
	percPerHr = 0.0f;
	initPerc = lastXP;
	startPerc = (float)timeGetTime() / 1000.0f;
	endPerc = 0.0f;

	ranTimes = 0;

}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ContinCheckProc(void)
{
	char buffer[256];

	if ((!okayHP || !okayMP || (((float)timeGetTime() / 1000.0f) - pauseStart < pauseLength)) &&
		!checkconUsed &&
		instrucPrevious->GetSafetyUseCoordCheckcon() &&
		instrucPrevious->GetSafetyUseCoordCheck() &&
		(instrucPrevious->GetSafetyCoordX() != locationX || instrucPrevious->GetSafetyCoordY() != locationY))
	{
		checkconUsed = true;

		sprintf_s(buffer, sizeof(buffer), "Wrong Coordinates [continuous check] (actual = %i , %i | Allowed = %i , %i)",
			locationX,
			locationY,
			instrucPrevious->GetSafetyCoordX(),
			instrucPrevious->GetSafetyCoordY());

		AddWarnLog(instrucPrevious->GetInstrucPosition(), buffer);

		UseWarnings("WRONG COORDINATES (cont)");
	}
	else if ((!okayHP || !okayMP || (((float)timeGetTime() / 1000.0f) - pauseStart < pauseLength)) &&
		!checkconUsed &&
		instrucPrevious->GetSafetyUseSurfCheckcon() &&
		instrucPrevious->GetSafetyUseSurfCheck())
	{
		if (surfSaved < instrucPrevious->GetSafetySurfMin())
		{
			checkconUsed = true;

			sprintf_s(buffer, sizeof(buffer), "Low Surf Count [continuous check] (actual = %i | Allowed = %i - %i)",
				surfSaved,
				instrucPrevious->GetSafetySurfMin(),
				instrucPrevious->GetSafetySurfMax());

			AddWarnLog(instrucPrevious->GetInstrucPosition(), buffer);

			UseWarnings("LOW SURF COUNT (cont)");
		}
		else if (surfSaved > instrucPrevious->GetSafetySurfMax())
		{
			checkconUsed = true;

			sprintf_s(buffer, sizeof(buffer), "High Surf Count [continuous check] (actual = %i | Allowed = %i - %i)",
				surfSaved,
				instrucPrevious->GetSafetySurfMin(),
				instrucPrevious->GetSafetySurfMax());

			AddWarnLog(instrucPrevious->GetInstrucPosition(), buffer);

			UseWarnings("HIGH SURF COUNT (cont)");
		}
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ScriptCycleProc(void)
{
	// do nothing if taking a screenshot
	if (captureScreen) return;

	char buffer[256];

	if (transform)
	{
		if (progressHP <= 0.0f)
		{
			transform = false;

			AddWarnLog(instrucCurrent->GetInstrucPosition(), "You Died");

			UseWarnings("YOU DIED", true);

			captureScreen = true;

			return;
		}

		if (instrucPrevious)
		{
			ContinCheckProc();
		}

		if (instrucCurrent)
		{
			if (okayHP && okayMP && ((float)timeGetTime() / 1000.0f) - pauseStart >= pauseLength)
			{
				if (checkconUsed == false)
				{
					checkconUsed = true;

					if (instrucCurrent->GetSafetyUseCoordCheck() && (
						instrucCurrent->GetSafetyCoordX() != locationX ||
						instrucCurrent->GetSafetyCoordY() != locationY))
					{
						// -1 is used to force a warning
						if (instrucCurrent->GetSafetyCoordX() != -1 && instrucCurrent->GetSafetyCoordY() != -1)
						{
							sprintf_s(buffer, sizeof(buffer), "Wrong Coordinates [initial check] (actual = %i , %i | Allowed = %i , %i)",
								locationX,
								locationY,
								instrucCurrent->GetSafetyCoordX(),
								instrucCurrent->GetSafetyCoordY());

							AddWarnLog(instrucCurrent->GetInstrucPosition(), buffer);
						}

						UseWarnings("WRONG COORDINATES");
					}
					else if (instrucCurrent->GetSafetyUseSurfCheck() &&
						surfSaved < instrucCurrent->GetSafetySurfMin())
					{
						// -1 is used to force a warning
						if (instrucCurrent->GetSafetySurfMin() != -1)
						{
							sprintf_s(buffer, sizeof(buffer), "Low Surf Count [initial check] (actual = %i | Allowed = %i - %i)",
								surfSaved,
								instrucCurrent->GetSafetySurfMin(),
								instrucCurrent->GetSafetySurfMax());

							AddWarnLog(instrucCurrent->GetInstrucPosition(), buffer);
						}

						UseWarnings("LOW SURF COUNT");
					}
					else if (instrucCurrent->GetSafetyUseSurfCheck() &&
						surfSaved > instrucCurrent->GetSafetySurfMax())
					{
						// -1 is used to force a warning
						if (instrucCurrent->GetSafetySurfMax() != -1)
						{
							sprintf_s(buffer, sizeof(buffer), "High Surf Count [initial check] (actual = %i | Allowed = %i - %i)",
								surfSaved,
								instrucCurrent->GetSafetySurfMin(),
								instrucCurrent->GetSafetySurfMax());

							AddWarnLog(instrucCurrent->GetInstrucPosition(), buffer);
						}

						UseWarnings("HIGH SURF COUNT");
					}
				}
				else
				{
					if (instrucCurrent->GetInstrucClearReturnStack())
					{
						ClearJReturnStack();
					}
					else if (instrucCurrent->GetInstrucUseKeystroke())
					{
						g_CustomDevice->directInputHook->SetDIKeyState(instrucCurrent->GetInstrucKeystrokeCode(), 0x80);
					}
					else if (instrucCurrent->GetInstrucUseMouseClick())
					{
						g_CustomDevice->directInputHook->SetMouseClick(instrucCurrent->GetInstrucMouseClickX(),
							instrucCurrent->GetInstrucMouseClickY(), MOUSEBUTTON_LEFT);
					}
					else if (instrucCurrent->GetInstrucSendPacket())
					{
						sprintf_s(buffer, sizeof(buffer), "%s.cap",
							instrucCurrent->GetInstrucPacketFile());

						SendCap(buffer, networkAdapter, "s");
					}
					
					GetPause();
					NextNormalInstruction();
				}
			}
		}
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::MemoryCheckProc(void)
{
	try { progressHP = (*g_progressHP / 63.0f) * 100.0f; }
	catch (...) { }

	if (useHealthKey && progressHP < healthPercentTrigger && progressHP > 0.0f)
		g_CustomDevice->directInputHook->SetDIKeyState(healthMonitorKey, 0x80);

	if (useManaKey && progressMP < manaPercentTrigger && progressMP > 0.0f)
		g_CustomDevice->directInputHook->SetDIKeyState(manaMonitorKey, 0x80);

	if (zeroPause || (instrucPrevious && instrucPrevious->GetSafetyUseHPWait() == false) ||
	   (instrucPrevious && instrucPrevious->GetSafetyUseHPWait() && progressHP >= 100.0f))
		okayHP = true;
	else
		okayHP = false;

	if (zeroPause || (instrucPrevious && instrucPrevious->GetSafetyUseMPWait() == false) ||
	   (instrucPrevious && instrucPrevious->GetSafetyUseMPWait() && progressMP >= 100.0f))
		okayMP = true;
	else
		okayMP = false;

	try { progressMP = (*g_progressMP / 63.0f) * 100.0f; }
	catch (...) { }
	
	try { progressXP = (*g_progressXP / 63.0f) * 100.0f; }
	catch (...) { }

	if (transform)
	{
		if (progressXP != lastXP)
		{
			if (lastXP != initPerc)
			{
				totalPerc += progressXP - lastXP;

				percPerHr = ((100.0f * totalPerc) / ((((float)timeGetTime() / 1000.0f) - (float)startPerc) / 3600.0f));
				hrTillHundert = (100.0f - progressXP) / percPerHr;

				endPerc = (((float)timeGetTime() / 1000.0f) - startPerc) / 3600.0f;
			}
			else
			{
				startPerc = (float)timeGetTime() / 1000.0f;
			}

			lastXP = progressXP;
		}
	}

	try { locationX = (int)*g_locationX; }
	catch (...) { }
	
	try { locationY = (int)*g_locationY; }
	catch (...) { }
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::NextNormalInstruction(void) 
{
	if (instrucCurrent)
	{
		instrucPrevious = instrucCurrent;

		while (jjump && instrucCurrent->GetInstrucPosition() == jreturnStack->GetJReturnFrom())
		{
			if ((jreturnStack->GetJReturn())->GetWarningsJAL())
			{
				instrucCurrent = jreturnStack->GetJReturn();
			}

			JReturnStack* temp = jreturnStack;
			jreturnStack = jreturnStack->GetNext();
			delete temp;

			if (jreturnStack == NULL)
				jjump = false;
		}

		int findStart = instrucCurrent->GetInstrucPosition();
		
		instrucCurrent = instrucCurrent->GetNext();
		
		if (instrucCurrent == NULL)
		{
			instrucCurrent = instrucHeader->GetNext();
			ranTimes++;
		}
		
		bool isSpecial = instrucCurrent->GetInstrucIsSpecial();

		bool timeCheck = instrucCurrent->GetInstrucUseTimedUse();
		bool okayTimedUse = true;

		if (timeCheck)
			okayTimedUse = (((float)timeGetTime() / 1000.0f) - instrucCurrent->GetTimedUseStart()) >= instrucCurrent->GetTimedUseLength();

		if (okayTimedUse && instrucCurrent->GetInstrucNotFirstTimedUse() && instrucCurrent->GetTimedUseStart() == 0.0)
		{
			if (instrucCurrent->GetInstrucTimedUseA() != instrucCurrent->GetInstrucTimedUseB())
			{
				srand((int)timeGetTime());
				instrucCurrent->SetTimedUseLength((rand() % (int)(instrucCurrent->GetInstrucTimedUseB() + 1 - instrucCurrent->GetInstrucTimedUseA())) + instrucCurrent->GetInstrucTimedUseA());
			}
			else
				instrucCurrent->SetTimedUseLength(instrucCurrent->GetInstrucTimedUseA());

				instrucCurrent->SetTimedUseStart((float)timeGetTime() / 1000.0f);

			okayTimedUse = false;
		}

		// use to ignore specials
		while (transform && ((timeCheck && !okayTimedUse) || (!jjump && isSpecial && instrucCurrent->GetInstrucPosition() != findStart)))
		{
			instrucCurrent = instrucCurrent->GetNext();
			if (instrucCurrent == NULL)
			{
				instrucCurrent = instrucHeader->GetNext();
			}

			isSpecial = instrucCurrent->GetInstrucIsSpecial();

			timeCheck = instrucCurrent->GetInstrucUseTimedUse();

			if (timeCheck)
				okayTimedUse = (((float)timeGetTime() / 1000.0f) - instrucCurrent->GetTimedUseStart()) >= instrucCurrent->GetTimedUseLength();

			if (okayTimedUse && instrucCurrent->GetInstrucNotFirstTimedUse() && instrucCurrent->GetTimedUseStart() == 0.0)
			{
				if (instrucCurrent->GetInstrucTimedUseA() != instrucCurrent->GetInstrucTimedUseB())
				{
					srand((int)timeGetTime());
					instrucCurrent->SetTimedUseLength((rand() % (int)(instrucCurrent->GetInstrucTimedUseB() + 1 - instrucCurrent->GetInstrucTimedUseA())) + instrucCurrent->GetInstrucTimedUseA());
				}
				else
					instrucCurrent->SetTimedUseLength(instrucCurrent->GetInstrucTimedUseA());

				instrucCurrent->SetTimedUseStart((float)timeGetTime() / 1000.0f);

				okayTimedUse = false;
			}
		}

		if (timeCheck)
		{
			if (instrucCurrent->GetInstrucTimedUseA() != instrucCurrent->GetInstrucTimedUseB())
			{
				srand((int)timeGetTime());
				instrucCurrent->SetTimedUseLength((rand() % (int)(instrucCurrent->GetInstrucTimedUseB() + 1 - instrucCurrent->GetInstrucTimedUseA())) + instrucCurrent->GetInstrucTimedUseA());
			}
			else
				instrucCurrent->SetTimedUseLength(instrucCurrent->GetInstrucTimedUseA());

			instrucCurrent->SetTimedUseStart((float)timeGetTime() / 1000.0f);
		}

		checkconUsed = false;
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::GetPause(int i) 
{
	if (instrucCurrent)
	{
		if (i == 0)
		{
			pauseLength = 0.0f;
			zeroPause = true;
		}
		else
		{
			if (instrucCurrent->GetInstrucPauseA() != instrucCurrent->GetInstrucPauseB())
			{
				srand((int)timeGetTime());
				pauseLength = (rand() % (int)(instrucCurrent->GetInstrucPauseB() + 1 - instrucCurrent->GetInstrucPauseA())) + instrucCurrent->GetInstrucPauseA();
			}
			else
				pauseLength = instrucCurrent->GetInstrucPauseA();

			zeroPause = false;
		}
		
		pauseStart = (float)timeGetTime() / 1000.0f;
	}
	else
	{
		pauseLength = 0.0f;
		zeroPause = true;
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::UseWarnings(char* str, bool justText, int pos) 
{
	char buffer[256];

	bool ignoreDocumenting = false;

	Instruction* toUse = NULL;

	if (_stricmp(str, "LOW SURF COUNT (cont)") == 0 ||
		_stricmp(str, "HIGH SURF COUNT (cont)") == 0 ||
		_stricmp(str, "WRONG COORDINATES (cont)") == 0)
		toUse = instrucPrevious;
	else
		toUse = instrucCurrent;

	if ((_stricmp(str, "LOW SURF COUNT") == 0 && toUse->GetSafetySurfMin() == -1) ||
		(_stricmp(str, "HIGH SURF COUNT") == 0 && toUse->GetSafetySurfMax() == -1) ||
		(_stricmp(str, "WRONG COORDINATES") == 0 && (toUse->GetSafetyCoordX() == -1 || toUse->GetSafetyCoordY() == -1)))
	{
		ignoreDocumenting = true;
	}

	if (ignoreDocumenting == false)
	{
		if (pos >= 0)
			sprintf_s(buffer, sizeof(buffer), "(%i) %s", toUse->GetInstrucPosition(), str);
		else
			sprintf_s(buffer, sizeof(buffer), "%s", str);
		
		// add warning to the warning panel
		_LoadString(p1ListViewer2, buffer);
	}

	if (toUse && justText == false)
	{
		if (toUse->GetWarningsTakeScreenshot())
		{
			captureScreen = true;

			if (ignoreDocumenting == false)
			{
				//UseWarnings("SCREEN CAPTURED", true);

				AddWarnLog(toUse->GetInstrucPosition(), "Screen Captured");
			}
		}

		if (toUse->GetWarningsPlayAudio())
		{
			sprintf_s(buffer, sizeof(buffer), "%s.wav", toUse->GetWarningsAudioFile());
			if (GetFileAttributes(buffer) != 0xFFFFFFFF)
			{
				PlaySound(buffer, NULL, SND_FILENAME | SND_ASYNC);

				if (ignoreDocumenting == false)
				{
					//UseWarnings("AUDIO PLAYED", true);

					sprintf_s(buffer, sizeof(buffer), "Audio Played (%s.wav)",
						toUse->GetWarningsAudioFile());

					AddWarnLog(toUse->GetInstrucPosition(), buffer);
				}
			}
		}

		if (toUse->GetWarningsJump())
		{
			Instruction* jumpTo = instrucHeader->GetNext();
			
			while (jumpTo && jumpTo->GetInstrucPosition() != toUse->GetWarningsJumpA())
				jumpTo = jumpTo->GetNext();

			JReturnStack* jreturnNew = new JReturnStack(jreturnStack, toUse->GetWarningsJumpB(), instrucCurrent);
			jreturnStack = jreturnNew;
			
			instrucPrevious = NULL;
			checkconUsed = false;

			instrucCurrent = jumpTo;
			jjump = true;

			GetPause(0);

			if (ignoreDocumenting == false)
			{
				sprintf_s(buffer, sizeof(buffer), "Jumped to %i",
					toUse->GetWarningsJumpA());

				AddWarnLog(toUse->GetInstrucPosition(), buffer);
			}
		}
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::ReadScriptFile()
{
	ClearInstructions();

	char buffer[MAX_PATH];
	sprintf_s(buffer, sizeof(buffer), "XenDLL Files/Extensions/Computron/Scripts/%s.xml", scriptFilename);

	if (GetFileAttributes(buffer) == 0xFFFFFFFF) return;

	TiXmlDocument document;

	if (document.LoadFile(buffer) == false) return;

	TiXmlElement* root = NULL;
	int version;

	if (!(root = document.FirstChildElement("script")) ||
		!root->Attribute("version", &version) ||
		version != SCRIPT_VERSION) return;

	instrucHeader = new Instruction();
	instrucHeader->SetInstrucPosition(-1);

	Instruction* prev = instrucHeader;
	Instruction* curr = NULL;

	countTotal = 0;
	countSpecial = 0;
	countNormal = 0;

	// get the script settings
	TiXmlElement* element = root->FirstChildElement("setting");

	while (element)
	{
		TiXmlElement* item = NULL;

		item = element->FirstChildElement("scriptScreenSizeA");
		if (item) screenSizeA = (float)atof(item->GetText());

		item = element->FirstChildElement("scriptScreenSizeB");
		if (item) screenSizeB = (float)atof(item->GetText());
		
		item = element->FirstChildElement("scriptUseHealthKey");
		if (item) useHealthKey = (_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("scriptHealthPercent");
		if (item) healthPercentTrigger = atoi(item->GetText());

		item = element->FirstChildElement("scriptHealthCode");
		if (item) healthMonitorKey = atoi(item->GetText());
		
		item = element->FirstChildElement("scriptUseManaKey");
		if (item) useManaKey = (_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("scriptManaPercent");
		if (item) manaPercentTrigger = atoi(item->GetText());

		item = element->FirstChildElement("scriptManaCode");
		if (item) manaMonitorKey = atoi(item->GetText());

		element = element->NextSiblingElement();
	}

	// get the script instructions
	element = root->FirstChildElement("instruction");

	while (element)
	{
		TiXmlElement* item = NULL;
		
		curr = new Instruction();
		prev->SetNext(curr);
		prev = curr;
		
		curr->SetInstrucPosition(countTotal);
		countTotal++;

		// instruction
		item = element->FirstChildElement("instrucIsSpecial");
		if (item)
		{
			curr->SetInstrucIsSpecial(_stricmp(item->GetText(), "true") == 0);

			if (curr->GetInstrucIsSpecial())
			{
				countSpecial++;
			}
			else
			{
				countNormal++;
			}
		}

		item = element->FirstChildElement("instrucClearReturnStack");
		if (item) curr->SetInstrucClearReturnStack(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("instrucUseKeystroke");
		if (item) curr->SetInstrucUseKeystroke(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("instrucKeystrokeCode");
		if (item) curr->SetInstrucKeystrokeCode(_convertDIK_Name(item->GetText()));

		item = element->FirstChildElement("instrucUseMouseClick");
		if (item) curr->SetInstrucUseMouseClick(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("instrucMouseClickX");
		if (item) curr->SetInstrucMouseClickX((int)(atoi(item->GetText()) * (_ScreenWidth() / screenSizeA)));

		item = element->FirstChildElement("instrucMouseClickY");
		if (item) curr->SetInstrucMouseClickY((int)(atoi(item->GetText()) * (_ScreenHeight() / screenSizeB)));

		item = element->FirstChildElement("instrucSendPacket");
		if (item) curr->SetInstrucSendPacket(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("instrucPacketFile");
		if (item) curr->SetInstrucPacketFile(_restring(item->GetText(), 0, strlen(item->GetText())));

		item = element->FirstChildElement("instrucPauseA");
		if (item) curr->SetInstrucPauseA((float)atof(item->GetText()));

		item = element->FirstChildElement("instrucPauseB");
		if (item) curr->SetInstrucPauseB((float)atof(item->GetText()));

		item = element->FirstChildElement("instrucComment");
		if (item) curr->SetInstrucComment(_restring(item->GetText(), 0, strlen(item->GetText())));

		item = element->FirstChildElement("instrucUseTimedUse");
		if (item) curr->SetInstrucUseTimedUse(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("instrucTimedUseA");
		if (item) curr->SetInstrucTimedUseA((float)atof(item->GetText()));

		item = element->FirstChildElement("instrucTimedUseB");
		if (item) curr->SetInstrucTimedUseB((float)atof(item->GetText()));

		item = element->FirstChildElement("instrucNotFirstTimedUse");
		if (item) curr->SetInstrucNotFirstTimedUse(_stricmp(item->GetText(), "true") == 0);
		
		// safety
		item = element->FirstChildElement("safetySurfMin");
		if (item) curr->SetSafetySurfMin(atoi(item->GetText()));

		item = element->FirstChildElement("safetySurfMax");
		if (item) curr->SetSafetySurfMax(atoi(item->GetText()));

		item = element->FirstChildElement("safetyUseSurfCheck");
		if (item) curr->SetSafetyUseSurfCheck(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("safetyUseSurfCheckcon");
		if (item) curr->SetSafetyUseSurfCheckcon(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("safetyUseCoordCheck");
		if (item) curr->SetSafetyUseCoordCheck(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("safetyUseCoordCheckcon");
		if (item) curr->SetSafetyUseCoordCheckcon(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("safetyCoordX");
		if (item) curr->SetSafetyCoordX(atoi(item->GetText()));

		item = element->FirstChildElement("safetyCoordY");
		if (item) curr->SetSafetyCoordY(atoi(item->GetText()));

		item = element->FirstChildElement("safetyUseHPWait");
		if (item) curr->SetSafetyUseHPWait(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("safetyUseMPWait");
		if (item) curr->SetSafetyUseMPWait(_stricmp(item->GetText(), "true") == 0);
		
		// warnings
		item = element->FirstChildElement("warningsTakeScreenshot");
		if (item) curr->SetWarningsTakeScreenshot(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("warningsPlayAudio");
		if (item) curr->SetWarningsPlayAudio(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("warningsAudioFile");
		if (item) curr->SetWarningsAudioFile(_restring(item->GetText(), 0, strlen(item->GetText())));

		item = element->FirstChildElement("warningsJump");
		if (item) curr->SetWarningsJump(_stricmp(item->GetText(), "true") == 0);

		item = element->FirstChildElement("warningsJumpA");
		if (item) curr->SetWarningsJumpA(atoi(item->GetText()));

		item = element->FirstChildElement("warningsJumpB");
		if (item) curr->SetWarningsJumpB(atoi(item->GetText()));

		item = element->FirstChildElement("warningsJAL");
		if (item) curr->SetWarningsJAL(_stricmp(item->GetText(), "true") == 0);

		element = element->NextSiblingElement();
	}

	ResetTransform();
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Computron::SendCap(char* file, char* adapter, char* s)
{
	//char buffer[256];

	//pcap_t* inDesc = NULL;
	//pcap_t* outDesc = NULL;
	//char errorBuffer[PCAP_ERRBUF_SIZE];
	//char source[PCAP_BUF_SIZE];
	//FILE* capFilename = NULL;
	//int capLength, sync;
	//u_int res;
	//pcap_send_queue* sendQueue = NULL;
	//struct pcap_pkthdr* packetHeader = NULL;
	//const u_char* packetData = NULL;
	//float cpuTime;
	//u_int npacks = 0;
	//	
	//// retrieve the length of the capture file
	//fopen_s(&capFilename, file,"rb");

	//if(capFilename == NULL)
	//{
	//	AddWarnLog(-1, "Capture file not found");
	//	UseWarnings(buffer);
	//	return;
	//}
	//
	//fseek(capFilename , 0, SEEK_END);
	//capLength = ftell(capFilename)- sizeof(struct pcap_file_header);
	//fclose(capFilename);
	//		
	//// chek if the timestamps must be respected
	//if(s[0] == 's')
	//	sync = TRUE;
	//else
	//	sync = FALSE;

	//// open the capture
	//// create the source string according to the new WinPcap syntax
	//if (_pcap_createsrcstr(	source,			// variable that will keep the source string
	//						PCAP_SRC_FILE,	// we want to open a file
	//						NULL,			// remote host
	//						NULL,			// port on the remote host
	//						file,			// name of the file we want to open
	//						errorBuffer			// error buffer
	//						) != 0)
	//{
	//	AddWarnLog(-1, "Error creating a source string");
	//	UseWarnings(buffer);
	//	return;
	//}
	//
	//// open the capture file
	//if ((inDesc = _pcap_open(source, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errorBuffer)) == NULL)
	//{
	//	sprintf_s(buffer, sizeof(buffer), "Unable to open the file %s", source);
	//	AddWarnLog(-1, buffer);
	//	UseWarnings(buffer);
	//	return;
	//}

	//// open the output adapter
	//if ((outDesc = _pcap_open(adapter, 100, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errorBuffer)) == NULL)
	//{
	//	sprintf_s(buffer, sizeof(buffer), "Unable to open adapter %s", adapter);
	//	AddWarnLog(-1, buffer);
	//	UseWarnings(buffer);
	//	return;
	//}

	//// check the mac type
	//if (_pcap_datalink(inDesc) != _pcap_datalink(outDesc))
	//{
	//	AddWarnLog(-1, "Warning: the datalink of the capture differs from the one of the selected interface");
	//	UseWarnings(buffer);
	//}

	//// allocate a send queue
	//sendQueue = _pcap_sendqueue_alloc(capLength);

	//// fill the queue with the packets from the file
	//while ((res = _pcap_next_ex(inDesc, &packetHeader, &packetData)) == 1)
	//{
	//	if (_pcap_sendqueue_queue(sendQueue, packetHeader, packetData) == -1)
	//	{
	//		AddWarnLog(-1, "Warning: packet buffer too small, not all the packets will be sent");
	//		UseWarnings(buffer);
	//		break;
	//	}

	//	npacks++;
	//}

	//if (res == -1)
	//{
	//	AddWarnLog(-1, "Corrupted input file");
	//	UseWarnings(buffer);
	//	_pcap_sendqueue_destroy(sendQueue);
	//	return;
	//}

	//// transmit the queue

	//cpuTime = (float)clock();

	//if ((res = _pcap_sendqueue_transmit(outDesc, sendQueue, sync)) < sendQueue->len)
	//{
	//	sprintf_s(buffer, sizeof(buffer), "An error occurred sending the packets: %s. Only %d bytes were sent", _pcap_geterr(outDesc), res);
	//	AddWarnLog(-1, buffer);
	//	UseWarnings(buffer);
	//}
	//
	//cpuTime = (clock() - cpuTime)/CLK_TCK;

	//// free the send queue
	//_pcap_sendqueue_destroy(sendQueue);

	//// Close the input file
	//_pcap_close(inDesc);

	//// lose the output adapter 
	//// IMPORTANT: remember to close the adapter, otherwise there will be no guarantee that all the 
	//// packets will be sent!
	//_pcap_close(outDesc);
}


//-------------------------------------------------------
// bDown is true when button is down (can be ignored)
// bRepeat is true if button is held down
// return false to stop processing of other keys
//-------------------------------------------------------
bool Computron::HandleKeyboard()
{
	if (logout) return true;

	KeyInfo xKey = _Key();

	if (xKey.bDown)
	{
		if (instrucCurrent && xKey.cKey == pauseRestartKey)
		{
			return false;
		}
		else if (instrucCurrent && xKey.cKey == pauseResumeKey)
		{
			return false;
		}
	}
	else if (xKey.bRepeat == false)
	{
		if (instrucCurrent && xKey.cKey == pauseRestartKey)
		{
			if (transform == false)
				ResetTransform();

			transform = !transform;

			return false;
		}
		else if (instrucCurrent && xKey.cKey == pauseResumeKey)
		{
			if (!transform && instrucCurrent == NULL)
				ResetTransform();
			
			transform = !transform;

			return false;
		}
	}

	return true;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
Computron::Computron(void)
{
	initialized = false;

	logout = false;

	g_progressHP = NULL;
	g_progressMP = NULL;
	g_progressXP = NULL;

	g_locationX = NULL;
	g_locationY = NULL;

	locationX = 0;
	locationY = 0;

	progressHP = 0.0f;
	progressMP = 0.0f;
	progressXP = 0.0f;
	
	pauseRestartKey = 0x00;
	pauseResumeKey = 0x00;

	extComputron = NULL;
	panel1 = NULL;
	p1TextBar1 = NULL;
	p1ListViewer1 = NULL;
	p1ListViewer2 = NULL;

	g_hInetConnection = NULL;
	g_hInet = NULL;
	
	captureScreen = false;

	zeroPause = false;

	okayHP = true;
	okayMP = true;

	checkconUsed = false;

	jjump = false;
	minuteJumpTriggered = false;

	transform = false;
	
	surfCount = 0;
	surfSaved = 0;

	countTotal = 0;
	countSpecial = 0;
	countNormal = 0;

	ranTimes = 0;

	screenSizeA = 0.0f;
	screenSizeB = 0.0f;
			
	useHealthKey = false;
	healthPercentTrigger = 0;
	healthMonitorKey = 0x00;
			
	useManaKey = false;
	manaPercentTrigger = 0;
	manaMonitorKey = 0x00;

	totalPerc = 0.0f;
	lastXP = 0.0f;
	hrTillHundert = 0.0f;
	percPerHr = 0.0f;
	initPerc = 0.0f;
	startPerc = 0.0f;
	endPerc = 0.0f;

	scriptFilename = "";
	scriptHandle = "";
	scriptPassword = "";
	
	pauseLength = 0.0f;
	pauseStart = 0.0f;

	instrucHeader = NULL;
	instrucCurrent = NULL;
	instrucPrevious = NULL;

	jreturnStack = NULL;

	PacketMod = NULL;
	pthreadVCMod = NULL;
	WanPacketMod = NULL;
	wpcapMod = NULL;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
bool Computron::Uninitialize(void)
{
	initialized = false;

	if (extComputron)
	{
		if (_CheckExtMenuIO(extComputron))
			_WriteConfigItem("MenuItemIO", "true", "XenDLL Files/Extensions/Computron/Computron.xml");
		else
			_WriteConfigItem("MenuItemIO", "false", "XenDLL Files/Extensions/Computron/Computron.xml");

		_UnloadExtension(extComputron);
	}

	ClearInstructions();

	if (wpcapMod)
	{
		FreeLibrary(wpcapMod);
	}

	if (pthreadVCMod)
	{
		FreeLibrary(pthreadVCMod);
	}

	if (PacketMod)
	{
		FreeLibrary(PacketMod);
	}

	if (WanPacketMod)
	{
		FreeLibrary(WanPacketMod);
	}

	if (g_hInetConnection)
	{
		InternetCloseHandle(g_hInetConnection);
		g_hInetConnection = NULL;
	}

	if (g_hInet)
	{
		InternetCloseHandle(g_hInetConnection);
		g_hInetConnection = NULL;
	}

	return initialized;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
Computron::~Computron(void)
{
	Uninitialize();
}

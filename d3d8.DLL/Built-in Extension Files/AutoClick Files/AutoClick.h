#ifndef AutoClick_H
#define AutoClick_H

#pragma once

class AutoClick;
extern AutoClick* g_AutoClick;

class AutoClick
{
public:

	bool initialized;
	bool Initialize(void);

	AutoClick(void);
	~AutoClick(void);

	// direct input hook calls
	HGNodeList* extAutoClick;
	int triggerButton;

	// extension calls
	bool HandleKeyboard(void);

private:

	BYTE triggerKey;

	bool Uninitialize(void);
};

#endif

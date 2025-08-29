#pragma once
#include "Module.h"

#define ELEVATE_BUTTON_TEXT L"Elevate Player"
#define DEELEVATE_BUTTON_TEXT L"De-Elevate Player"

enum ElevatorButtons
{
	ElevateButton = 0,
	MuteToggleButton = 1
};

class Elevator : public Module
{
	Elevator();
public:
	void CreateMenu();
	void RemoveMenu();

public:
	static Elevator* Get();
};
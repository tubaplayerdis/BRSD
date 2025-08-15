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
	void CreateButton();
	void DestroyHooks();
	//Nulls the current button ref
	void NullifyRefrence();
	bool DoesButtonRefMatch(void* Ref, ElevatorButtons but);
	void OnElevateButtonClicked();
	void OnMuteToggleButtonClicked();

public:
	static Elevator* Get();
};
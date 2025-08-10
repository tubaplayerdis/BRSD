#pragma once
#include "Module.h"

#define ELEVATE_BUTTON_TEXT L"Elevate Player"
#define DEELEVATE_BUTTON_TEXT L"De-Elevate Player"

class Elevator : public Module
{
	Elevator();
public:
	void CreateButton();
	void DestroyHooks();
	//Nulls the current button ref
	void NullifyRefrence();
	bool DoesButtonRefMatch(void* Ref);
	void OnElevateButtonClicked();

public:
	static Elevator* Get();
};
#pragma once
#include "Module.h"

#define ELEVATE_BUTTON_TEXT L"Elevate Player"
#define DEELEVATE_BUTTON_TEXT L"De-Elevate Player"

class Elevator : public Module
{
	Elevator();
public:
	void CreateButton();

public:
	static Elevator* Get();
};
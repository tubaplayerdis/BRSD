/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     watermark.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>

namespace watermark
{
	inline SDK::UCanvasPanel* panel = nullptr;
	inline SDK::UBrickBorder* TextBorder = nullptr;
	bool InitalizeWaterMark();
	void ShowWaterMark();
	void HideWaterWark();
	void UnInitalizeWaterMark();
}
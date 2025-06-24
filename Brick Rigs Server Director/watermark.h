#pragma once
#include <SDK.hpp>

namespace watermark
{
	inline SDK::UCanvasPanel* panel = nullptr;
	inline SDK::UPanelWidget* RootPanel = nullptr;
	inline SDK::UBrickBorder* TextBorder = nullptr;
	bool InitalizeWaterMark();
	void ShowWaterMark();
	void HideWaterWark();
	void UnInitalizeWaterMark();
}
#pragma once
#include <SDK.hpp>


namespace FSlateColor
{
	/*Use this by passing in a valid linear color and a FSlateColor created by the default constructor*/
	void FSlateColorConstructor(SDK::FSlateColor* This, SDK::FLinearColor* InColor);

	/*
	* Colors are 0-255 and A is 0-1
	*/
	SDK::FSlateColor CreateSlateColor(float R, float G, float B, float A);
}

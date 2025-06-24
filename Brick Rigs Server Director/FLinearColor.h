#pragma once
#include <SDK.hpp>

namespace FLinearColor
{
	/*
	* Colors are 0-255 and A is 0-1
	* FLinearColor expect values from 0-1 not 0-255, given that, 0-255 values are taken and mapped down in the function.
	*/
	SDK::FLinearColor CreateLinearColor(float R, float G, float B, float A);


	/*
	* Colors are 0-1 and A is 0-1
	* FLinearColor expect values from 0-1 not 0-255, and since this is the normal function, inputs are 0-1.
	*/
	SDK::FLinearColor CreateLinearColorNorm(float R, float G, float B, float A);
}
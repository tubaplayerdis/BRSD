#include "FLinearColor.h"

SDK::FLinearColor FLinearColor::CreateLinearColor(float R, float G, float B, float A)
{
    SDK::FLinearColor ret = SDK::FLinearColor();
    ret.A = A;
    ret.R = R / 255.0f;
    ret.G = G / 255.0f;
    ret.B = B / 255.0f;
    return ret;
}

SDK::FLinearColor FLinearColor::CreateLinearColorNorm(float R, float G, float B, float A)
{
    SDK::FLinearColor ret = SDK::FLinearColor();
    ret.A = A;
    ret.R = R;
    ret.G = G;
    ret.B = B;
    return ret;
}

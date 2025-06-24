#include "UBrickBorder.h"

void UBrickBorder::SynchronizeProperties(SDK::UBrickBorder* This)
{
	uintptr_t SynchronizePropertiesFunction = (uintptr_t)GetModuleHandle(NULL) + 0x0DE8030;

	using SynchronizePropertiesFn = void(__fastcall*)(SDK::UBrickBorder* Thiss);
	SynchronizePropertiesFn OnInitalizeFunction = reinterpret_cast<SynchronizePropertiesFn>(SynchronizePropertiesFunction);

	OnInitalizeFunction(This);
}

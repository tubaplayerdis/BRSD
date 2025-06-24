#include "FBrickChatMessage.h"
#include <windows.h>

void FBrickChatMessage::FBrickChatMessageConstructor(SDK::FBrickChatMessage* This, SDK::EChatMessageType ChatType, SDK::ABrickPlayerController* PC)
{
    uintptr_t FBrickChatMessageConstructorP = (uintptr_t)GetModuleHandle(NULL) + 0x0D0EA10;

    using SetLoadoutAccessorFn = void(__fastcall*)(SDK::FBrickChatMessage* Thiss, SDK::EChatMessageType ChatTypee, SDK::ABrickPlayerController* PCC);
    SetLoadoutAccessorFn OnFBrickChatMessageConstructor = reinterpret_cast<SetLoadoutAccessorFn>(FBrickChatMessageConstructorP);

    OnFBrickChatMessageConstructor(This, ChatType, PC);
}

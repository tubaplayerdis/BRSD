/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     messages.cpp                                                */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "messages.h"
#include "functions.h"
#include "global.h"

#define TEXT(text) SDK::UKismetTextLibrary::Conv_StringToText(SDK::FString(text))

void messages::sendUserSpecificMessage(PlayerInfo info, std::string message)
{
    if (!global::isMapValid()) return;
    SDK::FText Fmessage = TEXT(global::to_wstring_n(message).c_str());
    SDK::FBrickChatMessage SMessage;
    SDK::ABrickPlayerController* cont = GetBrickPlayerControllerFromName(info.name);
    if (cont != nullptr) {
        FBrickChatMessage::FBrickChatMessageConstructor(&SMessage, SDK::EChatMessageType::Message, cont);
        SMessage.TextOption = Fmessage;
        SMessage.Type = SDK::EChatMessageType::Message;
        SMessage.IntOption = 1;//Equates to SDK::EChatContext. use this to get admin messages or other types of messages.
        SMessage.Player.PlayerId = cont->GetPlayerId();
        SMessage.Player.PlayerName = STRING(L"BRSD");
        cont->ClientReceiveChatMessage(SMessage);
    }
    else {
        auto PlayerController = GetBrickPlayerController();
        if (PlayerController == nullptr) return;
        SDK::FText FmessageN = SDK::UKismetTextLibrary::Conv_StringToText(SDK::UKismetStringLibrary::Concat_StrStr(UC::FString(L"Message Failed To Send To: "), UC::FString(global::to_wstring_n(info.name).c_str())));
        auto SMessageN = SDK::FBrickChatMessage();
        SMessageN.TextOption = FmessageN;
        PlayerController->ClientReceiveChatMessage(SMessageN);
    }
}

void messages::sendUserSpecificMessageWithContext(PlayerInfo info, std::string message, SDK::EChatContext context, const wchar_t* sender)
{
    if (!global::isMapValid()) return;
    SDK::FText Fmessage = TEXT(global::to_wstring_n(message).c_str());
    SDK::FBrickChatMessage SMessage;
    SDK::ABrickPlayerController* cont = GetBrickPlayerControllerFromName(info.name);
    if (cont != nullptr) {
        FBrickChatMessage::FBrickChatMessageConstructor(&SMessage, SDK::EChatMessageType::Message, cont);
        SMessage.TextOption = Fmessage;
        SMessage.Type = SDK::EChatMessageType::Message;
        SMessage.IntOption = (int)context;
        SMessage.Player.PlayerId = cont->GetPlayerId();
        SMessage.Player.PlayerName = STRING(sender);
        cont->ClientReceiveChatMessage(SMessage);
    }
    else {
        auto PlayerController = GetBrickPlayerController();
        if (PlayerController == nullptr) return;
        SDK::FText FmessageN = SDK::UKismetTextLibrary::Conv_StringToText(SDK::UKismetStringLibrary::Concat_StrStr(UC::FString(L"Message Failed To Send To: "), UC::FString(global::to_wstring_n(info.name).c_str())));
        auto SMessageN = SDK::FBrickChatMessage();
        SMessageN.TextOption = FmessageN;
        PlayerController->ClientReceiveChatMessage(SMessageN);
    }
}

void messages::sendMessageToAdmin(std::string message)
{
	if (!global::isMapValid()) return;
	sendUserSpecificMessageWithContext(GetPlayerInfoFromController(GetBrickPlayerController()), message, SDK::EChatContext::Admin, L"BRSD");
}

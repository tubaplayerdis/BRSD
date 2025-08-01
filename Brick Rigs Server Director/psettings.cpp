/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     psettings.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "psettings.h"
#include "uibase.h"
#include "global.h"
#include "functions.h"
#include "SetSelectedItem.h"
#include "utils.h"
#include <chrono>


template<typename T>
T* psettings::elements::GetWidget(const char* tag)
{
    for (UISetting setting : list)
    {
        if (strcmp(tag, setting.tag) == 0) return static_cast<T*>(setting.ElementPtr);
    }
    ASSERT(false, "Widget could not be found! Aborting!");
}

void __fastcall psettings::LoaderReturn(void* input)
{
    std::cout << "loading!\n";
    LoadPackage(nullptr, L"/Game/BrickRigs/UI/Properties/WBP_PropertyContainer");
    isLoaded = true;
}

struct FModuleManager
{
    uint8_t pad[0x3D0];
};

struct FdepQ
{
    uint8_t req;
    uint8_t exclu;
};

bool psettings::CreateCustomSettingsPageBase()
{
    MockPage = SpawnObject(SDK::UMenuPageWidget, GetMenu());
    CustomSettingsPage = SpawnObject(SDK::UBrickScrollBox, GetWindowManager());
    CustomSettingsPage->SetSlotSpacingStyle(SDK::EBrickUISpacingStyle::Large);

    SDK::UBrickTextBlock* TextBlock = SpawnObject(SDK::UBrickTextBlock, CustomSettingsPage);
    if (!TextBlock) return false;
    TextBlock->SetText(TEXT(L"Brick Rigs Server Director"));
    TextBlock->SetTextStyle(SDK::EBrickUITextStyle::Bold);
    SDK::UBrickBorder* TextBorder = SpawnObject(SDK::UBrickBorder, CustomSettingsPage);
    if (!TextBorder) return false;
    TextBorder->SetContent(TextBlock);
    TextBorder->ForceLayoutPrepass();
    SDK::UScrollBoxSlot* slot2 = static_cast<SDK::UScrollBoxSlot*>(CustomSettingsPage->AddChild(TextBorder));
    slot2->SetHorizontalAlignment(SDK::EHorizontalAlignment::HAlign_Center);
    slot2->SetVerticalAlignment(SDK::EVerticalAlignment::VAlign_Center);
    TextBorder->SetHorizontalAlignment(SDK::EHorizontalAlignment::HAlign_Center);
    TextBorder->SetVerticalAlignment(SDK::EVerticalAlignment::VAlign_Center);

    std::cout << "spacer" << std::endl;

    SDK::UWBP_PropertyContainer_C* CCcontainer = CreateWidget(SDK::UWBP_PropertyContainer_C);
    CCcontainer->NameTextBlock->SetText(TEXT(L"Chat Commands Enabled"));
    CustomSettingsPage->AddChild(CCcontainer);

    SDK::UWBP_PropertyContainer_C* BLcontainer = CreateWidget(SDK::UWBP_PropertyContainer_C);
    BLcontainer->NameTextBlock->SetText(TEXT(L"Blacklist Enabled"));
    CustomSettingsPage->AddChild(BLcontainer);

    elements::BRSDBlock = TextBorder;
    elements::ChatCommandsPC = CCcontainer;
    elements::list.push_back(UISetting(TextBorder, PS_BLOCK));
    elements::list.push_back(UISetting(CCcontainer, PS_CC_CONTAINER));
    elements::list.push_back(UISetting(BLcontainer, PS_BL_CONTAINER));

    return true;
}

void psettings::PrepareCustomSettingsPage()
{
    SDK::UWBP_BoolProperty_C* cb = CreateWidget(SDK::UWBP_BoolProperty_C);
    elements::GetWidget<SDK::UWBP_PropertyContainer_C>(PS_CC_CONTAINER)->AddPropertyWidget(cb, SDK::EOrientation::Orient_Horizontal);
    elements::GetWidget<SDK::UWBP_PropertyContainer_C>(PS_CC_CONTAINER)->PropertyWidget = cb;//Not set manually for some reason.
    cb->ComboBox->InitItems(2, 1);
    cb->ComboBox->SetSelectedItem(1);//Set this based off of loaded settings

    SDK::UWBP_BoolProperty_C* blcb = CreateWidget(SDK::UWBP_BoolProperty_C);
    elements::GetWidget<SDK::UWBP_PropertyContainer_C>(PS_BL_CONTAINER)->AddPropertyWidget(blcb, SDK::EOrientation::Orient_Horizontal);
    elements::GetWidget<SDK::UWBP_PropertyContainer_C>(PS_BL_CONTAINER)->PropertyWidget = blcb;
    blcb->ComboBox->InitItems(2, 1);
    blcb->ComboBox->SetSelectedItem(1);
}

void psettings::SetHook(bool toggle) 
{
    if (toggle) hooks::S_SetSelectedItem->Enable();
    else hooks::S_SetSelectedItem->Disable();
}

void psettings::SetVisibility(SDK::ESlateVisibility vis)
{
    CustomSettingsPage->SetVisibility(vis);
}

void psettings::Uninitalize()
{
    if (GetMenu() && GetMenu()->CurrentMenuPage && GetMenu()->CurrentMenuPage == psettings::MockPage) {
        static_cast<SDK::UBrickBorder*>(psettings::CustomSettingsPage->Slot->Parent)->SetContent(psettings::MockPage);
    }

    elements::BRSDBlock = nullptr;
    elements::ChatCommandsPC = nullptr;
    for (int i = 0; i < elements::list.size(); i++)
    {
        elements::list[i].ElementPtr->RemoveFromParent();
        elements::list[i].ElementPtr = nullptr;
    }

    MockPage = nullptr;
    CustomSettingsPage->ClearChildren();
    CustomSettingsPage->RemoveFromParent();
    CustomSettingsPage == nullptr;
}

uintptr_t CastPointer(void* ptr)
{
    return (uintptr_t)ptr;
}

bool psettings::elements::IsSettingsContainer(SDK::UPropertyWidget* comp, SDK::UPropertyWidget* box)
{
    if (!box || !comp) return false;
    
    return comp == box;
}

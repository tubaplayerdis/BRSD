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
#include "OnComboBoxMenuItemSelected.h"
#include "spawnutils.h"

void __fastcall psettings::LoaderReturn(void* input)
{
    std::cout << "loading!\n";
    LoadPackage(nullptr, L"/Game/BrickRigs/UI/Properties/WBP_PropertyContainer");
    isLoaded = true;
}

bool psettings::CreateCustomSettingsPage()
{
    MockPage = Spawn(SDK::UMenuPageWidget, GetMenu());
    CustomSettingsPage = Spawn(SDK::UBrickScrollBox, GetWindowManager());
    CustomSettingsPage->SetSlotSpacingStyle(SDK::EBrickUISpacingStyle::Large);

    SDK::UBrickTextBlock* TextBlock = Spawn(SDK::UBrickTextBlock, CustomSettingsPage);
    if (!TextBlock) return false;
    TextBlock->SetText(TEXT(L"Brick Rigs Server Director"));
    TextBlock->SetTextStyle(SDK::EBrickUITextStyle::Bold);
    SDK::UBrickBorder* TextBorder = Spawn(SDK::UBrickBorder, CustomSettingsPage);
    if (!TextBorder) return false;
    TextBorder->SetContent(TextBlock);
    TextBorder->ForceLayoutPrepass();
    SDK::UScrollBoxSlot* slot2 = static_cast<SDK::UScrollBoxSlot*>(CustomSettingsPage->AddChild(TextBorder));
    slot2->SetHorizontalAlignment(SDK::EHorizontalAlignment::HAlign_Center);
    slot2->SetVerticalAlignment(SDK::EVerticalAlignment::VAlign_Center);
    TextBorder->SetHorizontalAlignment(SDK::EHorizontalAlignment::HAlign_Center);
    TextBorder->SetVerticalAlignment(SDK::EVerticalAlignment::VAlign_Center);

    SDK::UWBP_Menu_C::StaticClass();
    /*
    SDK::FString path = UC::FString(L"/Game/BrickRigs/UI/Properties/WBP_PropertyContainer");
    std::cout << path.Num() << std::endl;
    SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)> del = SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)>();
    LoadPackageAsync(path, del);
    */

    std::cout << "spacer" << std::endl;

    SDK::UWBP_PropertyContainer_C* container = Create(SDK::UWBP_PropertyContainer_C);
    if (!container) {
        std::cout << "had" << std::endl;
        GetMenu()->OnClickedGameplaySettings();
        GetMenu()->StepBack();
        container = Create(SDK::UWBP_PropertyContainer_C);
    }
    container->NameTextBlock->SetText(TEXT(L"Chat Commands Enabled"));

    CustomSettingsPage->AddChild(container);

    elements::BRSDBlock = TextBorder;
    elements::ChatCommandsPC = container;
    elements::list.push_back(TextBorder);
    elements::list.push_back(container);

    std::cout << "Created UI Elements!" << std::endl;

    //Add to the ClassPool.
    //classPool.push_back(*SDK::UWBP_PropertyContainer_C::StaticClass())





    return true;
}

void psettings::PrepareCustomSettingsPage()
{
    std::cout << "preparing page!" << std::endl;
    std::cout << SDK::UWBP_BoolProperty_C::StaticClass() << std::endl;
    SDK::UWBP_BoolProperty_C* cb = Create(SDK::UWBP_BoolProperty_C);
    cb->ComboBox = Create(SDK::UWBP_BrickComboBox_C);
    cb->ComboBox->InitItems(2, 1);
    elements::ChatCommandsPC->AddPropertyWidget(nullptr, SDK::EOrientation::Orient_Horizontal);
    std::cout << "prepared page!" << std::endl;
}

void psettings::SetHook(bool toggle)
{
    toggle ? hooks::S_OnComboBoxMenuItemSelected->Enable() : hooks::S_OnComboBoxMenuItemSelected->Disable();
}

void psettings::SetVisibility(SDK::ESlateVisibility vis)
{
    CustomSettingsPage->SetVisibility(vis);
}

void psettings::Uninitalize()
{
    if (GetMenu()->CurrentMenuPage && GetMenu()->CurrentMenuPage == psettings::MockPage) {
        static_cast<SDK::UBrickBorder*>(psettings::CustomSettingsPage->Slot->Parent)->SetContent(psettings::MockPage);
    }

    elements::BRSDBlock = nullptr;
    elements::ChatCommandsPC = nullptr;
    for (int i = 0; i < elements::list.size(); i++)
    {
        elements::list[i]->RemoveFromParent();
        elements::list[i] = nullptr;
    }

    MockPage = nullptr;
    CustomSettingsPage->ClearChildren();
    CustomSettingsPage->RemoveFromParent();
    CustomSettingsPage == nullptr;
}

bool psettings::elements::IsComboBox(SDK::UWBP_PropertyContainer_C* propc, SDK::UBrickComboBoxWidget* cbox)
{
    if (!propc || !cbox) return false;
    if (!propc->PropertyWidget) return false;
    if (!propc->PropertyWidget->IsA(SDK::UWBP_BoolProperty_C::StaticClass())) return false;
    SDK::UWBP_BoolProperty_C* bp = static_cast<SDK::UWBP_BoolProperty_C*>(propc->PropertyWidget);
    if (!bp->ComboBox) return false;
    return bp->ComboBox == cbox;
}

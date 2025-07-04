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
    //SDK::FString path = SDK::FString(L"/Game/BrickRigs/UI/Properties/WBP_PropertyContainer.WBP_PropertyContainer_C");
    //SDK::UClass* ptr = ConstructorHelpersInternal::FindOrLoadClass(path, SDK::UPropertyContainerWidget::StaticClass());
    //if (!ptr) std::cout << "ugh" << std::endl;
    SDK::FString PackageName = STRING(L"/Game/BrickRigs/UI/Properties/WBP_PropertyContainer");
    SDK::FString FileName;
    bool res = FPackageName::DoesPackageExist(PackageName, nullptr, &FileName);
    if (res) {
        std::cout << "exists! filename: " << FileName.ToString() << std::endl;
        __int64 pack = LoadPackageAsync(PackageName, nullptr);
        std::cout << "Loading Async!" << std::endl;
        Sleep(100);
    }
    SDK::UWBP_PropertyContainer_C* container = Create(SDK::UWBP_PropertyContainer_C);
    if (!container) {
        std::cout << "had to load container" << std::endl;
        GetMenu()->OnClickedAdminSettings();
        GetMenu()->OnClickedBack();
        container = Create(SDK::UWBP_PropertyContainer_C);
        //This is so hacky and stupid i hate this but Unreal Engine leaves me no alternatives what is wrong with you unreal engine.
    }
  
    CustomSettingsPage->AddChild(container);

    //SynchronizeProperties(TextBorder);
    //SynchronizeProperties(CustomSettingsPage);

    ElementsList.push_back(TextBorder);
    ElementsList.push_back(container);

    std::cout << "Created UI Elements!" << std::endl;
    return true;
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

    MockPage = nullptr;
    ElementsList.clear();
    CustomSettingsPage->ClearChildren();
    CustomSettingsPage->RemoveFromParent();
    CustomSettingsPage == nullptr;
}

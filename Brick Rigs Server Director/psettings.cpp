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

void psettings::VerifyController()
{
    std::cout << World()->OwningGameInstance->LocalPlayers.Max() << std::endl;
    if (!World()->OwningGameInstance->LocalPlayers[0]->PlayerController) {
        
    }
}

bool psettings::CreateCustomSettingsPage()
{
    MockPage = Spawn(SDK::UMenuPageWidget, GetMenu());


    CustomSettingsPage = Spawn(SDK::UBrickScrollBox, GetWindowManager());
    CustomSettingsPage->SetSlotSpacingStyle(SDK::EBrickUISpacingStyle::Large);
    CustomSettingsPage->SetVisibility(SDK::ESlateVisibility::Hidden);
    SDK::UCanvasPanelSlot* slot = GetCanvasPanel()->AddChildToCanvas(CustomSettingsPage);
    SDK::FAnchors anchor = SDK::FAnchors();
    anchor.Minimum.X = 0.214f;
    anchor.Minimum.Y = 0.16f;
    anchor.Maximum.X = 0.993f;
    anchor.Maximum.Y = 0.988f;
    slot->SetAnchors(anchor);
    slot->SetAlignment(SDK::FVector2D(0.f, 0.f));
    slot->SetSize(SDK::FVector2D(0.0f, 0.0f));
    slot->SetZOrder(1000);

    SDK::UBrickTextBlock* TextBlock = Spawn(SDK::UBrickTextBlock, CustomSettingsPage);
    if (!TextBlock) return false;
    TextBlock->SetText(TEXT(L"Brick Rigs Server Director Options"));
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
    

    SDK::UWBP_PropertyContainer_C* container = Create(SDK::UWBP_PropertyContainer_C);
    if (!container) { 
        GetMenu()->OnClickedAdminSettings();
        GetMenu()->OnClickedBack();
        container = Create(SDK::UWBP_PropertyContainer_C);
    }

    
    //SDK::UWBP_BrickTextBox_C* widget = static_cast<SDK::UWBP_BrickTextBox_C*>(WidgetU::CreateWidget(World(), SDK::UWBP_BrickTextBox_C::StaticClass(), SDK::FName()));
    //if (widget) {
    //    std::cout << GetBoolString(widget->MultiLineTextBox) << std::endl;
    //    std::cout << GetBoolString(widget->WidgetTree) << std::endl;
    //    widget->SetVisibility(SDK::ESlateVisibility::Visible);
    //}
  

    CustomSettingsPage->AddChild(container);
    //CustomSettingsPage->AddChild(widget);

  
    SynchronizeProperties(TextBorder);
    SynchronizeProperties(CustomSettingsPage);

    ElementsList.push_back(TextBorder);
    //ElementsList.push_back(widget);
    ElementsList.push_back(container);


    CustomSettingsPage->SetVisibility(SDK::ESlateVisibility::Collapsed);
    return true;
}

void psettings::SetVisibility(SDK::ESlateVisibility vis)
{
    CustomSettingsPage->SetVisibility(vis);
}

void psettings::Uninitalize()
{
    ElementsList.clear();
    CustomSettingsPage->ClearChildren();
    CustomSettingsPage->RemoveFromParent();
    CustomSettingsPage == nullptr;
}

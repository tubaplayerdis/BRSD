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
    CustomSettingsPage->SetSlotSpacingStyle(SDK::EBrickUISpacingStyle::Medium);
    CustomSettingsPage->SetSlotSpacing(10.f);
    CustomSettingsPage->SetVisibility(SDK::ESlateVisibility::Hidden);
    SDK::UCanvasPanelSlot* slot = GetCanvasPanel()->AddChildToCanvas(CustomSettingsPage);
    SDK::FAnchors anchor = SDK::FAnchors();
    anchor.Minimum.X = 0.214f;
    anchor.Minimum.Y = 0.159f;
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
    SDK::UPanelSlot* slot2 = CustomSettingsPage->AddChild(TextBorder);
    //TextBorder->ForceLayoutPrepass(); //Forces the correct size
    //slot2->SetAutoSize(true);
    //SDK::FAnchors anchos = SDK::FAnchors();
    //anchos.Maximum = SDK::FVector2D(0.5f, 0.0f);
    //anchos.Minimum = SDK::FVector2D(0.5f, 0.0f);
    //slot2->SetAnchors(anchos);
    //slot2->SetAlignment(SDK::FVector2D(0.5f, 0.0f));
    SynchronizeProperties(TextBorder);

    ElementsList.push_back(TextBorder);

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

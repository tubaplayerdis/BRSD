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


    CustomSettingsPage = Spawn(SDK::UCanvasPanel, GetWindowManager());
    CustomSettingsPage->SetVisibility(SDK::ESlateVisibility::Hidden);
    SDK::UCanvasPanelSlot* slot = GetCanvasPanel()->AddChildToCanvas(CustomSettingsPage);
    slot->SetAutoSize(true);
    SDK::FAnchors anchor = SDK::FAnchors();
    anchor.Minimum.X = 0.3;
    anchor.Minimum.Y = 0.2;
    anchor.Maximum.X = 0.3;
    anchor.Maximum.Y = 0.2;
    slot->SetAnchors(anchor);
    slot->SetAlignment(SDK::FVector2D(1.f, 0.f));
    slot->SetSize(SDK::FVector2D(200.0f, 200.0f));


    SDK::UBrickTextBlock* TextBlock = Spawn(SDK::UBrickTextBlock, CustomSettingsPage);
    if (!TextBlock) return false;
    TextBlock->SetText(TEXT(L"Testing!"));
    SDK::UBrickBorder* TextBorder = Spawn(SDK::UBrickBorder, CustomSettingsPage);
    if (!TextBorder) return false;
    TextBorder->SetContent(TextBlock);
    SDK::UCanvasPanelSlot* slot2 = CustomSettingsPage->AddChildToCanvas(TextBorder);
    slot2->SetAutoSize(true);
    SDK::FAnchors anchos = SDK::FAnchors();
    anchos.Maximum = SDK::FVector2D(1.0f, 0.0f);
    anchos.Minimum = SDK::FVector2D(1.0f, 0.0f);
    slot2->SetAnchors(anchos);  // Top-Right Align
    slot2->SetAlignment(SDK::FVector2D(1.f, 0.f));
    slot2->SetPosition(SDK::FVector2D(0.0f,0.0f));  // x: 20px left, y: 20px down


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
    CustomSettingsPage == nullptr;
    for (int i = 0; i < ElementsList.size(); i++)
    {
        if (ElementsList[i]) {
            ElementsList[i]->RemoveFromParent();
        }
        ElementsList[i] = nullptr;
    }
}

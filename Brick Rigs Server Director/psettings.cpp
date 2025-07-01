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
    CustomSettingsPage = static_cast<SDK::UMenuPageWidget*>(SDK::UGameplayStatics::SpawnObject(SDK::UMenuPageWidget::StaticClass(), GetMenu()));


    SDK::UBrickTextBlock* TextBlock = Spawn(SDK::UBrickTextBlock, GetCanvasPanel());
    if (!TextBlock) return false;
    TextBlock->SetText(TEXT(L"Testing!"));
    SDK::UBrickBorder* TextBorder = Spawn(SDK::UBrickBorder, GetCanvasPanel());
    if (!TextBorder) return false;
    TextBorder->SetContent(TextBlock);
    TextBorder->SetVisibility(SDK::ESlateVisibility::Hidden);
    SDK::UCanvasPanelSlot* slot2 = GetCanvasPanel()->AddChildToCanvas(TextBorder);
    slot2->SetAutoSize(true);
    SDK::FAnchors anchor = SDK::FAnchors();
    anchor.Maximum = SDK::FVector2D(1.0f, 0.0f);
    anchor.Minimum = SDK::FVector2D(1.0f, 0.0f);
    slot2->SetAnchors(anchor);  // Top-Right Align
    // 2. Align to the top-right of the widget
    slot2->SetAlignment(SDK::FVector2D(1.f, 0.f));

    // 3. Offset inward from top-right screen corner
    std::cout << "X: " << CustomSettingsPage->GetCachedGeometry(). << std::endl;
    std::cout << "Y: " << CustomSettingsPage->RenderTransformPivot.Y << std::endl;

    slot2->SetPosition(CustomSettingsPage->RenderTransformPivot);  // x: 20px left, y: 20px down


    SynchronizeProperties(TextBorder);

    ElementsList.push_back(TextBorder);

    return true;
}

void psettings::SetVisibilityOfElements(SDK::ESlateVisibility vis)
{
    for (SDK::UWidget* widget : ElementsList)
    {
        if(widget) widget->SetVisibility(vis);
    }
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

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

    //SDK::UPropertyContainerWidget* container = Spawn(SDK::UPropertyContainerWidget, CustomSettingsPage);
    //container->PreAddPropertyButtons();
    //container->NameTextBlock = Spawn(SDK::UBrickTextBlock, container);
    //container->NameTextBlock->SetText(TEXT(L"Testing!"));
    //SDK::UGenericPropertyWidget* Generic = Spawn(SDK::UGenericPropertyWidget, CustomSettingsPage);
    //auto* TextBox = Spawn(SDK::UBrickTextBoxWidget, CustomSettingsPage);
    //TextBox->Cr
    //Initalize(TextBox);
    //CallGameFunction<void, SDK::UBrickTextBoxWidget*>(0x0DCCA00, TextBox);//Native Construct
    //std::cout << GetBoolString(TextBox->WidgetTree) << std::endl;
    //container->AddPropertyWidget(Generic, SDK::EOrientation::Orient_Horizontal);
    //container->SetVisibility(SDK::ESlateVisibility::Visible);
    //container->PostAddPropertyButtons();
    //container->SetDesiredSizeInViewport(SDK::FVector2D(200, 200));
    //container->ForceLayoutPrepass();

    SDK::UUserWidget* widget = WidgetU::CreateWidget(World(), SDK::UBrickTextBoxWidget::StaticClass(), NAME(L"OHIO"));
    if (widget) {
        std::cout << "grrr" << std::endl;
        std::cout << widget->GetName() << std::endl;
    }
    //Initalize(static_cast<SDK::UBrickTextBoxWidget*>(widget));
    //CallGameFunction<void, SDK::UBrickTextBoxWidget*>(0x0DCCA00, static_cast<SDK::UBrickTextBoxWidget*>(widget));
    //CustomSettingsPage->AddChild(TextBox);

  
    SynchronizeProperties(TextBorder);
    SynchronizeProperties(CustomSettingsPage);

    ElementsList.push_back(TextBorder);
    //ElementsList.push_back(TextBox);

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

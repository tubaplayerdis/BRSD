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

bool psettings::CreateCustomSettingsPage()
{
    CustomSettingsPage = static_cast<SDK::UMenuSettingsPageWidget*>(SDK::UGameplayStatics::SpawnObject(SDK::UMenuSettingsPageWidget::StaticClass(), GetMenu()));
    //CustomSettingsPage->Name = NAME(L"BSRDSettings"); //Causes issues with the gc
    /*
    SDK::UPropertyCategoryWidget* cat = static_cast<SDK::UPropertyCategoryWidget*>(SDK::UGameplayStatics::SpawnObject(SDK::UPropertyCategoryWidget::StaticClass(), CustomSettingsPage));
    cat->CategoryIndex = 0;
    SDK::UPropertyContainerWidget* cot = static_cast<SDK::UPropertyContainerWidget*>(SDK::UGameplayStatics::SpawnObject(SDK::UPropertyContainerWidget::StaticClass(), CustomSettingsPage));
    cot->ButtonPanelWidget = CustomSettingsPage->GetMenuWidget()->ButtonPanel;
    SDK::UBrickTextBlock* textblock = Spawn(SDK::UBrickTextBlock, CustomSettingsPage);
    textblock->SetText(TEXT(L"Hello Sus"));
    cot->NameTextBlock = textblock;
    cot->PropertyListSlotIndex = 0;
    CustomSettingsPage->PropertiesPanel->PropertyCategoryWidgets.Add(cat);
    CustomSettingsPage->PropertiesPanel->PropertyContainerWidgets.Add(cot);
    */
    return true;
}

void psettings::Uninitalize()
{
    CustomSettingsPage == nullptr;
}

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
    return true;
}

void psettings::Uninitalize()
{
    CustomSettingsPage == nullptr;
}

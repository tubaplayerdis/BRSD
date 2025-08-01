/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     psettings.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

//NOTE TO SELF - WHEN USING GUI COMPONENTS MAKE SURE TO USE THE UWBP_COMPONENT IF IT IS A UUSERWIDGET. otherwise it wont render. the UWBP classes run blueprints and other metadata

#pragma once
#include <SDK.hpp>
#include <vector>

#define PS_BLOCK "NameBlock"
#define PS_CC_CONTAINER "CCContainer"
#define PS_BL_CONTAINER "BLContainer"

struct UISetting
{
	SDK::UWidget* ElementPtr;
	const char* tag;
	UISetting(SDK::UWidget* ptr, const char* tagname)
	{
		ElementPtr = ptr;
		tag = tagname;
	}
};

namespace psettings
{
	inline bool isLoaded = false;
	void __fastcall LoaderReturn(void* input);
	inline std::vector<SDK::UClass> classPool = std::vector<SDK::UClass>();
	namespace elements
	{
		inline std::vector<UISetting> list = std::vector<UISetting>();
		inline SDK::UBrickBorder* BRSDBlock = nullptr;
		inline SDK::UWBP_PropertyContainer_C* ChatCommandsPC = nullptr;

		template<typename T>
		T* GetWidget(const char* tag);

		bool IsSettingsContainer(SDK::UPropertyWidget* comp, SDK::UPropertyWidget* box);
	}
	inline SDK::UMenuPageWidget* MockPage = nullptr; //adds the backround of the menu page that looks correct by forcing a BorderPanel to be added.
	inline SDK::UBrickScrollBox* CustomSettingsPage = nullptr;
	bool CreateCustomSettingsPageBase();
	//Called before setting visibility. 
	void PrepareCustomSettingsPage();
	void SetHook(bool toggle);
	void SetVisibility(SDK::ESlateVisibility vis);
	void Uninitalize();
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Module.h													  */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

//The Idea of this class is similar to hooks where modules are singletons and implemented induvidually.

//These macros enforce singleton programming somehow I just thought of that.
#define EnableModule(mod) mod->Enable()
#define DisableModule(mod) mod->Disable()
#define CreateModule(cls, mod) mod = new cls()
#define DestroyModule(mod) DisableModule(mod); delete mod; mod = nullptr

class Module
{

private:
	bool bIsEnabled;
	bool bIsInitalized;
	bool bNeedsInitalization;

public:
	/// <summary>
	/// Creates a module and defines default behavior.
	/// </summary>
	/// <param name="NeedsInitalization">Whether the module requires an initalization function</param>
	/// <param name="vInitilizationFunction">Initalization function if specified</param>
	Module(bool NeedsInitalization, bool(*vInitilizationFunction)(void) = nullptr);

	//Return true if succeeded. Called during Enable(). Enable will return false and not set Enable value if false is returned
	virtual bool Enable_Implementation() = 0;
	//Return true if succeeded. Called during Disable(). Disable will return false and not set Disable value if false is returned
	virtual bool Disable_Implementation() = 0;

	inline void SetEnable(bool toggle) { bIsEnabled = toggle;  }
	inline bool GetEnable() { return bIsEnabled;  }
	inline bool IsDisabled() { return !GetEnable(); }

	bool Enable();
	bool Disable();
};

inline bool Module::Enable()
{
	if (GetEnable()) return true;
	bool bResult = Enable_Implementation();
	bResult ? SetEnable(true) : SetEnable(false);
	return bResult;
}

inline bool Module::Disable()
{
	if (IsDisabled()) return true;
	bool bResult = Disable_Implementation();
	bResult ? SetEnable(false) : SetEnable(true);
	return bResult;
}
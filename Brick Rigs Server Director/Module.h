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

#define CreateModule(cls, mod) mod = new cls()
#define DestroyModule(mod) delete mod; mod = nullptr

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

	virtual bool Enable() = 0;
	virtual bool Disable() = 0;

	inline void SetEnable(bool toggle) { bIsEnabled = toggle;  }
	inline bool GetEnable() { return bIsEnabled;  }

};
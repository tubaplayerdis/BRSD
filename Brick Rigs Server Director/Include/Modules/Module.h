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
#include <vector>
#include <memory>

class Module
{
	static std::vector<std::unique_ptr<Module>> m_modules;

private:
	bool bIsEnabled;
	bool bIsInitalized;

public:
	/// <summary>
	/// Creates a module and defines default behavior.
	/// </summary>
	Module()
	{
		m_modules.push_back(std::unique_ptr<Module>(this));
		bIsEnabled = false;
		bIsInitalized = true;
	}

	/// Define deconstruction logic in the actual deconstructor
	virtual ~Module()
	{
		m_modules.erase(std::remove(m_modules.begin(), m_modules.end(), this), m_modules.end());
	}

	//Return true if succeeded. Called during Enable(). Enable will return false and not set Enable value if false is returned
	virtual bool Enable_Implementation();
	//Return true if succeeded. Called during Disable(). Disable will return false and not set Disable value if false is returned
	virtual bool Disable_Implementation();

	/// Virtual function to run in a loop.
	virtual void Tick_Implementation();

	inline void SetEnable(bool toggle) { bIsEnabled = toggle;  }
	inline bool GetEnable() { return bIsEnabled;  }
	inline bool IsDisabled() { return !GetEnable(); }

	bool Enable();
	bool Disable();

	static void EnableModules()
	{
		for (int i = 0; i < m_modules.size(); i++)
		{
			if (m_modules[i].get()) m_modules[i]->Enable();
		}
	}

	static void DisableModules()
	{
		for (int i = 0; i < m_modules.size(); i++)
		{
			if (m_modules[i].get()) m_modules[i]->Disable();
		}
	}
};

inline bool Module::Enable_Implementation()
{
	return true;
}

inline bool Module::Disable_Implementation()
{
	return true;
}

inline void Module::Tick_Implementation()
{

}

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


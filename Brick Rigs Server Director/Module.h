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

class Module
{
private:
	bool bIsEnabled;
	bool bIsInitalized;
	bool bNeedsInitalization;

public:
	Module(bool NeedsInitalization, bool(*vInitilizationFunction)(void) = nullptr);

	virtual bool Enable() = 0;
	virtual bool Disable() = 0;

};
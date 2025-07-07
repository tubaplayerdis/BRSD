/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     spawnutils.h					                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    7 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include "offsets.h"
#include <windows.h>
#include <SDK.hpp>
#include "spawnutils.h"
#include "Hook.h"

namespace hooks
{
    class Repair;
    inline Repair* S_Repair = nullptr; //Non-Inline causes link 2005

    //Hook is currently not used
    class Repair : public Hook<void, SDK::UBrick*>
    {
    public:

        static void __fastcall HookedFunction(SDK::UBrick* This)
        {
            std::cout << "Brick was repaired!" << std::endl;
            if (This == _spawnutils::PieceOfResistance)
            {
                //do something to handle the delegate.
                return;
            }
            return S_Repair->OriginalFunction(This);
        }

        Repair() : Hook(HRepair, HookedFunction) {}
    };
}
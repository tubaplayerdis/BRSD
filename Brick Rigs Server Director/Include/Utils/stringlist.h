/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     stringlist.h                                                */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <string>

inline std::string MasterHelpMessage = R"(/info
/uninject
/help controls (Host Only)
/help moderation
/help movement
/help enviroment
/help weapons
/help main)";

#ifdef _DEBUG

inline std::string ControlsHelpMessage = R"(BRSD Controls:
Divide - Uninject
Multiply - Toggle Chat Commands
Return - (Fix Freezes))";

#else

inline std::string ControlsHelpMessage = R"(BRSD Controls:
CTRL-U - Uninject
CTRL-T - Toggle Chat Commands
Return - (Fix Freezes))";

#endif // _DEBUG

inline std::string MainHelpMessage = R"(PlayerID's work for pm/block. Try /pid
/pm [who] [content] - Personal Message Someone
/silence - allow incoming personal messages
/unsilence - disallow incoming personal messages
/block [who] - Block a user from personal messaging you
/unblock [who] - Un-Block a user from personal messaging you
/pid - List every players PlayerID)";

inline std::string ModerationHelpMessage = R"(PlayerID's work for mute/unmute. Try /pid
(*) = Admin Only
/off [command] - Disables command (*)
/on [command] - Enables command (*)
/mute [who] - Disable a users ability to message (*)
/unmute - [who] - Re-Enable a users ability to message (*)
/save - Save the current moderation config to the disk (*)
/load - Load the moderation config on the disk (*))";

inline std::string MovementHelpMessage = R"(
/fly - Fly around (slow)
/walk - Walk around
/tp [who] - Teleport to other people
/ghost - Fly around and disable collisions)";

inline std::string EnviromentHelpMessage = R"(
/night - Make it night
/day - Make it day
/rain - Make it rain
/sun - Make it sunny)";

inline std::string WeaponsHelpMessage = R"(Ammo Types:
0 - Default
1 - Incendiary
2 - HighExplosive
3 - TargetSeeking
4 - Guided
5 - Flare
/ammotype [type] - Change your active weapon's ammo type to the desired ammo type number.)";
//List the ammo types later

inline std::string InfoMessage = R"(Brick Rigs Server Director(BRSD), is a mod made for Brick Rigs that server management functionality to your server. It is dll based and server-side only.
Star the GitHub: https://github.com/tubaplayerdis/BRSD
Follow the Creator: https://github.com/tubaplayerdis
)";

inline std::string BypassPassword = "uno";


#ifdef _DEBUG
inline std::string WelcomeServerMessage = R"((DEV)Thank you for using the Brick Rigs Server Director(BRSD). Use /help to get started!)";
#else
inline std::string WelcomeServerMessage = R"(Thank you for using the Brick Rigs Server Director(BRSD). Use /help to get started!)";
#endif

inline std::string WelcomeClientMessage = R"(This server uses the Brick Rigs Server Director(BRSD). Use /help to get started!)";
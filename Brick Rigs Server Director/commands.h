#pragma once
#include <string>
#include <cstdarg>
#include <sstream>
#include "interpreter.h"
#include "messages.h"
#include "PlayerInfo.h"
#include "stringlist.h"

#define DECLARE_COMMAND(name) extern Command c_##name;
#define COMMAND(group, name, about, func) Command command_##name = Command(group, #name, about, false, true, func);
#define COMMAND_IMMUTABLE(group, name, about, func) Command command_##name = Command(group, #name, about, false, false, func);
#define COMMAND_ADMIN(group, name, about, func) Command command_##name = Command(group, #name, about, true, true, func);
#define COMMAND_ADMIN_IMMUTABLE(group, name, about, func) Command command_##name = Command(group, #name, about, true, false, func);


enum ECommandGroup
{
	Master,
	Main,
	Moderation,
	Movement,
	Enviroment,
	Weapons
};

struct Command
{
	inline static std::vector<Command*> command_registry = std::vector<Command*>();

	const std::string name;
	const std::string about;
	const ECommandGroup command_group;
	const bool admin_only;
	const bool togglable; //Whether the command can be turned off
	bool enabled;
	bool(*function)(PlayerInfo, std::vector<std::string> args);

	Command(ECommandGroup group, std::string Name, std::string About, bool admin, bool Toggleable, bool(*Function)(PlayerInfo, std::vector<std::string> args)) :
		name(Name),
		about(About),
		command_group(group),
		admin_only(admin),
		function(Function),
		togglable(Toggleable)
	{
		enabled = true;
		command_registry.push_back(this);
	}

	bool Execute(PlayerInfo info, std::vector<std::string> args)
	{
		if (!enabled) return false;
		return function(info, args);
	}

	const char* GetDisplayString()
	{
		std::stringstream ret;
		ret << name << " - " << about;
		return ret.str().c_str();
	}
};

//Master
DECLARE_COMMAND(info);
DECLARE_COMMAND(help);

//Main
DECLARE_COMMAND(pm);
DECLARE_COMMAND(silence);
DECLARE_COMMAND(unsilence);
DECLARE_COMMAND(block);
DECLARE_COMMAND(unblock);
DECLARE_COMMAND(pid);

//Moderation
DECLARE_COMMAND(on);
DECLARE_COMMAND(off);
DECLARE_COMMAND(mute);
DECLARE_COMMAND(unmute);
DECLARE_COMMAND(save);
DECLARE_COMMAND(load);

//Movement
DECLARE_COMMAND(fly);
DECLARE_COMMAND(walk);
DECLARE_COMMAND(tp);
DECLARE_COMMAND(ghost);

//Enviroment
DECLARE_COMMAND(night);
DECLARE_COMMAND(day);
DECLARE_COMMAND(rain);
DECLARE_COMMAND(sun);

//Weapons
DECLARE_COMMAND(ammotype);
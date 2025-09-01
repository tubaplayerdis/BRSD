#pragma once
#include <string>
#include <cstdarg>
#include <sstream>
#include "interpreter.h"
#include "messages.h"
#include "PlayerInfo.h"
#include "stringlist.h"

#define DECLARE_COMMAND(name) extern Command c_##name;
#define COMMAND(group, name, about, func, ...) Command command_##name = Command(group, #name, about, false, true, func, __VA_ARGS__);
#define COMMAND_IMMUTABLE(group, name, about, func, ...) Command command_##name = Command(group, #name, about, false, false, func, __VA_ARGS__);
#define COMMAND_ADMIN(group, name, about, func, ...) Command command_##name = Command(group, #name, about, true, true, func, __VA_ARGS__);
#define COMMAND_ADMIN_IMMUTABLE(group, name, about, func, ...) Command command_##name = Command(group, #name, about, true, false, func, __VA_ARGS__);

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
	const char* success_message;
	bool enabled;
	bool(*function)(PlayerInfo, std::vector<std::string> args);
	std::string display_string;

	Command(ECommandGroup group, std::string Name, std::string About, bool admin, bool Toggleable, bool(*Function)(PlayerInfo, std::vector<std::string> args), const char* SuccessMessage = nullptr) :
		name(Name),
		about(About),
		command_group(group),
		admin_only(admin),
		function(Function),
		togglable(Toggleable),
		success_message(SuccessMessage)
	{
		enabled = true;
		command_registry.push_back(this);
		display_string = Name;
		display_string.append(" - ");
		display_string.append(About);;
	}

	constexpr const char* get_command_string()
	{
		switch (command_group)
		{
			case Master:
				return "Master";
			case Main:
				return "Main";
			case Moderation:
				return "Moderation";
			case Movement:
				return "Movement";
			case Enviroment:
				return "Enviroment";
			case Weapons:
				return "Weapons";
			default:
				return "Unknown";
		}
	}

	static const char* get_command_string(ECommandGroup Group)
	{
		switch (Group)
		{
		case Master:
			return "Master";
		case Main:
			return "Main";
		case Moderation:
			return "Moderation";
		case Movement:
			return "Movement";
		case Enviroment:
			return "Enviroment";
		case Weapons:
			return "Weapons";
		default:
			return "Unknown";
		}
	}

	bool Execute(PlayerInfo info, std::vector<std::string> args)
	{
		if (!enabled) {
			std::string disabled_message = "\"";
			disabled_message.append(name);
			disabled_message.append("\" is currently disabled!");
			messages::sendUserSpecificMessageCommandFailed(info, disabled_message);
			return false;
		}
		bool ret = function(info, args);
		if (success_message && ret) messages::sendUserSpecificMessage(info, success_message);
		if (!ret) messages::sendUserSpecificMessageCommandFailed(info, name);
		return ret;
	}
};

//Master
DECLARE_COMMAND(info);
DECLARE_COMMAND(help);
DECLARE_COMMAND(uninject);

//Main
DECLARE_COMMAND(pm);
DECLARE_COMMAND(silence);
DECLARE_COMMAND(unsilence);
DECLARE_COMMAND(block);
DECLARE_COMMAND(unblock);
DECLARE_COMMAND(pid);
DECLARE_COMMAND(silenced);

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
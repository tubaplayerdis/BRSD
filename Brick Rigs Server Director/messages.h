/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     messages.h                                                  */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "PlayerInfo.h"
#include <SDK.hpp>

#define Message(info, message) messages::sendUserSpecificMessage(info, message)
#define ContextMessage(info, message, context, sender) messages::sendUserSpecificMessageWithContext(info,message,context,sender)
#define CommandFailedMessage(info, message) messages::sendUserSpecificMessageCommandFailed(info, message)
#define ShortArgsMessage(info, message, group) messages::ToFewArgs(info, message, group)
#define MessageHost(message) messages::sendMessageAdmin(message)

namespace messages
{
	/*
	* Send a specified user a message. messages send from user "BRSD" with default message context
	* info - Recipient info
	* message - message context
	*/
	void sendUserSpecificMessage(PlayerInfo info, std::string message);

	/*
	* Send a specified user a message with context and sender options.
	* info - Recipient info
	* message - message content
	* context - Brick Rigs's chat context
	* sender - custom sender name.
	*/
	void sendUserSpecificMessageWithContext(PlayerInfo info, std::string message, SDK::EChatContext context, const wchar_t* sender);

	/*
	* Send a specified user a message with context as the Command Failed! sender.
	* info - Recipient info
	* message - message content
	* context - Brick Rigs's chat context
	*/
	inline void sendUserSpecificMessageCommandFailed(PlayerInfo info, std::string message) { sendUserSpecificMessageWithContext(info, message, SDK::EChatContext::Global, L"Command Failed!"); }

	inline void ToFewArgs(PlayerInfo info, std::string commandstr, std::string cmdgroup) { sendUserSpecificMessageCommandFailed(info, std::string("Too few arguments used in command: ") + commandstr + std::string("\nUse /help ") + cmdgroup + std::string(" for more details!")); };

	/// <summary>
	/// Sends a message to the host as the administrator.
	/// </summary>
	/// <param name="message">The message to be sent to the host as an administrator.</param>
	void sendMessageAdmin(std::string message);
}
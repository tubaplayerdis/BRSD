#include "command_engine_menu.h"
#include "interpreter.h"
#include "messages.h"
#include "commands.h"
#include <imgui/imgui.h>

constexpr int COMMAND_ENGINE_MENU_WIDTH = 350;
constexpr int COMMAND_ENGINE_MENU_HEIGHT = 200;

bool checked = true;

void command_engine_menu_function()
{
	ImGui::SetNextWindowSize(ImVec2(COMMAND_ENGINE_MENU_WIDTH, COMMAND_ENGINE_MENU_HEIGHT), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f); // round window
	if (ImGui::Begin("Command Engine"))
	{
		if(ImGui::Checkbox("Enabled", &interpreter::is_enabled))
		{
			if (interpreter::is_enabled)
			{
				messages::sendMessageToAdmins("Chat commands were disabled");
			}
			else
			{
				messages::sendMessageToAdmins("Chat commands were enabled");
			}
		}

        ImGui::Separator();

        if (ImGui::TreeNodeEx("Commands", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); // red label
            ImGui::Text("Red Text = Admin Only");
            ImGui::PopStyleColor();

            if (ImGui::TreeNode("Master"))
            {
                for (Command* command : Command::command_registry)
                {
                    if (command->command_group != Master) continue;

                    if (command->admin_only)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    }

                    if (command->togglable)
                    {
                        ImGui::Checkbox(command->GetDisplayString(), &command->enabled);
                    }
                    else
                    {
                        ImGui::BeginDisabled();
                        ImGui::Checkbox(command->GetDisplayString(), &checked);
                        ImGui::EndDisabled();
                    }

                    if (command->admin_only)
                    {
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Main"))
            {
                for (Command* command : Command::command_registry)
                {
                    if (command->command_group != Main) continue;

                    if (command->admin_only)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    }

                    if (command->togglable)
                    {
                        ImGui::Checkbox(command->GetDisplayString(), &command->enabled);
                    }
                    else
                    {
                        ImGui::BeginDisabled();
                        ImGui::Checkbox(command->GetDisplayString(), &checked);
                        ImGui::EndDisabled();
                    }

                    if (command->admin_only)
                    {
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Moderation"))
            {
                for (Command* command : Command::command_registry)
                {
                    if (command->command_group != Moderation) continue;

                    if (command->admin_only)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    }

                    if (command->togglable)
                    {
                        ImGui::Checkbox(command->GetDisplayString(), &command->enabled);
                    }
                    else
                    {
                        ImGui::BeginDisabled();
                        ImGui::Checkbox(command->GetDisplayString(), &checked);
                        ImGui::EndDisabled();
                    }

                    if (command->admin_only)
                    {
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Movement"))
            {
                for (Command* command : Command::command_registry)
                {
                    if (command->command_group != Movement) continue;

                    if (command->admin_only)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    }

                    if (command->togglable)
                    {
                        ImGui::Checkbox(command->GetDisplayString(), &command->enabled);
                    }
                    else
                    {
                        ImGui::BeginDisabled();
                        ImGui::Checkbox(command->GetDisplayString(), &checked);
                        ImGui::EndDisabled();
                    }

                    if (command->admin_only)
                    {
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Enviroment"))
            {
                for (Command* command : Command::command_registry)
                {
                    if (command->command_group != Enviroment) continue;

                    if (command->admin_only)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    }

                    if (command->togglable)
                    {
                        ImGui::Checkbox(command->GetDisplayString(), &command->enabled);
                    }
                    else
                    {
                        ImGui::BeginDisabled();
                        ImGui::Checkbox(command->GetDisplayString(), &checked);
                        ImGui::EndDisabled();
                    }

                    if (command->admin_only)
                    {
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Weapons"))
            {
                for (Command* command : Command::command_registry)
                {
                    if (command->command_group != Weapons) continue;

                    if (command->admin_only)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    }

                    if (command->togglable)
                    {
                        ImGui::Checkbox(command->GetDisplayString(), &command->enabled);
                    }
                    else
                    {
                        ImGui::BeginDisabled();
                        ImGui::Checkbox(command->GetDisplayString(), &checked);
                        ImGui::EndDisabled();
                    }

                    if (command->admin_only)
                    {
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::TreePop();
            }

            ImGui::TreePop(); // Pop Root
        }
	}
	ImGui::End();
    ImGui::PopStyleVar();
}

DEFINE_GUI_MENU(menus::command_engine_menu, command_engine_menu_function);
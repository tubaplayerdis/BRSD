#include "../../Include/GUI/menus.h"
#include <BR-SDK.hpp>
#include "../../Include/Global/global.h"
#include "imgui/imgui.h"
#include "../../Include/Modules/Server Director/moderation.h"
#include "../../Include/Utils/offsets.h"
#include "../../Include/Modules/Command Engine/commands.h"

constexpr int MODERATION_MENU_WIDTH = 400;
constexpr int MODERATION_MENU_HEIGHT = 250;

#define ADMIN_CONTROLLER() GetPlayerInfoFromController(GetBrickPlayerController())

std::vector<SDK::ABrickPlayerState*> player_states = std::vector<SDK::ABrickPlayerState*>();

void custom_toggle_moderation(bool toggle)
{
    if (toggle) //going to be turned on
    {
        player_states.clear();

        if (GetBrickGameState() == nullptr || GetBrickGameState()->GameModeClass.Get() == nullptr ||GetBrickGameState()->GameModeClass.Get()->IsA(SDK::AMenuGameMode::StaticClass()) || GetBrickGameState()->PlayerArray.Num() == 0) return;

        for (SDK::APlayerState* state : GetBrickGameState()->PlayerArray)
        {
            if (!state->IsA(SDK::ABrickPlayerState::StaticClass())) continue;
            player_states.push_back(static_cast<SDK::ABrickPlayerState*>(state));
        }
    }
}

void moderation_menu_function()
{
    using namespace ImGui;

	ImGui::SetNextWindowSize(ImVec2(MODERATION_MENU_WIDTH, MODERATION_MENU_HEIGHT), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f); // round window
	if (ImGui::Begin("Moderation"))
	{
        if (player_states.size() == 0)
        {
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        static int selected = 0;
        {
            ImGui::BeginChild("players", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
            for (int i = 0; i < player_states.size(); i++)
            {
                char label[128];
                sprintf_s(label, "%s", player_states[i]->GetPlayerName().ToString().c_str());
                if (ImGui::Selectable(label, selected == i)) selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        SDK::FString input_string = SDK::FString();
        SDK::FString* net_id = CallGameFunction<SDK::FString*, SDK::FUniqueNetIdRepl*, SDK::FString*>(FUniqueNetIdWrapper_ToString, &player_states[selected]->UniqueId, &input_string);

        ImGui::BeginGroup();
        ImGui::Text("%s: ID: %d, SteamID: %s", player_states[selected]->GetPlayerName().ToString().c_str(),player_states[selected]->PlayerId, input_string.ToString().c_str());
        ImGui::Separator();

        ImGui::BeginChild("items_pane", ImVec2(150, 0), true); // Max Width, Full Height

        SDK::ABrickPlayerState* current = player_states[selected];

        {
            PlayerInfo info = PlayerInfo(current->GetPlayerName().ToString().c_str());
            bool is_host = current->Ping == 0;
            bool is_admin = current->bIsAdmin;
            bool is_team_leader = current->bIsTeamLeader;
            bool is_muted = moderation::isPlayerMuted(info);
            float money = current->Money;
            int kills = current->Kills;
            int deaths = current->Deaths;

            ImGui::BeginDisabled();
            ImGui::Checkbox("Is Host", &is_host);
            ImGui::EndDisabled();

            if(ImGui::Checkbox("Is Admin", &is_admin))
            {
                if (current != GetBrickPlayerState())
                {
                    current->SetIsAdmin(is_admin);
                }
            }
            
            if (ImGui::Checkbox("Is Team Leader", &is_team_leader))
            {
                if (current != GetBrickPlayerState())
                {
                    current->SetIsTeamLeader(is_admin);
                }
            }

            if (ImGui::Checkbox("Is Muted", &is_muted))
            {
                if (is_muted)
                {
                    moderation::AddMutedPlayer(info);
                }
                else
                {
                    moderation::RemoveMutedPlayer(info);
                }
            }

            Text("Kills: % d", kills);
            Text("Deaths: % d", deaths);
        }

        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("action_pane", ImVec2(0,0), true);

        if (ImGui::Button("Kill")) 
        {
            Command* command = Command::get_command_by_name("kill");
            if (command)
            {
                command->Execute(ADMIN_CONTROLLER(), std::vector<std::string>{player_states[selected]->GetPlayerName().ToString()});
            }
        }
        if (ImGui::Button("Revive")) 
        {
            Command* command = Command::get_command_by_name("revive");
            if (command)
            {
                command->Execute(ADMIN_CONTROLLER(), std::vector<std::string>{player_states[selected]->GetPlayerName().ToString()});
            }
        }
        if (ImGui::Button("Destroy Vehicle")) 
        {
            Command* command = Command::get_command_by_name("vehdel");
            if (command)
            {
                command->Execute(ADMIN_CONTROLLER(), std::vector<std::string>{player_states[selected]->GetPlayerName().ToString()});
            }
        }
        if (ImGui::Button("Empty Inventory")) 
        {
            Command* command = Command::get_command_by_name("empty");
            if (command)
            {
                command->Execute(ADMIN_CONTROLLER(), std::vector<std::string>{player_states[selected]->GetPlayerName().ToString()});
            }
        }
        if (ImGui::Button("Teleport To Player")) 
        {
            GetBrickCharacter()->K2_TeleportTo(current->K2_GetActorLocation(), current->K2_GetActorRotation());
        }
        if (ImGui::Button("Teleport Player To You")) 
        {
            current->K2_TeleportTo(GetBrickCharacter()->K2_GetActorLocation(), current->K2_GetActorRotation());
        }

        ImGui::EndChild();

        ImGui::EndGroup();
	}
	ImGui::End();
    ImGui::PopStyleVar();
}

DEFINE_GUI_MENU(menus::moderation_menu, moderation_menu_function, custom_toggle_moderation)
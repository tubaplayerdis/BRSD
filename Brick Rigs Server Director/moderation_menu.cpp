#include "menus.h"
#include <BR-SDK.hpp>
#include "global.h"
#include "imgui/imgui.h"
#include "moderation.h"

constexpr int MODERATION_MENU_WIDTH = 400;
constexpr int MODERATION_MENU_HEIGHT = 250;

std::vector<SDK::ABrickPlayerState*> player_states = std::vector<SDK::ABrickPlayerState*>();

void custom_toggle_moderation(bool toggle)
{
    if (toggle) //going to be turned on
    {
        player_states.clear();

        for (SDK::APlayerState* state : GetBrickGameState()->PlayerArray)
        {
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

        ImGui::BeginGroup();
        ImGui::BeginChild("players view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        SDK::FString input_string = SDK::FString();
        SDK::FString* net_id = CallGameFunction<SDK::FString*, SDK::FUniqueNetIdRepl*, SDK::FString*>(BASE + 0x0815AF0, &player_states[selected]->UniqueId, &input_string);
        ImGui::Text("%s: %s", player_states[selected]->GetPlayerName().ToString().c_str(), input_string.ToString().c_str());
        ImGui::Separator();

        SDK::ABrickPlayerState* current = player_states[selected];

        {
            PlayerInfo info = PlayerInfo(current->GetPlayerName().ToString().c_str());
            bool is_host = current->Ping == 0;
            bool is_admin = current->bIsAdmin;
            bool is_team_leader = current->bIsTeamLeader;
            bool is_muted = moderation::isPlayerMuted(info);
            int player_id = current->PlayerId;
            float money = current->Money;
            int kills = current->Kills;
            int deaths = current->Deaths;

            ImGui::BeginDisabled();
            ImGui::Checkbox("Is Host", &is_host);
            ImGui::EndDisabled();

            ImGui::SameLine();

            if(ImGui::Checkbox("Is Admin", &is_admin))
            {
                if (current != GetBrickPlayerState())
                {
                    current->SetIsAdmin(is_admin);
                }
            }

            ImGui::SameLine();
            
            if (ImGui::Checkbox("Is Team Leader", &is_team_leader))
            {
                if (current != GetBrickPlayerState())
                {
                    current->SetIsTeamLeader(is_admin);
                }
            }

            ImGui::SameLine();

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

            BeginDisabled();

            InputInt("Player ID", &player_id);
            SameLine();
            InputInt("Kills", &kills);
            SameLine();
            InputInt("Deaths", &deaths);

            EndDisabled();
        }


        ImGui::EndChild();
        if (ImGui::Button("Kill")) 
        {
            SDK::ABrickPlayerController* controller = static_cast<SDK::ABrickPlayerController*>(current->Owner);
            controller->KillCharacter();
        }
        ImGui::SameLine();
        if (ImGui::Button("Revive")) 
        {
            current->SetIsAlive(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Destroy Vehicle")) 
        {
            SDK::ABrickPlayerController* controller = static_cast<SDK::ABrickPlayerController*>(current->Owner);
            std::cout << controller->GetName() << std::endl;
            if (controller && controller->GetPlayerVehicle())
            {
                SDK::ABrickVehicle* vehicle = controller->GetPlayerVehicle();
                static_cast<SDK::ABrickCharacter*>(controller->GetPlayerCharacter())->ForceEjectFromVehicle();
                vehicle->ScrapVehicle();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Empty Inventory")) 
        {
            static_cast<SDK::ABrickPlayerController*>(current->Owner)->AccessedInventory->EmptyInventory(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Teleport To Player")) 
        {
            GetBrickCharacter()->K2_TeleportTo(current->K2_GetActorLocation(), current->K2_GetActorRotation());
        }
        ImGui::SameLine();
        if (ImGui::Button("Teleport Player To You")) 
        {
            current->K2_TeleportTo(GetBrickCharacter()->K2_GetActorLocation(), current->K2_GetActorRotation());
        }
        ImGui::EndGroup();
	}
	ImGui::End();
    ImGui::PopStyleVar();
}

DEFINE_GUI_MENU(menus::moderation_menu, moderation_menu_function, custom_toggle_moderation)
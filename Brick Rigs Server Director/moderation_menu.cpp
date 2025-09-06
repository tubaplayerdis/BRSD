#include "menus.h"
#include <BR-SDK.hpp>
#include "global.h"
#include "imgui/imgui.h"

constexpr int MODERATION_MENU_WIDTH = 350;
constexpr int MODERATION_MENU_HEIGHT = 200;

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
        ImGui::Text("%s", player_states[selected]->GetPlayerName().ToString().c_str());
        ImGui::Separator();

        SDK::ABrickPlayerState* current = player_states[selected];

        {
            SDK::FString input_string = SDK::FString();
            SDK::FString* net_id = CallGameFunction<SDK::FString*, SDK::FUniqueNetIdRepl*, SDK::FString*>(BASE + 0x0815AF0, &current->UniqueId, &input_string);
            ImGui::Text("SteamID: %s", input_string.ToString().c_str());

            bool is_host = current->Ping == 0;
            bool is_admin = current->bIsAdmin;
            bool is_team_leader = current->bIsTeamLeader;
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
        }


        ImGui::EndChild();
        if (ImGui::Button("Kill")) 
        {
            current->SetIsAlive(false);
        }
        ImGui::SameLine();
        if (ImGui::Button("Revive")) 
        {
            current->SetIsAlive(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Destroy Vehicle")) 
        {
            SDK::ABP_BrickVehicle_C* vehicle = static_cast<SDK::ABP_BrickVehicle_C*>(current->Owner);
            if (vehicle)
            {
                static_cast<SDK::ABP_BrickCharacter_C*>(current->InactiveCharacter)->ForceEjectFromVehicle();
                vehicle->ScrapVehicle();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Empty Inventory")) 
        {
            static_cast<SDK::ABP_BrickCharacter_C*>(current->InactiveCharacter)->InventoryComponent->EmptyInventory(true);
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
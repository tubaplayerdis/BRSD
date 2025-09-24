#include "../../Include/GUI/menus.h"
#include <imgui/imgui.h>

#ifdef _DEBUG
const char* BuildMode = "Debug";
#else
const char* BuildMode = "Release";
#endif // _DEBUG

constexpr int MAIN_MENU_WIDTH = 350;
constexpr int MAIN_MENU_HEIGHT = 200;

void main_menu_function()
{
	ImGui::SetNextWindowSize(ImVec2(MAIN_MENU_WIDTH, MAIN_MENU_HEIGHT), ImGuiCond_Appearing);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f); // round window
	if (ImGui::Begin("Brick Rigs Server Director"))
	{
		if (ImGui::BeginTabBar("ShortcutsMenuBar")) {

			if (ImGui::TabItemButton("Moderation", ImGuiTabItemFlags_None)) {
				menus::moderation_menu->toggle();
			}

			if (ImGui::TabItemButton("Blacklist", ImGuiTabItemFlags_None)) {
				
			}

			if (ImGui::TabItemButton("Saver", ImGuiTabItemFlags_None)) {
				
			}

			if (ImGui::TabItemButton("Command Engine", ImGuiTabItemFlags_None)) {
				menus::command_engine_menu->toggle();
			}

			ImGui::EndTabBar();
		}


		ImGui::Text("Version: 0.0.1");
		ImGui::Text("Build Mode: %s", BuildMode);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}


DEFINE_GUI_MENU(menus::main_menu, main_menu_function);
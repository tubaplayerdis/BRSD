#pragma once
#include <memory>
#include "gui_manager.h"
#include <imgui/imgui.h>

GUI_MENU(main_menu, []() -> void
{
	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Appearing);
	if (ImGui::Begin("Brick Rigs Server Director"))
	{
		ImGui::Text("This is the first window!");
		ImGui::End();
	}
});
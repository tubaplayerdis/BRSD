#pragma once
#include <memory>
#include "gui_manager.h"
#include <imgui/imgui.h>

GUI_MENU(main_menu, []() -> void
{
	ImGui::Begin("Window A");
	ImGui::Text("This is the first window!");
	ImGui::End();
});
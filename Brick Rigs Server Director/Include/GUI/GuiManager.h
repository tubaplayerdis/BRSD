#pragma once
#include <memory>
#include <atomic>

#define DECLARE_GUI_MENU(name) extern std::shared_ptr<gui_menu> name;
#define DEFINE_GUI_MENU(name, lamb, ...) std::shared_ptr<gui_menu> name = std::make_shared<gui_menu>(lamb, __VA_ARGS__);

struct gui_menu;

class GuiManager
{
	friend class std::unique_ptr<GuiManager>;

	GuiManager();

public:

	static GuiManager* get();
	static void shutdown();

	void hide_all();
	bool are_all_hidden();
	void display_all_previous();

	void add_menu(std::shared_ptr<gui_menu> menu);
	void remove_menu(std::shared_ptr<gui_menu> menu);

	//Thread safe function to change menu visibility. these are not actually thread safe but work way better.
	void set_menu_visibility(std::shared_ptr<gui_menu> menu, bool visibility);
	void toggle_menu_visibility(std::shared_ptr<gui_menu> menu);
};

struct gui_menu : public std::enable_shared_from_this<gui_menu>
{
	void(*menu)();
	void(*custom_toggle)(bool);
	std::atomic<bool> is_visible;

	gui_menu(void(*in_menu)(), void(*in_custom_toggle)(bool) = nullptr) : is_visible(false), menu(in_menu), custom_toggle(in_custom_toggle) {}

	void toggle()
	{
		GuiManager::get()->toggle_menu_visibility(shared_from_this());
	}

	void display()
	{
		GuiManager::get()->set_menu_visibility(shared_from_this(), true);
	}

	void hide()
	{
		GuiManager::get()->set_menu_visibility(shared_from_this(), false);
	}
};
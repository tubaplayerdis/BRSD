#pragma once
#include <memory>

#define DECLARE_GUI_MENU(name) extern std::unique_ptr<gui_menu> name;
#define DEFINE_GUI_MENU(name, lamb) std::unique_ptr<gui_menu> name = std::make_unique<gui_menu>(lamb);

struct gui_menu;

class gui_manager
{
	friend class std::unique_ptr<gui_manager>;

	gui_manager();

public:

	static gui_manager* get();
	static void shutdown();

	void hide_all();
	bool are_all_hidden();
	void display_all_previous();

	void add_menu(gui_menu* menu);
	void remove_menu(gui_menu* menu);
};

struct gui_menu
{
	void(*menu)();
	bool is_visible;

	gui_menu(void(*in_menu)())
	{
		menu = in_menu;
		is_visible = false;
	}

	void toggle()
	{
		gui_manager::get()->add_menu(this);
		is_visible = !is_visible;
	}

	void display()
	{
		is_visible = true;
		gui_manager::get()->add_menu(this);
	}

	void hide()
	{
		is_visible = false;
	}
};
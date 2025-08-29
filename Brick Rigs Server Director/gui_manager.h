#pragma once
#include <memory>

#define GUI_MENU(name, lamb) inline std::unique_ptr<gui_menu> name = std::make_unique<gui_menu>(lamb);
#define GUI_MENU_NO_INLINE(name, lamb) inline std::unique_ptr<gui_menu> name = std::make_unique<gui_menu>(lamb);

struct gui_menu;

class gui_manager
{
public:
	gui_manager();
	~gui_manager();

	static gui_manager* get();

	void add_menu(gui_menu* menu);
	void remove_menu(gui_menu* menu);
};

struct gui_menu
{
	void(*menu)();

	gui_menu(void(*in_menu)())
	{
		menu = in_menu;
	}

	void display()
	{
		gui_manager::get()->add_menu(this);
	}

	void hide()
	{
		gui_manager::get()->remove_menu(this);
	}
};
#pragma once
#include <SDL/SDL.h>
#include "Menu.h"


class Manager
{
public:
	Manager(int windowSizeX, int windowSizeY);
    ~Manager();
	// menu function to run the menu
	void menu();

protected:
	// Set up stuff needed for SDL and create pointer to menu class
	int m_windowSizeX;
	int m_windowSizeY;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	Menu* m_menu;
};


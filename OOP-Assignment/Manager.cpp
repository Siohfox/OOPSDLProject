#include "Manager.h"

// Constrtuctor 
Manager::Manager(int windowSizeX, int windowSizeY)
{
    this->m_windowSizeX = windowSizeX;
    this->m_windowSizeY = windowSizeY;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Check if VS is linked to the TTF lib file and initialise it
    if (TTF_Init() < 0)
    {
        SDL_Log("Unable to initialise SDL_TTF: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create Window
    m_window = SDL_CreateWindow("Asteroid Shooter Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSizeX, windowSizeY, SDL_WINDOW_SHOWN);

    if (m_window == nullptr)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Instantiate a menu object and pass in the new renderer
    m_menu = new Menu(m_renderer);
}

// Destructor for manager, destroys all sdl stuff and the menu instance
Manager::~Manager()
{
    delete(m_menu);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}

// Call the menu function
void Manager::menu()
{
    m_menu->MenuLoop();
}
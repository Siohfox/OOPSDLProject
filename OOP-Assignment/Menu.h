#ifndef MENU_H
#define MENU_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include "Game.h"

class Menu
{
public:

    // Constructor.
    Menu(SDL_Renderer* renderer)
    {
        m_menuState = true;
        m_debugMode = false;

        // Read highscore from the save file and assign it to highscore variable
        m_highScore = readHighscore();

        // Set this classes renderer to the one passed in
        this->m_renderer = renderer;

        // Create the background
        m_backgroundRect = { 0,0,640,480 };
        m_backgroundImage = SDL_LoadBMP("Images/spacebackground.bmp");
        if (!m_backgroundImage)
        {
            throw std::exception();
        }
        m_backgroundTexture = SDL_CreateTextureFromSurface(m_renderer, m_backgroundImage);
        SDL_FreeSurface(m_backgroundImage);

        // Create the start button text image
        m_startButtonRect = { 120,240,400,50 };
        m_startButtonImage = SDL_LoadBMP("Images/MenuPlay.bmp");
        if (!m_startButtonImage)
        {
            throw std::exception();
        }
        m_startButtonTexture = SDL_CreateTextureFromSurface(renderer, m_startButtonImage);
        SDL_FreeSurface(m_startButtonImage);

        m_quitButtonRect = { 220,320,200,30 };
        m_quitButtonImage = SDL_LoadBMP("Images/MenuQuit.bmp");
        if (!m_quitButtonImage)
        {
            throw std::exception();
        }
        m_quitButtonTexture = SDL_CreateTextureFromSurface(renderer, m_quitButtonImage);
        SDL_FreeSurface(m_quitButtonImage);

        m_font = TTF_OpenFont("Fonts/Dream.ttf", 20);
        m_highScoreColour = { 255, 255, 255, 255 };
        m_highScoreStr = "highscore: " + std::to_string(m_highScore);
        m_highScoreSurface = TTF_RenderText_Solid(m_font, m_highScoreStr.c_str(), m_highScoreColour);
        m_highScoreImage = SDL_CreateTextureFromSurface(renderer, m_highScoreSurface);
        m_highScoreRect = { 380,10,220,25 };
        SDL_FreeSurface(m_highScoreSurface);

        // Create an audio channel
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        // Load the background music
        m_backgroundMusic = Mix_LoadMUS("Audio/SpaceMusic.wav");
    }
    ~Menu()
    {
        SDL_DestroyTexture(m_backgroundTexture);
        SDL_DestroyTexture(m_startButtonTexture);
        SDL_DestroyTexture(m_quitButtonTexture);
    }

    void MenuLoop();
    void UpdateHighScoreText();
    void PollEvents();
    void Render();
    int readHighscore();
    void setHighscore(int _highscore);

protected:
    
    bool m_menuState;
    int m_highScore;
    bool m_debugMode;

    SDL_Renderer* m_renderer;
    SDL_Rect m_backgroundRect;
    SDL_Surface* m_backgroundImage;
    SDL_Texture* m_backgroundTexture;

    SDL_Rect m_startButtonRect;
    SDL_Surface* m_startButtonImage;
    SDL_Texture* m_startButtonTexture;

    SDL_Rect m_quitButtonRect;
    SDL_Surface* m_quitButtonImage;
    SDL_Texture* m_quitButtonTexture;

    TTF_Font* m_font;
    SDL_Color m_highScoreColour;
    SDL_Surface* m_highScoreSurface;
    std::string m_highScoreStr;
    SDL_Texture* m_highScoreImage;
    SDL_Rect m_highScoreRect;
   
    Mix_Music* m_backgroundMusic;
    // Assign the music from your project folder:

};
#endif


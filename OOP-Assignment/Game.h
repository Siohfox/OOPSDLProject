#ifndef GAME_H
#define GAME_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include "Player.h"
#include "Asteroid.h"
#include "Bullet.h"

class Game
{
public:
    // Construct the game and intialise the player ship
    Game(SDL_Renderer* renderer, int _highScore) : m_ship("PlayerShip1", 30, 30, renderer) 
    {
        // Create randomness by default
        srand(static_cast<unsigned int>(time(0)));

        // Assign renderer to member variable so all objects of the game can use it
        this->m_renderer = renderer;

        m_frameRate = 1.0f / 60.0f;
        m_lastTime = 0.0f;
        m_currentTime = (float)SDL_GetTicks();
        m_score = 0;
        m_backgroundRect = { 0,0,640,480 };
        m_backgroundRectTwo = { 640,0,640,480 };
        m_backgroundImage = SDL_LoadBMP("Images/spacebackground.bmp");
        m_highScore = _highScore;
        if (!m_backgroundImage)
        {
            throw std::exception();
        }
        m_backgroundTexture = SDL_CreateTextureFromSurface(renderer, m_backgroundImage);
        SDL_FreeSurface(m_backgroundImage);
        
        m_waveCount = 1;
        m_waveLimit = 15;

        m_font = TTF_OpenFont("Fonts/Dream.ttf", 20);

        // INTIALIZE SCORE TEXT COMPONENTS
        m_scoreColour = { 255, 255, 255, 255 };
        m_scoreStr = "score: " + std::to_string(m_score);
        m_scoreSurface = TTF_RenderText_Solid(m_font, m_scoreStr.c_str(), m_scoreColour);
        m_scoreTexture = SDL_CreateTextureFromSurface(renderer, m_scoreSurface);
        // Free memory of the surface:
        SDL_FreeSurface(m_scoreSurface);
        m_scoreRect = { 10,10,220,25 };

        // INTIALIZE WIN TEXT COMPONENTS
        m_winColour = { 0, 255, 0, 255 };
        m_winStr = "you win!";
        m_winSurface = TTF_RenderText_Solid(m_font, m_winStr.c_str(), m_winColour);
        m_winTexture = SDL_CreateTextureFromSurface(renderer, m_winSurface);
        m_winRect = { 220,240,220,25 };
        SDL_FreeSurface(m_winSurface);

        // INTIALIZE LOSE TEXT COMPONENTS
        m_loseColour = { 255, 0, 0, 255 };
        m_loseStr = "you lose";
        m_loseSurface = TTF_RenderText_Solid(m_font, m_loseStr.c_str(), m_loseColour);
        m_loseTexture = SDL_CreateTextureFromSurface(renderer, m_loseSurface);
        m_loseRect = { 220,240,220,25 };
        SDL_FreeSurface(m_loseSurface);

        // INTIALIZE HIGHSCORE TEXT COMPONENTS
        m_highScoreColour = { 255, 255, 255, 255 };
        m_highScoreStr = "highscore: " + std::to_string(m_highScore);
        m_highScoreSurface = TTF_RenderText_Solid(m_font, m_highScoreStr.c_str(), m_highScoreColour);
        m_highScoreTexture = SDL_CreateTextureFromSurface(renderer, m_highScoreSurface);
        m_highScoreRect = { 400,10,220,25 };
        SDL_FreeSurface(m_highScoreSurface);

        // INTIALIZE HEALTH TEXT COMPONENTS
        m_healthColour = { 255, 0, 0, 255 };
        m_healthStr = "health: " + std::to_string(m_ship.getHitPoints());
        m_healthSurface = TTF_RenderText_Solid(m_font, m_healthStr.c_str(), m_healthColour);
        m_healthTexture = SDL_CreateTextureFromSurface(renderer, m_healthSurface);
        m_healthRect = { 400,50,220,25 };
        SDL_FreeSurface(m_healthSurface);

        // INTIALIZE WAVE TEXT COMPONENTS
        m_waveColour = { 255, 255, 0, 255 };
        m_waveStr = "wave: " + std::to_string(m_waveCount);
        m_waveSurface = TTF_RenderText_Solid(m_font, m_waveStr.c_str(), m_waveColour);
        m_waveTexture = SDL_CreateTextureFromSurface(renderer, m_waveSurface);
        m_waveRect = { 10,440,110,25 };
        SDL_FreeSurface(m_waveSurface);

        // Create initial amount of enemies to spawn with and set enemy counter to it
        m_numberOfEnemiesToSpawn = 5;
        m_numberOfEnemiesAlive = m_numberOfEnemiesToSpawn;
    }

    ~Game()
    {
        // Clean up all outstanding textures
        SDL_DestroyTexture(m_scoreTexture);
        SDL_DestroyTexture(m_backgroundTexture);
        SDL_DestroyTexture(m_winTexture);
        SDL_DestroyTexture(m_loseTexture);
        SDL_DestroyTexture(m_highScoreTexture);
        SDL_DestroyTexture(m_healthTexture);
        SDL_DestroyTexture(m_waveTexture);

        // Destructor should be called when going back to menu
    }

    // Function to run the game
    int Run();


private:
    // This is a variable which stores the renderer, so we can pass it down to everything in the game
    SDL_Renderer* m_renderer;

    // Member variables
    float m_frameRate, m_lastTime, m_currentTime;
    int m_score, m_highScore, m_numberOfEnemiesToSpawn, m_numberOfEnemiesAlive, m_waveCount, m_waveLimit;
    bool m_running = false;


    // ----- TEXT STUFF -----
    TTF_Font* m_font;

    // Score text
    SDL_Color m_scoreColour;
    SDL_Surface* m_scoreSurface;
    std::string m_scoreStr;
    SDL_Texture* m_scoreTexture;
    SDL_Rect m_scoreRect;

    // High score text
    SDL_Color m_highScoreColour;
    SDL_Surface* m_highScoreSurface;
    std::string m_highScoreStr;
    SDL_Texture* m_highScoreTexture;
    SDL_Rect m_highScoreRect;

    // Win text
    SDL_Color m_winColour;
    SDL_Surface* m_winSurface;
    std::string m_winStr;
    SDL_Texture* m_winTexture;
    SDL_Rect m_winRect;

    // Lose text
    SDL_Color m_loseColour;
    SDL_Surface* m_loseSurface;
    std::string m_loseStr;
    SDL_Texture* m_loseTexture;
    SDL_Rect m_loseRect;

    // Health text
    SDL_Color m_healthColour;
    SDL_Surface* m_healthSurface;
    std::string m_healthStr;
    SDL_Texture* m_healthTexture;
    SDL_Rect m_healthRect;

    // Wave text
    SDL_Color m_waveColour;
    SDL_Surface* m_waveSurface;
    std::string m_waveStr;
    SDL_Texture* m_waveTexture;
    SDL_Rect m_waveRect;
    // ----- TEXT STUFF -----


    // Background image
    SDL_Rect m_backgroundRect, m_backgroundRectTwo;
    SDL_Surface* m_backgroundImage;
    SDL_Texture* m_backgroundTexture;
   
    // Game-oriented functions
    void GameLoop();
    void Render();
    void Update(float deltaTime);
    void PollEvents();
    void SpawnEnemies(int numberOfEnemies);
    bool OutOfPlay(SDL_Rect &bullet);
    void UpdateWaves();
    void UpdateScores();
    void HandleCollisions();
    void Quit();
    float DeltaTime();
    double FindDirectionToMouse();
    void ScrollBackground();

    // Instance of the playership and a vector array of asteroids stored here
    PlayerShip m_ship;
    std::vector<Asteroid*> asteroids;
};
#endif
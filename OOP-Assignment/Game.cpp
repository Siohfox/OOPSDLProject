#include <iostream>
#include <string>
#include <ctime>
#include "Game.h"

int Game::Run()
{
    // When run is called, set alive and running to true
    m_running = true;

    // start the game loop
    GameLoop();

    return m_highScore;
}

void Game::Quit()
{
    // Quit us responsible for cleaning up any outstanding objects needing to be manually deleted and counters to be reset
    // After it's cleaned up, it exits the game

    // Delete all asteroids
            // -- It's important to note that the player doesn't need to be deleted here
            // -- because it will be destroyed with the game object
    for (size_t i = 0; i < asteroids.size(); i++)
    {
        delete asteroids[i];
        asteroids[i] = nullptr;
    }
    // Erase all asteroids which have been set to nullptr
    asteroids.erase(std::remove(asteroids.begin(), asteroids.end(), nullptr), asteroids.end());

    // Reset score
    m_score = 0;

    // Reset wavecount
    //m_waveCount = 1;
    m_numberOfEnemiesToSpawn = 5;
    std::cout << "Wavecount is now "<< m_waveCount << std::endl;

    // Reset PlayerHealth
    m_ship.setHP(30);

    // Set running to false; breaking the game loop and exiting to menu
    m_running = false;
}

void Game::GameLoop()
{ 
    // create deltaTime and apply it to the set framerate
    float deltaTime = m_frameRate;

    // Start initial wave by spawning 5 enemies
    SpawnEnemies(5);
    // Update the scores so they show their values properly before starting
    UpdateScores();

    while (m_running)
    {
        // While game is running do these four tasks in order

        // 1. Update positions and check for collisions, passing in deltatime to handle frames
        Update(deltaTime);

        // 2. Update and increase Waves when if all enemies are dead
        UpdateWaves();
        
        // 3. Render to screen
        Render();

        // 4. Check for any input to quit
        PollEvents();

        // Update the deltatime
        deltaTime = DeltaTime();
    }
}

void Game::PollEvents()
{
    // Create var for event
    SDL_Event event;

    // while there is an event happening like a key press, do this:
    while (SDL_PollEvent(&event))
    {
        // If the event is quit, then running is false and it back to menu.
        if (event.type == SDL_QUIT)
        {
            Quit();
        }
    }
}

void Game::Update(float deltaTime)
{
    
    ScrollBackground();

    // Call ships overriden update function and pass in delta time
    m_ship.Update(deltaTime);

    // For each asteroid, update their individual positions using their overriden update functions
    for (size_t i = 0; i < asteroids.size(); i++)
    {
        asteroids[i]->Update(deltaTime);
        //std::cout << asteroids[i]->getBody().x << asteroids[i]->getBody().y << std::endl;
    }
   
    // Call the ships fire function, which will test if the ship has fired or not and then fire if space is pressed
    m_ship.Fire(deltaTime, m_renderer);
    
    /*
        For every instantiated bullet, update their positions.
        Since i = 0 and it's checking if 0 is less than the vector size of bullets,
        if no bullets have been fired then the vector size is 0, which means it will skip the check altogether
        if the ship has not fired at all or if there are no bullets still on screen
    */
    for (size_t i = 0; i < m_ship.getBullet().size(); i++)
    {
        // Get the bullet fired by the ship per instance of the array, then update their movement, passing in delta time and the ship gameobject
        m_ship.getBullet()[i]->Update(deltaTime);
        
        // Test each bullet to see if they've gone out of the window, if they have, delete them from the vector

        // Create temporary rect to test with and assign it to the bullet's position rect
        SDL_Rect test = m_ship.getBullet()[i]->getBody().toRect();

        // Use this  temp rect to test if the bullet is out of the window
        if (OutOfPlay(test))
        {
            delete m_ship.getBullet()[i];
            
            m_ship.getBullet()[i] = nullptr;
        }
    }

    // rip any nullptr out of the bullets vector
    std::vector<Bullet*>::iterator itr;
    itr = m_ship.getBullet().begin();
    m_ship.getBullet().erase(std::remove(itr, m_ship.getBullet().end(), nullptr), m_ship.getBullet().end());

    HandleCollisions();

    m_numberOfEnemiesAlive = asteroids.size();

    if (m_ship.getHitPoints() <= 0)
    {
        Render();
        SDL_Delay(5000);
        Quit();
    }
}

void Game::Render()
{
    // Clear screen
    SDL_RenderClear(m_renderer);
    
    // Set background
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    // Set background image
    SDL_RenderCopyEx(m_renderer, this->m_backgroundTexture, nullptr, &this->m_backgroundRect, 0, nullptr, SDL_FLIP_NONE);
    SDL_RenderCopyEx(m_renderer, this->m_backgroundTexture, nullptr, &this->m_backgroundRectTwo, 0, nullptr, SDL_FLIP_HORIZONTAL);


    // Render each meteor in vector array
    for (size_t i = 0; i < asteroids.size(); i++)
    {
        SDL_Rect asteroid = asteroids[i]->getBody().toRect();
        SDL_RenderCopyEx(m_renderer, asteroids[i]->getTexture(), nullptr, &asteroid, 0, nullptr, SDL_FLIP_NONE);
    }
    // Render each bullet in vector array
    for (size_t i = 0; i < m_ship.getBullet().size(); i++)
    {
        SDL_Rect shipBullet = m_ship.getBullet()[i]->getBody().toRect();
        SDL_RenderCopyEx(m_renderer, m_ship.getBullet()[i]->getTexture() , nullptr, &shipBullet, m_ship.getBullet()[i]->getRotation() + this->FindDirectionToMouse() - 90.f, nullptr, SDL_FLIP_NONE);
    }

    // Render ship
    SDL_Rect ship = m_ship.getBody().toRect();
    SDL_RenderCopyEx(m_renderer, m_ship.getTexture(), nullptr, &ship, m_ship.getRotation() + this->FindDirectionToMouse() - 90.f, nullptr, SDL_FLIP_NONE);

    // Render score text
    SDL_RenderCopy(m_renderer, m_scoreTexture, nullptr, &m_scoreRect);

    // Render high score text
    SDL_RenderCopy(m_renderer, m_highScoreTexture, nullptr, &m_highScoreRect);

    // Render health text
    SDL_RenderCopy(m_renderer, m_healthTexture, nullptr, &m_healthRect);

    // Render wave text
    SDL_RenderCopy(m_renderer, m_waveTexture, nullptr, &m_waveRect);

    // If the wavecount is equal to the limit then display the 'you win' text
    // This is last so it renders over the other objects
    if (m_waveCount == m_waveLimit)
    {
        SDL_RenderCopy(m_renderer, m_winTexture, nullptr, &m_winRect);
    }

    // If the wavecount = 10 then display the 'you win' text
    // This is last so it renders over the other objects
    if (m_ship.getHitPoints() <= 0)
    {
        SDL_RenderCopy(m_renderer, m_loseTexture, nullptr, &m_loseRect);
    }

    // Update Screen
    SDL_RenderPresent(m_renderer);
}

float Game::DeltaTime()
{
    // Set current time to a floating variable of ticks / 1000
    m_currentTime = static_cast <float>(SDL_GetTicks()) / 1000.0f;

    // delta time is the current time minus the time before
    float deltaTime = m_currentTime - m_lastTime;

    // apply the last time as the current time
    m_lastTime = m_currentTime;

    // if deltatime is less than the framerate, delay the program
    if (deltaTime < (m_frameRate))
    {
        SDL_Delay((Uint32)((m_frameRate - deltaTime) * 1000.0f));
    }

    // Return delta time value
    return deltaTime;
}

double Game::FindDirectionToMouse()
{
    // create local vars
    int Delta_x; int Delta_y;
    int mouse_x, mouse_y;
    double result;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    // In order to get the actual centre of the ship's pos
    // We need to get the half size of the rect (50x50)
    // This is because the ship's body is technically at point 0 of the rect
    Delta_x = (int)(m_ship.getBody().x + 25.f) - mouse_x;
    Delta_y = (int)(m_ship.getBody().y + 25.f) - mouse_y;

    // calculate rotation result
    result = (atan2(Delta_y, Delta_x) * 180.0000) / 3.14159265;

    // Returns the correct rotation result
    return result;
}

void Game::ScrollBackground()
{
    
    //If the background has gone too far
    if (m_backgroundRect.x <= -m_backgroundRect.w)
    {
        //Reset the offset
        m_backgroundRect.x = 640;
    }
    else
    {
        // Scroll background
        m_backgroundRect.x -= 1;
    }
    if (m_backgroundRectTwo.x <= -m_backgroundRect.w)
    {
        //Reset the offset
        m_backgroundRectTwo.x = 640;
    }
    else
    {
        // Scroll background
        m_backgroundRectTwo.x -= 1; 
    }  
}

void Game::SpawnEnemies(int numberOfEnemies)
{
    // Create a new asteroid for every enemy that it's told to spawn
    for (int i = 0; i < numberOfEnemies; i++)
    {
        // Create asteroids on the heap so they have to be manually deleted
        // Each asteroid is named 'asteroid' and has 15 hp and deals 10 contact damage
        // It might be fun to change these values to random but I'm choosing not to
        Asteroid* asteroid = new Asteroid({ "Asteroid " + std::to_string(i) }, 15, 10, m_renderer);

        asteroids.push_back(asteroid);
    }
}

bool Game::OutOfPlay(SDL_Rect &bullet)
{

    if (bullet.x + bullet.w < 0)
    {
        return true;
    }

    if (bullet.x > 640)
    {
        return true;
    }
    if (bullet.y + bullet.h < 0)
    {
        return true;
    }
    if (bullet.y > 480)
    {
        return true;
    }


    return false;
}

void Game::UpdateWaves()
{

    // Check if there is no more enemies alive
    // If there are none, spawn extra and increase the wave count
    if (m_waveCount == m_waveLimit)
    {
        // Call render once to display the win message before delaying
        Render();
        SDL_Delay(5000);
        Quit();
    }
    else if (m_numberOfEnemiesAlive <= 0)
    {
        // Clear old wave texture
        SDL_DestroyTexture(m_waveTexture);

        // Increase wave count member variable
        m_waveCount++;

        // Increase the number of enemies to spawn by a random number between 1 and 3
        m_numberOfEnemiesToSpawn += rand() % 1 + 3;
        SpawnEnemies(m_numberOfEnemiesToSpawn);

        // Update wave string
        m_waveStr = "wave: " + std::to_string(m_waveCount);

        // Update surface with the updated wave string
        m_waveSurface = TTF_RenderText_Solid(m_font, m_waveStr.c_str(), m_waveColour);

        // Update image to the new texture
        m_waveTexture = SDL_CreateTextureFromSurface(m_renderer, m_waveSurface);

        // Free memory of the surface:
        SDL_FreeSurface(m_waveSurface);

    }
}

void Game::UpdateScores()
{
    // Destroy old textures before making new updated ones
    SDL_DestroyTexture(m_highScoreTexture);
    SDL_DestroyTexture(m_scoreTexture);

    // Update score string
    m_scoreStr = "score: " + std::to_string(m_score);

    // Update surface with the updated score string
    m_scoreSurface = TTF_RenderText_Solid(m_font, m_scoreStr.c_str(), m_scoreColour);

    // Update image to the new surface
    m_scoreTexture = SDL_CreateTextureFromSurface(m_renderer, m_scoreSurface);

    // Free memory of the surface:
    SDL_FreeSurface(m_scoreSurface);

    // Update score string
    m_highScoreStr = "highscore: " + std::to_string(m_highScore);

    // Update surface with the updated score string
    m_highScoreSurface = TTF_RenderText_Solid(m_font, m_highScoreStr.c_str(), m_highScoreColour);

    // Update image to the new surface
    m_highScoreTexture = SDL_CreateTextureFromSurface(m_renderer, m_highScoreSurface);

    // Free memory of the surface:
    SDL_FreeSurface(m_highScoreSurface);

}

void Game::HandleCollisions()
{

    // ------------------------ HANDLE BULLET VS ASTEROID COLLISIONS ------------------------
    for (size_t x = 0; x < m_ship.getBullet().size(); x++)
    {
        SDL_Rect localShipBulletRect = m_ship.getBullet()[x]->getBody().toRect();
        for (size_t i = 0; i < asteroids.size(); i++)
        {
            SDL_Rect localAsteroidRect = asteroids[i]->getBody().toRect();

            // Test if the current asteroid and bullet have collided
            if (SDL_HasIntersection(&localShipBulletRect, &localAsteroidRect) == SDL_TRUE)
            {
                // Update score
                m_score += asteroids[i]->getPointsWorth();

                // Check if highscore is lower than score and update it if it is
                if (m_highScore < m_score)
                {
                    m_highScore = m_score;
                }

                // Update score and/or highscore text
                UpdateScores();

                // If they've collided, delete both the bullet and the asteroid instances
                delete m_ship.getBullet()[x];
                delete asteroids[i];

                // Set them to nullptrs
                m_ship.getBullet()[x] = nullptr;
                asteroids[i] = nullptr;
            }
        }
    }

    // Remove any bullets or asteroids that have been set to nullptr from the vector array
    m_ship.getBullet().erase(std::remove(m_ship.getBullet().begin(), m_ship.getBullet().end(), nullptr), m_ship.getBullet().end());
    asteroids.erase(std::remove(asteroids.begin(), asteroids.end(), nullptr), asteroids.end());

    // ------------------------ HANDLE SHIP VS ASTEROID COLLISIONS ------------------------

    // Create local rect of ship to use for collision testing
    SDL_Rect localShipRect = m_ship.getBody().toRect();

    for (size_t i = 0; i < asteroids.size(); i++)
    {
        // Create local rect of asteroid to use for collision testing for each asteroid
        SDL_Rect localAsteroidRect = asteroids[i]->getBody().toRect();

        // Test if the asteroids are currently 'active'
        // This is in place so that when the asteroids spawn, they won't instantly kill the player
        if (asteroids[i]->getActiveState() == true)
        {
            // Test if the current asteroid and bullet have collided
            if (SDL_HasIntersection(&localShipRect, &localAsteroidRect) == SDL_TRUE)
            {
                // Update score
                m_score += asteroids[i]->getPointsWorth();

                // Check if highscore is lower than score and update it if it is
                if (m_highScore < m_score)
                {
                    m_highScore = m_score;
                }

                // Update score and/or highscore text
                UpdateScores();

                // Set ships health to it's current hp - asteroids damage
                m_ship.setHP(m_ship.getHitPoints() - asteroids[i]->getDamage());
                std::cout << "Ships health: " << m_ship.getHitPoints() << std::endl;

                // Destroy the old health texture to free memory
                SDL_DestroyTexture(m_healthTexture);

                // Update health text with new hp amount
                m_healthStr = "health: " + std::to_string(m_ship.getHitPoints());
                m_healthSurface = TTF_RenderText_Solid(m_font, m_healthStr.c_str(), m_healthColour);
                m_healthTexture = SDL_CreateTextureFromSurface(m_renderer, m_healthSurface);
                SDL_FreeSurface(m_healthSurface);

                // Play shoot sound
                Mix_PlayChannel(2, m_ship.getCrashSound(), 0);
                // Edit audio:
                Mix_VolumeChunk(m_ship.getCrashSound(), 15);

                // If they've collided, delete both the bullet and the asteroid
                delete asteroids[i];

                // Set them to nullptrs
                asteroids[i] = nullptr;

            }
        }  
    }

    // Remove any bullets or asteroids from the vector array
    m_ship.getBullet().erase(std::remove(m_ship.getBullet().begin(), m_ship.getBullet().end(), nullptr), m_ship.getBullet().end());
    asteroids.erase(std::remove(asteroids.begin(), asteroids.end(), nullptr), asteroids.end());
}
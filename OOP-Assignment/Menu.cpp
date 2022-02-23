#include "Menu.h"

void Menu::MenuLoop()
{
    // Create an instance of the music and play it. -1 means it loops
    Mix_PlayMusic(m_backgroundMusic, -1);
    Mix_VolumeMusic(10);

    // While menu state is true we can do the menu loop
	while (m_menuState == true)
	{
        // Check for player input
		PollEvents();

        // Render background and text objects
		Render();
	}
	return;
}

// Wait for user to press P to start 
void Menu::PollEvents()
{
    const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:

            // If player presses P in the menu, it will run the game.
            // Once the game finishes, it will update the highscore for the menu
            if ((keyboard_state_array[SDL_SCANCODE_P]))
            {
                // Create a game instance
                // The game takes the renderer, checks if debug mode is on, and takes the current highscore so it can display it
                Game* game = new Game(m_renderer, m_highScore);

                // ----- RUN THE GAME -----
                // This will use the game object's run function to start the game loop.
                // Once the game has finished, it will return the highscore and be used in setHighscore
                setHighscore(game->Run());

                // Update the menu's highscore text
                UpdateHighScoreText();

                // Delete the instance of the game to free memory and reset the game
                delete game;
            }

            // If the player presses 'R' in the menu, it will reset the highscore to 0 and update the highscore text
            if ((keyboard_state_array[SDL_SCANCODE_R]))
            {
                setHighscore(0);
                UpdateHighScoreText();
                SDL_Delay(400);
            }

            // If the player presses 'Escape' in the menu, it will exit the menu loop and quit
            if ((keyboard_state_array[SDL_SCANCODE_ESCAPE]))
            {
                m_menuState = false;
            }
            break;
        }
    }
}

void Menu::UpdateHighScoreText()
{
    // Set highscore to what it can read in the save file
    m_highScore = readHighscore();

    // Update score string
    m_highScoreStr = "highscore: " + std::to_string(m_highScore);

    // Update surface with the updated score string
    m_highScoreSurface = TTF_RenderText_Solid(m_font, m_highScoreStr.c_str(), m_highScoreColour);

    // Update image to the new surface
    m_highScoreImage = SDL_CreateTextureFromSurface(m_renderer, m_highScoreSurface);

    // Free memory of the surface:
    SDL_FreeSurface(m_highScoreSurface);
}

void Menu::Render()
{
    // Clear screen
    SDL_RenderClear(m_renderer);

    // Set background default colour (black)
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

    // Set background image
    SDL_RenderCopyEx(m_renderer, this->m_backgroundTexture, nullptr, &this->m_backgroundRect, 0, nullptr, SDL_FLIP_NONE);

    // Render start and quit images
    SDL_RenderCopyEx(m_renderer, this->m_startButtonTexture, nullptr, &this->m_startButtonRect, 0, nullptr, SDL_FLIP_NONE);
    SDL_RenderCopyEx(m_renderer, this->m_quitButtonTexture, nullptr, &this->m_quitButtonRect, 0, nullptr, SDL_FLIP_NONE);

    // Render highscore text
    SDL_RenderCopyEx(m_renderer, m_highScoreImage, nullptr, &this->m_highScoreRect, 0, nullptr, SDL_FLIP_NONE);

    // Update Screen
    SDL_RenderPresent(m_renderer);
}

int Menu::readHighscore()
{
    // Create object of fstream
    std::fstream stream;

    // Open highscore file using ios::in to say we're inputting info to the file
    stream.open("Highscore/highscore.txt", std::ios::in);

    // Check if it can find the file
    if (!stream)
    {
        std::cout << "No such file exists";
    }
    else
    {
        // read the highscore from file and assign it to class' highscore variable
        while (!stream.eof())
        {
            stream >> m_highScore;
            std::cout << "Reading highscore... : " << m_highScore << " points!" << std::endl;
        }
        // Close highscore file
        stream.close();
    }

    // return the new amount 
    return m_highScore;
}

void Menu::setHighscore(int _highscore)
{
    // Create object of fstream
    std::fstream stream;

    // Open highscore file using ios::out to say we're outputting info to the file
    stream.open("Highscore/highscore.txt", std::ios::out);

    // Check if the file is made
    if (!stream)
    {
        std::cout << "File creation failed";
    }
    else
    {
        std::cout << "File overwritten!" << std::endl;

        // Write the highscore to the file
        stream << _highscore; 

        // Close the file
        stream.close();
    }
}

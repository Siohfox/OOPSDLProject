#include "Player.h"
#include "Bullet.h"

void PlayerShip::Update(float deltaTime)
{
	Move(deltaTime);
}

void PlayerShip::Fire(float deltaTime, SDL_Renderer* renderer)
{

	// Listener for keyboard inputs
	const unsigned char* keys = SDL_GetKeyboardState(nullptr);


	// if user presses space and the ship has 'reloaded' as determined by the fireDelay variable...
	if (keys[SDL_SCANCODE_SPACE] && fireDelay <= 0.0f)
	{

		// Create a new instance of a bullet, passing in the ship's current position 'm_rect' to the bullet
		Bullet* bullet = new Bullet("Bullet", 10, 10, renderer, m_rect);

		// Set bullet's location to the current ship's location
		// We can call the bullet's getBody getter function to use it's own width and height rather than setting them here
		bullet->setRect({ m_rect.x+25, m_rect.y+25, bullet->getBody().w, bullet->getBody().h });

		// Add bullet to the back of the vector
		bullets.push_back(bullet);

		// Delay the fire so the user cannot fire constantly
		fireDelay = 5.0f;

		// Play shoot sound
		Mix_PlayChannel(2, m_player_shoot, 0);
		// Edit audio:
		Mix_VolumeChunk(m_player_shoot, 10);

	}
	// if the firedelay is more than 0, take away until it's back to 0 and 'reloaded'
	if (fireDelay > 0)
	{
		fireDelay -= 0.1f;
	}
	
}

void PlayerShip::Move(float deltaTime)
{

	int maxX = 600, maxY = 440, minX = 0, minY = 0;

	// Get player inputs, and move in direction accordingly
	// For each input, check if player has reached the boundary
	const unsigned char* keys = SDL_GetKeyboardState(nullptr);
	if (keys[SDL_SCANCODE_W])
	{
		if (m_rect.y > minY)
		{
			m_rect.y -= m_speed * deltaTime;
		}
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (m_rect.x > minX)
		{
			m_rect.x -= m_speed * deltaTime;
		}
	}
	if (keys[SDL_SCANCODE_S])
	{
		if (m_rect.y < maxY)
		{
			m_rect.y += m_speed * deltaTime;
		}
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (m_rect.x < maxX)
		{
			m_rect.x += m_speed * deltaTime;
		}
	}
}

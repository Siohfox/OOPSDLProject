#ifndef ASTEROID_H
#define ASTEROID_H
#include "Entity.h"
#include <vector>

class Asteroid : public Entity
{ 
public:
	// Constructor

	Asteroid(std::string name, int hp, int damage, SDL_Renderer* renderer)
	{
		m_image = SDL_LoadBMP("Images/meteorBrown_big1.bmp");
		if (!m_image)
		{
			throw std::exception();
		}
		m_texture = SDL_CreateTextureFromSurface(renderer, m_image);
		SDL_FreeSurface(m_image);
		this->m_hp = hp;
		this->m_damage = damage;
		this->m_name = name;
		this->m_speed = 10.f + rand() % 200;
		this->m_pointsWorth = 10;
		
		m_maxX = 600;
		m_maxY = 440;
		m_minX = 0;
		m_minY = 0;

		// Set initial spawnpoint to be randomly anywhere on the screen
		this->m_rect = { static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_maxX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_maxY)), 50.f, 50.f };

		// set random directions to -1, which means no direction by default
		this->m_XDirection = -1;
		this->m_YDirection = -1;

		asteroidSounds[0] = Mix_LoadWAV("Audio/asteroidexplodeone.wav");
		asteroidSounds[1] = Mix_LoadWAV("Audio/asteroidexplodetwo.wav");

		m_active = false;
		m_activeTimer = 30.f;
	}

	// Destructor
	~Asteroid()
	{

		int random = rand() % 2;
		// Play shoot sound
		Mix_PlayChannel(-1, asteroidSounds[random], 0);
		// Edit audio:
		Mix_VolumeChunk(asteroidSounds[random], 3);

		
		// Attempt at deleting sounds for memory...
		asteroidSounds[0] = nullptr;
		delete asteroidSounds[0];
		asteroidSounds[1] = nullptr;
		delete asteroidSounds[1];

		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}

	//Functions
	
	void Update(float deltaTime) override;

	// Getters
	inline const int& getPointsWorth() { return this->m_pointsWorth; }
	inline const bool& getActiveState() { return this->m_active; }
	inline Mix_Chunk* getAsteroidSounds(int asteroidSound) { return asteroidSounds[asteroidSound]; }

private:
	int m_XDirection, m_YDirection;
	int m_pointsWorth = 10;
	int m_maxX, m_maxY, m_minX, m_minY;
	bool m_active;
	float m_activeTimer;
};

#endif // !ASTEROID_H
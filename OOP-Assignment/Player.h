#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
#include <vector>
class Bullet;

class PlayerShip : public Entity
{
public:

	// Constructor
	PlayerShip(std::string name, int hp, int damage, SDL_Renderer* renderer)
	{
		
		m_image = SDL_LoadBMP("Images/playerShip3_blue.bmp");
		if (!m_image)
		{
			throw std::exception();
		}
		m_texture = SDL_CreateTextureFromSurface(renderer, m_image);
		SDL_FreeSurface(m_image);
		this->m_rect = { 300, 300, 50, 50 };
		this->m_hp = hp;
		this->m_damage = damage;
		this->m_name = name;
		this->m_speed = 500.0f;
		this->m_rotation = 0;
		this->fireDelay = 0.0f;
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		m_player_shoot = Mix_LoadWAV("Audio/laserpew.wav");
		m_player_crash = Mix_LoadWAV("Audio/HollowBang.wav");
	}

	// Destructor
	~PlayerShip()
	{
		SDL_DestroyTexture(m_texture);
		Mix_FreeChunk(this->m_player_shoot);
		Mix_FreeChunk(this->m_player_crash);
	}

	// Update handles movement
	void Update(float deltaTime) override;
	void Fire(float deltaTime, SDL_Renderer* renderer);
	void Move(float deltaTime);


	// Getters
	inline std::vector<Bullet*>& getBullet() { return bullets; }
	inline Mix_Chunk* getCrashSound() { return m_player_crash; }

protected:
	float fireDelay;
	std::vector<Bullet*> bullets;
	Mix_Chunk* m_player_shoot;
	Mix_Chunk* m_player_crash;
};

#endif
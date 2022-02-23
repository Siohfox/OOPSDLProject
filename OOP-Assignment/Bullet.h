#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"

class PlayerShip;

class Bullet : public Entity
{
public:
	// Constructor
	Bullet(std::string name, int hp, int damage, SDL_Renderer* renderer, FRect shipBody)
	{
		m_image = SDL_LoadBMP("Images/laserRed.bmp");
		if (!m_image)
		{
			throw std::exception();
		}
		m_texture = SDL_CreateTextureFromSurface(renderer, m_image);
		SDL_FreeSurface(m_image);
		this->m_rect = { 0, 0, 5, 30 };
		this->m_hp = hp;
		this->m_damage = damage;
		this->m_name = name;
		this->m_speed = 2000.0f;

		// Get mouse state returns the pos of the mouse and assigns it to the class vars
		SDL_GetMouseState(&m_mouse_x, &m_mouse_y);

		// Every time a bullet is constructed, the direction it needs to head
		// will be calculated by the mouse's position minus the ship's central position
		m_directionX = (float)(m_mouse_x - (shipBody.toRect().x + shipBody.toRect().w / 2));
		m_directionY = (float)(m_mouse_y - (shipBody.toRect().y + shipBody.toRect().w / 2));
		m_velocity = { m_mouse_x, m_mouse_y };
	}

	~Bullet()
	{
		// Clear up textures
		SDL_DestroyTexture(m_texture);
	}

	void Update(float deltaTime) override;
	void MoveTowards(float deltaTime);

private:
	int m_mouse_x, m_mouse_y;
	float m_directionX, m_directionY, m_magnitude;
	SDL_Point m_velocity;
};

#endif
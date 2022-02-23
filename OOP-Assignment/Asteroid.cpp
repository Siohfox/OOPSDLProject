#include "Asteroid.h"
#include <vector>

void Asteroid::Update(float deltaTime)
{

	// There was a strange bug where the asteroids where spawning way out of bounds
	// This solution simply tests if they've somehow exited the boundary and teleports them back in
	if (this->m_rect.x > m_maxX + 20 || this->m_rect.y > m_maxY + 20)
	{
		this->m_rect = { static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_maxX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_maxY)), 50.f, 50.f };
	}
	if (this->m_rect.x < m_minX - 20 || this->m_rect.y < m_minY - 20)
	{
		this->m_rect = { static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_maxX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_maxY)), 50.f, 50.f };
	}
	

	// while it has no direction picked, it will pick a random direction for x and y, left/right and up/down
	if(m_XDirection == -1 || m_YDirection == -1)
	{
		m_XDirection = rand() % 2;
		m_YDirection = rand() % 2;
	}

	if (m_activeTimer > 0 && m_active == false)
	{
		m_activeTimer -= 0.1f;
	}
	else if (m_activeTimer <= 0 && m_active == false)
	{
		m_active = true;
	}
	
	// Every update, it will find which direction it's supposed to head in and go that way, if it has reached a wall it will bounce off of it.
	if (m_YDirection == 0)
	{
		if (m_rect.y > m_minY)
		{
			m_rect.y -= m_speed * deltaTime;
		}
		else
		{
			m_YDirection = 1;
		}
	}
	if (m_XDirection == 0)
	{
		if (m_rect.x > m_minX)
		{
			m_rect.x -= m_speed * deltaTime;
		}
		else
		{
			m_XDirection = 1;
		}
	}
	if (m_YDirection == 1)
	{
		if (m_rect.y < m_maxY)
		{
			m_rect.y += m_speed * deltaTime;
		}
		else
		{
			m_YDirection = 0;
		}
	}
	if (m_XDirection == 1)
	{
		if (m_rect.x < m_maxX)
		{
			m_rect.x += m_speed * deltaTime;
		}
		else
		{
			m_XDirection = 0;
		}
	}
}
#include "Bullet.h"

void Bullet::Update(float deltaTime)
{
	MoveTowards(deltaTime);
}


// This function moves the bullet towards the mouse's location's direction
void Bullet::MoveTowards(float deltaTime)
{
	// X2 + Y2 = C2
	m_magnitude = (m_directionX * m_directionX) + (m_directionY * m_directionY);

	// Squareroot C2 to find the magnitude
	m_magnitude = std::sqrt(m_magnitude);

	// calc direction for x and y  by deviding the direction by the magnitude
	m_directionX = (m_directionX / m_magnitude) ;
	m_directionY = (m_directionY / m_magnitude);

	// move object in the given direction
	m_rect.x += m_directionX * (m_speed * deltaTime);
	m_rect.y += m_directionY * (m_speed * deltaTime);
}


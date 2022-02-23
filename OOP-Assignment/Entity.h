#ifndef ENTITY_H
#define ENTITY_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>
#include <time.h>

// I had to make this structure because the older version of SDL does not seem to support floating point values in Rects
// So we have to make a floating point rect, and use toRect to convert it back to an integer rect, so we can use it in rendercopy.
struct FRect
{
	float x;
	float y;
	float w;
	float h;

	SDL_Rect toRect() const;
};

class Entity
{	
public:
	
	Entity()
	{
		// Default stuff to construct for all entity objects
		// It doesn't really matter what we put here since children will all likely override it
		m_hp = 30;
		m_damage = 10;
		m_image = nullptr;
		m_texture = nullptr;
		m_rect = { 300.f, 300.f, 50.f, 50.f };
		m_rect.toRect();
		m_name = "DefaultName";
		SDL_FreeSurface(m_image);
	}

	~Entity()
	{
		// Clear up SDL for memory
		SDL_DestroyTexture(m_texture);
		for (int i = 0; i < 2; i++)
		{
			Mix_FreeChunk(asteroidSounds[i]);
		}
	}

	// Getters
	inline const FRect& getBody() const { return this->m_rect; }
	inline SDL_Texture* getTexture() { return this->m_texture; }
	inline const int& getRotation() { return this->m_rotation; }
	inline const int& getHitPoints() { return this->m_hp; }
	inline const int& getDamage() { return this->m_damage; }

	// Setters
	inline void setRotation(const int rotation) { this->m_rotation = rotation; }
	inline void setRect(const FRect rect) { this->m_rect = rect; }
	inline void setHP(const int hitPoints) { this->m_hp = hitPoints; }

	// pure virtual so same func can be used differently by different children
	virtual void Update(float deltaTime) = 0;

protected:
	int m_hp, m_damage, m_rotation;
	float m_speed;
	std::string m_name;
	FRect m_rect;
	SDL_Surface* m_image;
	SDL_Texture* m_texture;
	Mix_Chunk* asteroidSounds[2];
};


#endif // !ENTITY_H
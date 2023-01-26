#pragma once
#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

class GameTile
{
private:
	//sprites
	sf::Texture texture;
	sf::Sprite sprite;

	float length;
	float height;

	//Variables
	float velocity;
	float velocityMax;
	float velocityMin;
	float acceleration;
	float drag;

	//Private Functions
    void initSprite(std::string name, sf::Vector2f scale);
	void initVariables();
	void initMovement(float velocityMin);





public:
	//constr and destr
	GameTile(std::string name, sf::Vector2f scale, float pos_x, float pos_y, float velocityMin);
	~GameTile();

	void move(float dir_y, float time);
	

	//Accessors
	sf::Sprite getSprite();
	sf::FloatRect getBounds();
	float getVelocity();


	
	void render(sf::RenderTarget& target);

};


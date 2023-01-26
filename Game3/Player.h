#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include "math.h"

class Player
{
private:

	//Sprite 
	sf::Texture textureLeft;
	sf::Texture textureRight;
	sf::Sprite sprite;
	sf::Vector2f scale;

	sf::Vector2f startingPossition;

	//Physics
	sf::Vector2f velocity;
	float velocityMax;
	float velocityMin;
	float acceleration;
	float drag;

	float gravity;
	float gravityMax;

	float jumpValue;
	

	//Functions
	void initVariables();
	void initSprite();
	void initPhysics();

public:

	//constr and destr
	Player();
	~Player();


	//movement
	void updateVelocity(float dir_x, float dir_y, float time);
	void jump();


	void playerUpdate(float time);
	void updatePhysics(float time);
	


	//accessors
	sf::Sprite getSprite();
	sf::Vector2f getVelocity();
	sf::FloatRect getBounds();
	
	//modifiers
	void setPossition(float pos_x, float pos_y);
	void setVelocity(float velocity_x, float velocity_y);

	//render
	void render(sf::RenderTarget& target);







};


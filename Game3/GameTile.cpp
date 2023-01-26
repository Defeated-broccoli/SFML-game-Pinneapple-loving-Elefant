#include "GameTile.h"


void GameTile::initSprite(std::string name, sf::Vector2f scale)
{
	if (!texture.loadFromFile(name))
	{
		std::cout << "Error. Failed to load GameTile texture!" << std::endl;
	}
	sprite.setTexture(texture);
	//sprite.setTextureRect(sf::IntRect(0, 0, length, height));
	sprite.setScale(scale);
}

void GameTile::initVariables()
{
	length = 20;
	height = 20;
}

void GameTile::initMovement(float velocityMin)
{
	velocity = 0.5;
	velocityMax = 2.5f;
	this->velocityMin = velocityMin;
	acceleration = 0.1f;
	drag = 0.98f;
}

GameTile::GameTile(std::string name, sf::Vector2f scale, float pos_x, float pos_y, float velocityMin)
{
	initVariables();
	initSprite(name, scale);
	sprite.setPosition(pos_x, pos_y);
	initMovement(velocityMin);
}

GameTile::~GameTile()
{

}

void GameTile::move(float dir_y, float time)
{
	if (dir_y == 0.5f)
	{
		velocity = velocityMin;
	}
		//Acceleration
		velocity += dir_y * acceleration * time / 5;

		//Limit Velocity
		if (std::abs(velocity) > velocityMax)
		{
			velocity = velocityMax * ((velocity < 0.f) ? -1.f : 1.f);
		}

		//deceleration
		velocity = velocity * pow(drag, time / 5);

		//limit Deceleration
		if (std::abs(velocity) < velocityMin)
		{
			velocity = velocityMin;
		}

		sprite.move(0.f, velocity * time / 5);
	
	
}


sf::Sprite GameTile::getSprite()
{
	return sprite;
}

sf::FloatRect GameTile::getBounds()
{
	return sprite.getGlobalBounds();
}

float GameTile::getVelocity()
{
	return velocity;
}

void GameTile::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

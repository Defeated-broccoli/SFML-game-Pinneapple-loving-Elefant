#include "Player.h"

void Player::initVariables()
{
	startingPossition = sf::Vector2f(300,100);
	scale = sf::Vector2f(0.25f, 0.25f);
}

void Player::initSprite()
{
	/*if (!textureLeft.loadFromFile("images/elefantMicroLeft.png"))
	{
		std::cout << "Error. Couldn't load the player image" << std::endl;
	}
	if (!textureRight.loadFromFile("images/elefantMicroRight.png"))
	{
		std::cout << "Error. Couldn't load the player image" << std::endl;
	}*/
	
	sf::Image playerLeft;
	sf::Image playerRight;

	textureLeft.loadFromFile("images/elefantLeft.png");
	textureRight.loadFromFile("images/elefantRight.png");

	sprite.setTexture(textureLeft);
	sprite.setScale(scale);
	sprite.setPosition(startingPossition);
	

}


void Player::initPhysics()
{
	velocity = sf::Vector2f(0, 0);
	velocityMax = 2.f;
	velocityMin = 0.4f;
	acceleration = 1.1f;
	drag = 0.98f;
	gravity = 0.15f;
	gravityMax = 7.f;
	jumpValue = 13.f;
}

Player::Player()
{
	initVariables();
	initSprite();
	initPhysics();
}

Player::~Player()
{
}

void Player::updateVelocity(float dir_x, float dir_y, float time)
{
	
	
		//Acceleration
		velocity.x += dir_x * acceleration;
		velocity.y += dir_y * acceleration;


		//Limit Velocity
		if (std::abs(velocity.x) > velocityMax)
		{
			velocity.x = velocityMax * ((velocity.x < 0.f) ? -1.f : 1.f);
		}
	
}

void Player::jump()
{
	velocity.y = -jumpValue;
}

void Player::playerUpdate(float time)
{
	updatePhysics(time);
}

void Player::updatePhysics(float time)
{
	
	
		//Deceleration
		velocity = velocity * pow(drag, time / 5);
		velocity.y = velocity.y + gravity;

		//limit Deceleration
		if (std::abs(velocity.x) < velocityMin)
		{
			velocity.x = 0.f;
		}
		//limit gravity
		if (velocity.y > gravityMax)
		{
			velocity.y = gravityMax;
		}

		if (velocity.x < 0)
		{
			sprite.setTexture(textureLeft);
		}
		if (velocity.x > 0)
		{
			sprite.setTexture(textureRight);
		}

		sprite.move(velocity.x * time / 5, velocity.y * time / 5);
}


sf::Sprite Player::getSprite()
{
	return sprite;
}

sf::Vector2f Player::getVelocity()
{
	return velocity;
}

void Player::setPossition(float pos_x, float pos_y)
{
	sprite.setPosition(pos_x, pos_y);
}

void Player::setVelocity(float velocity_x, float velocity_y)
{
	velocity = sf::Vector2f(velocity_x, velocity_y);
}


sf::FloatRect Player::getBounds()
{
	return sprite.getGlobalBounds();
}


void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

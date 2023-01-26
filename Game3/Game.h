#pragma once
#include <iostream>

#include "GameTile.h"
#include "Player.h"

class Game
{
private:
	//widnow
	int windowHeight;
	int windowWidth;
	bool gameOn;

	sf::RenderWindow* window;
	sf::Event ev;

	//background sprite
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	//tile
	GameTile* bottomTile;
	int tilePlacement;
	int previousTilePlacement;
	std::vector<GameTile*> tiles;
	float lengthOfGap;
	int pointsPerLevel;
	float velocityMin;
	float velocityIncrease;
	std::vector<GameTile*> fruits;

	//player
	Player* player;
	bool playerTilesCollision;
	bool playerWindowCollision;

	//player dead
	sf::Font gameOverFont;
	sf::Text gameOverText;


	//Time
	sf::Time deltaTime;
	sf::Clock clock;
	float time;

	//font
	sf::Font font;
	sf::Text pointText;
	float fontSize;
	sf::Vector2f textPossition;

	//points
	unsigned int points;

	//jump logic
	bool canJump;
	float jumpCheckHeight;
	sf::FloatRect jumpBox;

	//music
	sf::Music music;


public:
	void run();
	
	//Constr and Destr
	Game();
	~Game();



	//Functions
	void initialize();
	void initVariables();
	void initWindow();
	void initBackground();
	void initMap();
	void initPlayer();
	void initFont();
	void initSystem();
	void initMusic();


	//update functions
	void update();
	void updatePollEvents();
	void updateInput();
	void updateTime();
	void updateTiles();
	void updateCollision();
	void updateWindowCollision();
	void updateTilesCollision();
	void updateFruitsCollision();
	void updateMovement();
	void updateStaticMovement();
	void updateStaticTiles();
	void updateStaticPlayer();
	void updateTilesSpawn();
	void updateTilesDelete();
	void updateFruitsDelete();
	void updatePlayer();
	void updatePlayerDeath();
	void updateFont();
	void updateJump();
	void updateLevel(unsigned int points, int pointsPerLevel);
	void updateFruitSpawn();
	void updateMusic();
	





	//render functions
	void render();
	void renderWindow();
	void renderBackground();
	void renderMap();
	void renderPlayer();
	void renderPoints();
	void renderPlayerDeath();



};


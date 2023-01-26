#include "Game.h"


void Game::run()
{
	while (window->isOpen())
	{
		update();
		render();
	}
}

Game::Game()
{
	initialize();
}

Game::~Game()
{

}

void Game::initialize()
{
	initVariables();
	initWindow();
	initBackground();
	initMap();
	initPlayer();
	initSystem();
	initMusic();
	
}

void Game::initVariables()
{
	windowWidth = 600;
	windowHeight = 800;
	gameOn = true;

	tilePlacement = 0;
	previousTilePlacement = 0;

	playerTilesCollision = false;
	playerWindowCollision = false;

	lengthOfGap = 100.f;
	velocityMin = 0.1f;

	pointsPerLevel = 50;
	velocityIncrease = 1.2f;

	fontSize = 30;
	textPossition = sf::Vector2f(20, 20);

	points = 0;

	canJump = false;
	jumpCheckHeight = 5.f;
}

void Game::initWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Game 3");
	window->setFramerateLimit(144);
}

void Game::initBackground()
{
	if (!backgroundTexture.loadFromFile("images/background.png"))
	{
		std::cout << "Error. Failed to load the background" << std::endl;
	}
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(0.5f, 0.5f);
	//backgroundSprite.setTextureRect(sf::IntRect(0, 0, window->getSize().x, window->getSize().y));
	

}

void Game::initMap()
{

	//create starting map
	for (int i = windowHeight / lengthOfGap; i > 0; --i)
	{
		//randomize tiles placement
		tilePlacement = rand() % (windowWidth / 20 - 5);
		if (!(tilePlacement < previousTilePlacement - 2) || !(tilePlacement > previousTilePlacement + 2))
		{
			int n = rand() % 2;
			if (n == 0)
			{
				tilePlacement += 2;
			}
			else
			{
				tilePlacement -= 2;
			}
		}

		tiles.push_back(new GameTile("images/stone5.png", sf::Vector2f(1.f, 1.f), 20.f * tilePlacement, lengthOfGap * i + 100, velocityMin));
		
		previousTilePlacement = tilePlacement;
	}

	bottomTile = new GameTile("images/bottomLava.png", sf::Vector2f(1.f, 1.f), 0.f, window -> getSize().y - 20.f, 0.f);
}

void Game::initPlayer()
{
	 player = new Player();
	 player->setPossition(tiles[4]->getBounds().left + tiles[4]->getBounds().width / 2.f - player->getBounds().width / 2.f, tiles[4]->getBounds().top - player->getBounds().height);
}

void Game::initFont()
{
	if (!font.loadFromFile("fonts/fontPoints.ttf"))
	{
		std::cout << "Error, game failed to load font" << std::endl;
	}
	pointText.setFont(font);
	pointText.setCharacterSize(fontSize);
	pointText.setPosition(textPossition);
	pointText.setFillColor(sf::Color::White);
	pointText.setOutlineColor(sf::Color::Black);

	pointText.setString(points);

}

void Game::initSystem()
{
	initFont();
}


void Game::update()
{
	updatePollEvents();
	if (gameOn == true)
	{
		updateTime();
		updateTiles();
		updatePlayer();
		updateMovement();
	}
}

void Game::updatePollEvents()
{
	//closing the window
	while (window->pollEvent(ev))
	{
		if (ev.Event::type == sf::Event::Closed)
		{
			window->close();
		}

		if (ev.Event::KeyPressed && ev.Event::key.code == sf::Keyboard::Escape)
		{
			window->close();
		}
	}
}

void Game::updateInput()
{
	//move left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		player->updateVelocity(-1.f, 0.f, time);
	}
	//move right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		player->updateVelocity(1.f, 0.f, time);
	}
	//jump
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (canJump == true))
	{
		player->jump();
	}
	

}

void Game::updateTime()
{
	//measure time for movement 
	deltaTime = clock.restart();
	time = deltaTime.asMilliseconds();
}

void Game::updateTiles()
{
	//main update for tiles
	updateStaticMovement();
	updateCollision();
	updateTilesDelete();
	
}

void Game::updateCollision()
{
	updateWindowCollision();
	updateTilesCollision();
	updateFruitsCollision();

}

void Game::updateWindowCollision()
{
	bool right = false, left = false, top = false, bottom = false;
	//right side
	if (window->getSize().x < player->getBounds().left + player->getBounds().width)
	{
		player->setPossition(window->getSize().x - player->getBounds().width, player -> getBounds().top);
		player->setVelocity(0.f, player->getVelocity().y);
		right = true;
	}
	else
	{
		right = false;
	}
	//left side
	if (0 > player->getBounds().left)
	{
		player->setPossition(0, player->getBounds().top);
		player->setVelocity(0.f, player->getVelocity().y);
		left = true;
	}
	else
	{
		left = false;
	}
	//top 
	if (0 > player->getBounds().top)
	{
		player->setPossition(player -> getBounds().left, 0);
		player->setVelocity(player->getVelocity().x, 0.f);
		top = true;
	}
	else
	{
		top = false;
	}
	//bottom
	if (window->getSize().y < player->getBounds().top + player->getBounds().height)
	{
		player->setPossition(player -> getBounds().left, window -> getSize().y - player->getBounds().height);
		player->setVelocity(player->getVelocity().x, 0.f);
		bottom = true;
    }
	else
	{
		bottom = false;
	}

	if (right == true || left == true || top == true || bottom == true)
	{
		playerWindowCollision = true;
	}
	else
	{
		playerWindowCollision = false;
	}

}

void Game::updateTilesCollision()
{
	for (int i = 0; i < tiles.size(); ++i)
	{
			if (player->getBounds().intersects(tiles[i]->getBounds()))
			{
				
				//tiles top collision
				if ((player->getBounds().top + player->getBounds().height > tiles[i]->getBounds().top) && (player->getVelocity().y > 0))
				{
					player->setVelocity(player -> getVelocity().x, 0.f);
					player->setPossition(player->getBounds().left, tiles[i]->getBounds().top - player->getBounds().height);
					playerTilesCollision = true;
					
				}
				//tiles bottom collision
				else if ((player->getBounds().top < tiles[i]->getBounds().top + tiles[i]->getBounds().height) && (player->getVelocity().y < 0))
				{
					player->setVelocity(player->getVelocity().x, 0.f);
					player->setPossition(player->getBounds().left, tiles[i]->getBounds().top + tiles[i] -> getBounds().height + 5);
					playerTilesCollision = true;
				
				}
				//tiles right collision
				else if ((player->getBounds().left < tiles[i]->getBounds().left + tiles[i]->getBounds().width) && (player -> getVelocity().x < 0) )
				{
					player->setVelocity(0.f, player->getVelocity().y);
					player->setPossition(tiles[i] -> getBounds().left + tiles[i] -> getBounds().width, player->getBounds().top);
					playerTilesCollision = true;
					
				}
				//tiles left collision
				else if ((player->getBounds().left + player->getBounds().width > tiles[i]->getBounds().left) && (player -> getVelocity().x > 0))
				{
					player->setVelocity(0.f, player->getVelocity().y);
					player->setPossition(tiles[i]->getBounds().left - player -> getBounds().width , player->getBounds().top);
					playerTilesCollision = true;
					
				}
				
			}
			else
			{

				playerTilesCollision = false;
			}
	}
}

void Game::updateMovement()
{
	player -> getSprite().move(player -> getVelocity().x * time / 5, player -> getVelocity().y * time / 5);
}

void Game::initMusic()
{
	//load music
	if (!music.openFromFile("audio/mainAudio.wav"))
	{
		std::cout << "Error. Could not load the mainAudio file" << std::endl;
	}
	music.play();
}

void Game::updateStaticMovement()
{
	updateStaticTiles();
	updateStaticPlayer();
	updateTilesSpawn();
}

void Game::updateStaticTiles()
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (player->getBounds().top + 200.f < window->getSize().y / 2 - player->getBounds().height / 2)
		{
			tiles[i]->move(1.f, time);
		}
		else
		{
			tiles[i]->move(0.5f, time);
		}
	}

	for (int i = 0; i < fruits.size(); ++i)
	{
		if (player->getBounds().top + 200.f < window->getSize().y / 2 - player->getBounds().height / 2)
		{
			fruits[i]->move(1.f, time);
		}
		else
		{
			fruits[i]->move(0.5f, time);
		}
	}
}

void Game::updateStaticPlayer()
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		player->getSprite().move(0.f, tiles[0]->getVelocity());
	}


}

void Game::updateTilesSpawn()
{
	if (tiles[tiles.size() - 1]->getBounds().top > (lengthOfGap - 50))
	{
		//randomize tiles placement
		tilePlacement = rand() % (windowWidth / 20 - 5);
		if (!(tilePlacement < previousTilePlacement - 2) || !(tilePlacement > previousTilePlacement + 2))
		{
			int n = rand() % 2;
			if (n == 0)
			{
				tilePlacement += 2;
			}
			else
			{
				tilePlacement -= 2;
			}
		}

		tiles.push_back(new GameTile("images/stone5.png", sf::Vector2f(1.f, 1.f), 20.f * tilePlacement, tiles[tiles.size() - 1]->getBounds().top - lengthOfGap, velocityMin));

		previousTilePlacement = tilePlacement;


		updateFruitSpawn();
	}

}

void Game::updateTilesDelete()
{
	for (auto* tile : this->tiles)
	{
		int counter = 0;
		if (tile->getBounds().top > window->getSize().y)
		{
			delete tile;
			tiles.erase(tiles.begin() + counter);
			updateFont();
			updateLevel(points, pointsPerLevel);


		}
		counter++;
		
	}
}

void Game::updateFruitsDelete()
{
	for (auto* fruit : this->fruits)
	{
		int counter = 0;
		if (fruit->getBounds().top > window->getSize().y)
		{
			delete fruit;
			fruits.erase(fruits.begin() + counter);
		}
		counter++;

	}
}

void Game::updatePlayer()
{
	updateJump();
	updateInput();
	player->playerUpdate(time);
	updatePlayerDeath();
}

void Game::updateFruitsCollision()
{
	int counter = 0;
	for (auto* fruit : this->fruits)
	{
		if (player->getBounds().intersects(fruit->getBounds()))
		{
			delete fruit;
			fruits.erase(fruits.begin() + counter);
			points += 10;
			updateFont();
			updateLevel(points, pointsPerLevel);
		}
		counter++;
		
	}
}

void Game::updatePlayerDeath()
{
	if (player->getBounds().intersects(bottomTile->getBounds()))
	{
		//player dies
		delete player;
		gameOn = false;
		

		gameOverFont.loadFromFile("fonts/fontDeath.otf");
		gameOverText.setFont(gameOverFont);
		gameOverText.setString("YOU DIED");
		gameOverText.setCharacterSize(50);
		gameOverText.setPosition((window->getSize().x - gameOverText.getGlobalBounds().width) / 2.f, (window->getSize().y - gameOverText.getGlobalBounds().height) / 2.f);
		gameOverText.setFillColor(sf::Color::Red);

		pointText.setFillColor(sf::Color::Red);
		pointText.setString("POINTS: " + std::to_string(points));
		pointText.setPosition((window->getSize().x - pointText.getGlobalBounds().width) / 2.f, (window->getSize().y - pointText.getGlobalBounds().height) / 2.f + 60);
	}
}


void Game::updateFont()
{
	points++;
	pointText.setString(std::to_string(points));
}

void Game::updateJump()
{
	jumpBox = sf::FloatRect(player->getBounds().left, player->getBounds().top + player->getBounds().height, player->getBounds().width, jumpCheckHeight);
	if (jumpBox.top + jumpBox.height > window->getSize().y)
	{
		canJump = true;
	}
	else
	{
		for (auto* tile : tiles)
		{

			if (jumpBox.intersects(tile->getSprite().getGlobalBounds()))
			{
				canJump = true;
				break;

			}
			else
			{
				canJump = false;
			}
		}
	}
}

void Game::updateLevel(unsigned int points, int pointsPerLevel)
{
	int number = points % pointsPerLevel;
	if (number == 0)
	{
		velocityMin *= velocityIncrease;
	}
	
}

void Game::updateMusic()
{
	if (!music.getStatus() == sf::Music::Playing)
	{
		music.play();
	}
}




void Game::render()
{
	window->clear();

	renderBackground();
	if (gameOn == true)
	{
		renderPlayer();
		
	}
	renderMap();
	renderPoints();

	if (gameOn == false)
	{
		renderPlayerDeath();
	}

	renderWindow();
}

void Game::renderWindow()
{
	window->display();
}

void Game::renderBackground()
{
	window->draw(backgroundSprite);
}

void Game::renderMap()
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		tiles[i]->render(*window);
	}

	bottomTile->render(*window);

	for (int i = 0; i < fruits.size(); ++i)
	{
		fruits[i]->render(*window);
	}
}

void Game::renderPlayer()
{
	player->render(*window);
}


void Game::renderPoints()
{
	window->draw(pointText);
}

void Game::updateFruitSpawn()
{
	int chance = rand() % 10;
	if (chance == 0)
	{
		fruits.push_back(new GameTile("images/pinneapple.png", sf::Vector2f(1.f, 1.f), tiles[tiles.size() - 1]->getBounds().left + tiles[tiles.size() - 1]->getBounds().width / 2.f - 20.f, tiles[tiles.size() - 1]->getBounds().top - 20.f, velocityMin));

	}
}

void Game::renderPlayerDeath()
{
	window->draw(gameOverText);
}


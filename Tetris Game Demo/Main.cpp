#include <SFML/Graphics.hpp>
#include<iostream>
#include <vector>
#include <time.h>
#include <windows.h>

using namespace std;


struct Tetris
{
	sf::Sprite block[2][4];
	sf::Vector2f position;
	bool state;

};
vector<Tetris> blocks;
int main()
{
	srand(time(NULL));
	sf::Clock clock;
	float deltaTime;
	sf::RenderWindow window;
	sf::Texture blockTexture;
	blockTexture.loadFromFile("Sprite/blocks.png");
	sf::Vector2u blocksize = blockTexture.getSize();
	window.create(sf::VideoMode((blocksize.x / 8) * 10, (blocksize.x / 8) * 20), "Tetris Game", sf::Style::Default);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			blocks.push_back({ block[i][j].});
			block[i][j].setTextureRect(sf::IntRect(j * 32, i * 32, blocksize.x / 8, blocksize.y / 5));
		}
	}
	blocks.push_back(block[rand() % 2][rand() % 4]);
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
		for (size_t i = 0; i < blocks.size(); i++) {
			if (blocks[i].getPosition().y + blocks[i].getGlobalBounds().height < window.getSize().y) {
				blocks[i].move(0, 50 * deltaTime);
			}
		}
		window.clear();
		for (size_t i = 0; i < blocks.size(); i++) {
			window.draw(blocks[i]);
		}
		window.display();
	}

	return 0;
}

#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
float Colcheck = 0;
int main()
{
    srand(time(0));
    sf::Clock clock;
    sf::RenderWindow window; 
    sf::Texture blockTexture;
    blockTexture.loadFromFile("Sprite/blocks.png");
    sf::Vector2u blocksize = blockTexture.getSize();
    window.create(sf::VideoMode((blocksize.x/8)*10, (blocksize.x / 8)*20), "Tetris Game",sf::Style::Default);
    sf::Sprite block[8];
    block[0].setTexture(blockTexture);
    block[0].setTextureRect(sf::IntRect(0, 0, blocksize.x/8, blocksize.y/5));

    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();
        sf::Time t1=sf::milliseconds(1000);
        if (elapsed >= t1 && block[0].getPosition().y + 32 < 640) {
            block[0].move(sf::Vector2f(0.f, 16.f));
            clock.restart();
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && block[0].getPosition().y + 32 < 640)
                block[0].move(sf::Vector2f(0.f,16.f));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && block[0].getPosition().x > 0 && block[0].getPosition().y + 32 < 640)
                block[0].move(sf::Vector2f(-16.f, 0.f));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && block[0].getPosition().x + 32 < 320 && block[0].getPosition().y + 32 < 640)
                block[0].move(sf::Vector2f(16.f, 0.f));
        }
        window.clear();
        window.draw(block[0]);
        window.display();
    }

    return 0;
}

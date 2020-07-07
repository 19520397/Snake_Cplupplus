#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using std::cout;

class Credit
{
private:
    sf::RenderWindow* window;
    sf::Font* font;
    sf::Text* txt;

    float height;
    float width;
public:

    Credit(sf::RenderWindow* window, float width, float height) : window(window), width(width), height(height)
    {
        font = new sf::Font();
        if (!font->loadFromFile("Fonts/manaspc.ttf"))
        {
            cout << "ERROR: Could not load font";
        }
        txt = new sf::Text();
        txt->setFont(*font);
    }

	void load()
    {
        window->clear();

        
        txt->setFillColor(sf::Color::Green);
        txt->setCharacterSize(32);

        txt->setPosition(50, 50);
        txt->setString(">Esc<");
        window->draw(*txt);
        txt->setString("This is exercise for class SS004.K22");
        txt->setPosition(window->getSize().x / 2 - txt->getGlobalBounds().width / 2, height * 3 / 4 - txt->getGlobalBounds().height);
        window->draw(*txt);
        txt->setString("~ VNUHCM ~ UIT ~");
        txt->setPosition(window->getSize().x / 2 - txt->getGlobalBounds().width / 2, txt->getPosition().y + txt->getGlobalBounds().height + 15);
        window->draw(*txt);

        txt->setCharacterSize(50);
        txt->setFillColor(sf::Color::Yellow);
        txt->setString("Authors :\n ~ Nguyen Pham Duy Bang - Dragnell\n ~ Nguyen Trong Nhan - C0R3M3N\n ~ Vu Ngoc Thach - DragonPow");
        txt->setPosition(100, height / 3 - txt->getGlobalBounds().height);
        window->draw(*txt);

        window->display();
        while (window->isOpen() && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            // Wait until user want to return
            sf::Event e;
            while (window->pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                    window->close();
            }
        }
        cout << "Back to Main Menu\n";
        return;
    }
};


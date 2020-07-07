#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using sf::Text;
using sf::Font;
using sf::RenderWindow;
using sf::Sound;
using sf::SoundBuffer;
using sf::Keyboard;
using sf::Color;
using std::vector;

class MainMenu
{
private:
    int op = 1;
public:
    float width;
    float height;

    RenderWindow* window;
    Text* txt_title;
    Text* txt_selection;
    Text* txt_option;
    Sound* sound;
    SoundBuffer* buffer;
    Font* font_arcade;
    Font* font_manaspc;
    vector<std::string>* options;

    MainMenu(RenderWindow* window, float width, float height);
    ~MainMenu();

    int load();
    int getOption() { return op; }
    std::string getOption_Key() { return options->at(op); }
};


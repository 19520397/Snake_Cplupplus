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
using sf::Sprite;
using std::vector;

class Selection
{

private:
    int op = 1;

    Font* font_arcade;
    Font* font_manaspc;

    Text* txt_title;
    Text* txt_selection;
    Text* txt_option;

    Sound* sound;
    SoundBuffer* buffer;

    Sprite* sprite_backGround = NULL;
public:
    float width;
    float height;
    float top = 0;
    float left = 0;

    RenderWindow* window;
    vector<std::string>* options;

    Selection(RenderWindow* window, float width, float heigh, float top = 0, float left = 0, sf::Texture* background = NULL);
    ~Selection();

    int load();
    void setOptions_List(vector<std::string>* options);

    int getOption() { return op; }
    std::string getOption_Key() { return options->at(op); }

    void setOption_Size(int size) { txt_option->setCharacterSize(size); txt_selection->setCharacterSize(size); };
    void setOption_Color(sf::Color color) { txt_option->setFillColor(color); }

    void setTitle_String(sf::String str);
    void setTitle_Size(int size);
    void setTitle_Color(sf::Color color) { txt_title->setFillColor(color); }
    
};


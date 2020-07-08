#include "GameManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "MainMenu.h"
#include "Selection.h"
#include "Screen_Classic.h"
#include "Credit.h"

using namespace sf;

using std::cin;
using std::cout;

void GameManager::start()
{
    RenderWindow window(VideoMode(width_board + width_UI, height_board), "Snake Game!", Style::Titlebar | Style::Close);
    window.setPosition(Vector2i(VideoMode::getDesktopMode().width / 2 - window.getSize().x / 2, VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));

    Selection* mainMenu = new Selection(&window, width_board + width_UI, height_board);
    mainMenu->setTitle_String("Snake Game");
    mainMenu->setTitle_Color(Color::Green);
    mainMenu->setTitle_Size(120);
    mainMenu->setOptions_List(new vector<std::string>{"Quit", "Credit", "High Score", "Start"});
    mainMenu->setOption_Color(Color::Yellow);
    mainMenu->setOption_Size(80);
    mainMenu->load();

    while (mainMenu->getOption() != 0)
    {
        if (mainMenu->getOption() == 1)
        {
            cout << "Launch Credit\n";

            Credit* credit = new Credit(&window, width_board + width_UI, height_board);

            credit->load();

            delete credit;
        }
        else if (mainMenu->getOption() == 2)
        {
            cout << "Launch High Score Board\n";
        }
        else if (mainMenu->getOption() == 3)
        {
            cout << "Launch Screen Classic\n";

            Screen_Classic* screen_classic = new Screen_Classic(&window, N, M, img_size, width_UI);

            screen_classic->start();

            delete screen_classic;
        }
        else cout << "ERROR: Out of range\n";
        if (!window.isOpen())
        {
            cout << "Closed window\n";
            break;
        }
        else mainMenu->load();
    }

    if (!window.isOpen()) { window.close(); }
    delete mainMenu;
}

/*int main()
{
    GameManager gameManager;
    gameManager.start();
}*/
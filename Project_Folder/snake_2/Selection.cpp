#include "Selection.h"

Selection::Selection(RenderWindow* window, float width, float heigh, float top, float left, sf::Texture* background) : window(window), top(top), left(left), height(heigh - top), width(width - left)
{
    if (window == NULL) throw std::invalid_argument("Null pointer");
    else { this->window = window; }

    if (background != NULL) sprite_backGround = new Sprite(*background);

    txt_title = new Text();
    txt_selection = new Text();
    txt_option = new Text();

    sound = new Sound();
    buffer = new SoundBuffer();

    font_arcade = new Font();
    font_manaspc = new Font();

    options = new vector<std::string>{ "Quit", "Credit", "Start" };
    op = options->size() - 1;

    buffer->loadFromFile("audio/beforegame.ogg");
    sound->setBuffer(*buffer);
    sound->setVolume(30.f);

    if (!font_arcade->loadFromFile("Fonts/ARCADECLASSIC.TTF")) { cout << "ERROR: Could not load font"; }
    if (!font_manaspc->loadFromFile("Fonts/manaspc.ttf")) { cout << "ERROR: Could not load font"; }

    txt_title->setFont(*font_arcade);
    txt_title->setFillColor(Color::Green);
    txt_title->setCharacterSize(72);
    txt_title->setString("Snake Game");
    txt_title->setPosition( left + width / 2 - txt_title->getGlobalBounds().width / 2, top + height / 3 - txt_title->getGlobalBounds().height / 2);

    txt_option->setFont(*font_arcade);
    txt_option->setFillColor(Color::Yellow);
    txt_option->setCharacterSize(50);

    txt_selection->setFont(*font_manaspc);
    txt_selection->setFillColor(sf::Color::White);
    txt_selection->setString("> ");
    txt_selection->setCharacterSize(txt_option->getCharacterSize());
}

Selection::~Selection()
{
    delete txt_option;
    delete txt_selection;
    delete txt_title;
    delete sound;
    delete buffer;
    delete font_arcade;
    delete font_manaspc;
    delete options;
}

int Selection::load()
{
    bool isChanged = true;

    while (window->isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            while (Keyboard::isKeyPressed(Keyboard::Enter)) {}
            break;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (op < options->size() - 1) op++;
            else op = 0;
            isChanged = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (op > 0) op--;
            else op = options->size() - 1;
            isChanged = true;
        }

        sf::Event e;
        while (window->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window->close();
        }

        if (isChanged)
        {
            window->clear();

            sound->play();

            if (sprite_backGround != NULL) window->draw(*sprite_backGround);

            window->draw(*txt_title);

            txt_option->setString(options->at(options->size() - 1));
            txt_option->setPosition(txt_title->getPosition().x + txt_title->getGlobalBounds().width * 0.2f, txt_title->getPosition().y + txt_title->getGlobalBounds().height * 1.5f);
            window->draw(*txt_option);

            if (op == options->size() - 1)
            {
                txt_selection->setPosition(txt_option->getPosition().x - txt_selection->getGlobalBounds().width, txt_option->getPosition().y);
                window->draw(*txt_selection);
            }

            for (int i = options->size() - 2; i > -1; i--)
            {
                txt_option->setString(options->at(i)); // Here are some magic
                txt_option->setPosition(txt_option->getPosition().x, txt_option->getPosition().y + txt_option->getGlobalBounds().height * 1.3);
                window->draw(*txt_option);

                if (op == i)
                {
                    txt_selection->setPosition(txt_option->getPosition().x - txt_selection->getGlobalBounds().width, txt_option->getPosition().y);
                    window->draw(*txt_selection);
                }
            }

            window->display();
            while (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down))
            {
                // wait for key up
            }
            isChanged = false;
        }

    }

    if (window->isOpen())
    {
        return op - 1;
    }
    else return 0;
}

void Selection::setOptions_List(vector<std::string>* options)
{
    if (options == NULL) throw std::invalid_argument("Null pointer");
    else
    {
        this->options = options;
        op = options->size() - 1;
    }
}

void Selection::setTitle_String(sf::String str) 
{ 
    txt_title->setString(str); 
    txt_title->setPosition(left + width / 2 - txt_title->getGlobalBounds().width / 2, top + height / 3 - txt_title->getGlobalBounds().height / 2);
}
void Selection::setTitle_Size(int size) 
{ 
    txt_title->setCharacterSize(size); 
    txt_title->setPosition(left + width / 2 - txt_title->getGlobalBounds().width / 2, top + height / 3 - txt_title->getGlobalBounds().height / 2);
}

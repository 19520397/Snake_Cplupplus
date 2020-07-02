#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string>

using namespace sf;
using std::cin;
using std::cout;

bool isAlive = false;

enum DIRECTION
{
    DIRECTION_RIGHT, DIRECTION_UP, DIRECTION_LEFT, DIRECTION_DOWN
};

int score = 0;

// Width and Height of window in grid
int N = 40, M = 40;

// Size of image in pixels
float img_size = 24;

// Width of board in pixels
float width_board = img_size * N;
// Height of board in pixels
float height_board = img_size * M;

// Width of UI space in pixels
float width_UI = img_size * 10;
// Height of UI space in pixels
float height_UI = height_board;


DIRECTION direction;
int length;

struct Snake
{
    int x, y;
};

struct Fruit
{
    int x, y;
};

void loadScreen();
int start(RenderWindow&);
void update(Snake*&, Fruit&, Sound&);
void gameOver();
int show_credit(RenderWindow&);
int checkPlayAgain(RenderWindow&);
int checkResume(RenderWindow&);

void update(Snake*& s, Fruit& f, Sound& sound)
{
    // Update snake's body position
    for (int i = length; i > 0; --i)
    {
        s[i].x = s[i - 1].x; 
		s[i].y = s[i - 1].y;
    }

    // Update snake's head position
    if (direction == DIRECTION_UP) s[0].y -= 1;
    else if (direction == DIRECTION_LEFT) s[0].x -= 1;
    else if (direction == DIRECTION_RIGHT) s[0].x += 1;
    else if (direction == DIRECTION_DOWN) s[0].y += 1;

    // Check whether snake eat food
    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        f.x = rand() % (N - 2) + 1; // Range is 1-> N-2
        f.y = rand() % (M - 2) + 1; // Range is 1-> M-2

        for (int i = 0; i < length; i++)
        {
            if (s[i].x == f.x && s[i].y == f.y)
            {
                f.x = rand() % (N - 2) + 1; // Range is 1-> N-2
                f.y = rand() % (M - 2) + 1; // Range is 1-> M-2
                i = 0;
            }
        }
        sound.play();
        score+=10;
        length++;
    }

    // Check whether snake's head has collision with border
    if (s[0].x >= N-1) // Hit right border
    {
        cout << "Hit right border\n";
        gameOver();
    }
    if (s[0].x <= 0) // Hit left border
    {
        cout << "Hit left border\n";
        gameOver();
    }
    if (s[0].y >= M-1) // Hit bottom border
    {
        cout << "Hit bottom border\n";
        gameOver();
    }
    if (s[0].y <= 0) // Hit top border
    {
        //s[0].y = M-1;
        cout << "Hit top border\n";
        gameOver();
    }

    // Check whether snake's head hit its-self
    for (int i = 1; i < length; i++)
    {
        if (s[0].x == s[i].x && s[0].y == s[i].y)
        {
            //length = i;
            cout << "Hit its-self\n";
            gameOver();
        }
    }
}

int checkPlayAgain(RenderWindow& window)
{
    int op = 1;
    Texture t;
    t.create(window.getSize().x, window.getSize().y);
    t.update(window);
    Sprite last_state(t);
    Font font;

    Sound sound_die,sound_choose;
    SoundBuffer buffer_die,buffer_choose;
    buffer_die.loadFromFile("audio/message.ogg");
    buffer_choose.loadFromFile("audio/beforegame.ogg");
    sound_die.setBuffer(buffer_die);
    sound_die.setVolume(50.f);
    sound_choose.setBuffer(buffer_choose);
    sound_choose.setVolume(50.f);

    if (!font.loadFromFile("Fonts/ARCADECLASSIC.TTF"))
    {
        cout << "ERROR: Could not load font";
    }
    Text txt;
    txt.setFont(font);

    Font font2;
    if (!font2.loadFromFile("Fonts/manaspc.ttf"))
    {
        cout << "ERROR: Could not load font";
    }
    Text txt2;
    txt2.setFont(font2);
    txt2.setFillColor(Color::Black);
    txt2.setString(">");
    txt2.setCharacterSize(50);

    bool isChanged = true;
    sound_die.play();
    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            while (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                // wait to key up
            }
            break;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (op < 1) op++;
            isChanged = true;
            //while (Keyboard::isKeyPressed(Keyboard::Up)) {}
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (op > 0) op--;
            isChanged = true;
            //while (Keyboard::isKeyPressed(Keyboard::Down)) {}
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (isChanged)
        {
            window.clear();
            window.draw(last_state);
            sound_choose.play();

            txt.setFillColor(Color::Red);
            txt.setCharacterSize(150);
            txt.setString("You Died");
            txt.setPosition((width_board + width_UI) / 2 - txt.getGlobalBounds().width / 2, height_board / 3 - txt.getGlobalBounds().height / 2);
            window.draw(txt);

            float temp = txt.getPosition().y + txt.getGlobalBounds().height + 50;
            txt.setCharacterSize(txt2.getCharacterSize());
            txt.setFillColor(Color::Blue);
            txt.setString("Play again");
            txt.setPosition(txt.getPosition().x + 80, temp);
            window.draw(txt);

            if (op == 1)
            {
                txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                window.draw(txt2);
            }

            txt.setString("Back");
            txt.setPosition(txt.getPosition().x, txt.getPosition().y + txt.getGlobalBounds().height + 15);
            window.draw(txt);

            if (op == 0)
            {
                txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                window.draw(txt2);
            }

            window.display();
            while (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down))
            {
                // wait for key up
            }
            isChanged = false;
        }

    }

    if (op == 1)
    {
        cout << "Play Again\n";
        return 1;
    }
    else
    {
        cout << "Back\n";
        return -1;
    }
}

int checkResume(RenderWindow& window)
{
    int op = 2;

#pragma region Initialize

#pragma endregion

    Texture t;
    t.create(window.getSize().x, window.getSize().y);
    t.update(window);
    t.update(window);
    Sprite last_state(t);
    Font font;

    Sound sound_choose;
    SoundBuffer buffer_choose;
    buffer_choose.loadFromFile("audio/beforegame.ogg");
    sound_choose.setBuffer(buffer_choose);
    sound_choose.setVolume(50.f);

    if (!font.loadFromFile("Fonts/ARCADECLASSIC.TTF"))
    {
        cout << "ERROR: Could not load font";
    }
    Text txt;
    txt.setFont(font);

    Font font2;
    if (!font2.loadFromFile("Fonts/manaspc.ttf"))
    {
        cout << "ERROR: Could not load font";
    }
    Text txt2;
    txt2.setFont(font2);
    txt2.setFillColor(Color::Black);
    txt2.setString(">");
    txt2.setCharacterSize(50);

    bool isChanged = true;
    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            while (Keyboard::isKeyPressed(Keyboard::Enter)) {/*Wait to key up*/}
            break;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (op < 2) op++;
            isChanged = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (op > 0) op--;
            isChanged = true;
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (isChanged)
        {
            window.clear();
            window.draw(last_state);
            sound_choose.play();

            txt.setFillColor(Color::Red);
            txt.setCharacterSize(150);
            txt.setString("Paused");
            txt.setPosition((width_board + width_UI) / 2 - txt.getGlobalBounds().width / 2, height_board / 3 - txt.getGlobalBounds().height / 2);
            window.draw(txt);

            float temp = txt.getPosition().y + txt.getGlobalBounds().height + 50;
            txt.setCharacterSize(txt2.getCharacterSize());
            txt.setFillColor(Color::Blue);
            txt.setString("Resume");
            txt.setPosition(txt.getPosition().x + 80, temp);
            window.draw(txt);

            if (op == 2)
            {
                txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                window.draw(txt2);
            }

            txt.setString("Main Menu");
            txt.setPosition(txt.getPosition().x, txt.getPosition().y + txt.getGlobalBounds().height + 15);
            window.draw(txt);

            if (op == 1)
            {
                txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                window.draw(txt2);
            }

            txt.setString("Quit");
            txt.setPosition(txt.getPosition().x, txt.getPosition().y + txt.getGlobalBounds().height + 15);
            window.draw(txt);

            if (op == 0)
            {
                txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                window.draw(txt2);
            }

            window.display();
            while (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down))
            {
                // wait for key up
            }
            isChanged = false;
        }

    }

    if (op == 2)
    {
        cout << "Resume\n";
        return 2;
    }
    else if (op == 1)
    {
        cout << "Main Menu\n";
        return 1;
    }
    else
    {
        cout << "Back\n";
        return 0;
    }
}

int start(RenderWindow& window)
{
    int op = 1;
    int MAX = (N - 2) * (M - 2);

#pragma region Initialize
    Sound sound_eatfood;
    SoundBuffer buffer;
    buffer.loadFromFile("audio/eatfood.ogg");
    sound_eatfood.setBuffer(buffer);
    sound_eatfood.setVolume(50.f);

    Snake* s = new Snake[MAX];
    Fruit f;

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/white.png");
    t2.loadFromFile("images/red.png");
    t3.loadFromFile("images/purble.jpg");
    t4.loadFromFile("images/green_2.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);
    Sprite sprite4(t4);
    sprite1.scale(1.5f, 1.5f);
    sprite2.scale(1.5f, 1.5f);
    sprite3.scale(1.5f, 1.5f);
    sprite4.scale(1.5f, 1.5f);

    Font font_arcade;
    if (!font_arcade.loadFromFile("Fonts/ARCADECLASSIC.TTF"))
    {
        cout << "ERROR: Could not load font";
    }

    Text txt_score;
    txt_score.setFont(font_arcade);
    txt_score.setFillColor(Color::White);
    txt_score.setCharacterSize(32);
    txt_score.setString("Point");
    txt_score.setPosition(width_board + width_UI / 2 - txt_score.getLocalBounds().width / 2, height_UI / 2 - 32);

    Text txt;
    txt.setFont(font_arcade);
    txt.setFillColor(Color::Magenta);
    txt.setCharacterSize(32);
    txt.setPosition(txt_score.getPosition().x + txt_score.getLocalBounds().width / 2 - 16, txt_score.getPosition().y + txt_score.getGlobalBounds().height + 16);
#pragma endregion

    while (op == 1)
    {
        srand(time(0));

        isAlive = true;

        Clock clock;
        float timer = 0, delay = 0.1;
        
        score = 0;
        length = 3;
        for (int i = 0; i < length; i++)
        {
            s[i].x = 15;
            s[i].y = 15;
        }
        timer = delay * 1.1;
        f.x = 10;
        f.y = 10;

        DIRECTION temp_d = DIRECTION::DIRECTION_RIGHT;
        direction = temp_d;

        while (window.isOpen() && isAlive)
        {
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                    return -1;
                }
            }

#pragma region Get Input
            if (Keyboard::isKeyPressed(Keyboard::Left)) temp_d = DIRECTION_LEFT;
            else if (Keyboard::isKeyPressed(Keyboard::Right)) temp_d = DIRECTION_RIGHT;
            else if (Keyboard::isKeyPressed(Keyboard::Up)) temp_d = DIRECTION_UP;
            else if (Keyboard::isKeyPressed(Keyboard::Down)) temp_d = DIRECTION_DOWN;
            // Check pause keyboard
            if (Keyboard::isKeyPressed(Keyboard::Escape))
            {
                while (Keyboard::isKeyPressed(Keyboard::Escape)) {/*Wait for key up*/ }
#pragma region Draw

                window.clear();

                // Draw background
                for (int x = width_board - img_size; x > -1; x -= img_size) { sprite3.setPosition(x, 0);  window.draw(sprite3); } // Draw top Border
                for (int y = height_board - 2 * img_size; y > 0; y -= img_size)
                {
                    sprite3.setPosition(0, y);  window.draw(sprite3); // Draw left Border
                    for (int x = width_board - 2 * img_size; x > 0; x -= img_size)
                    {
                        sprite1.setPosition(x, y);  window.draw(sprite1);
                    }
                    sprite3.setPosition(width_board - img_size, y);  window.draw(sprite3); // Draw right Border
                }
                for (int x = width_board - img_size; x > -1; x -= img_size) { sprite3.setPosition(x, height_board - img_size);  window.draw(sprite3); } // Draw bottom Border

                // Draw snake
                for (int i = 0; i < length; i++)
                {
                    sprite2.setPosition(s[i].x * img_size, s[i].y * img_size);  window.draw(sprite2);
                }

                // Draw food
                sprite4.setPosition(f.x * img_size, f.y * img_size);  window.draw(sprite4);

                // Draw UI
                txt.setString(std::to_string(score)); window.draw(txt); window.draw(txt_score);

                window.display();
#pragma endregion
                int sl = checkResume(window);
                if (sl == 0) return -1; // Quit
                else if (sl == 1)// main menu
                {
                    delete[] s;
                    return 2;
                }
                else {} // Resume
            }
#pragma endregion

            if (timer > delay)
            {
                if (!((temp_d == DIRECTION_LEFT && direction == DIRECTION_RIGHT)
                    || (temp_d == DIRECTION_RIGHT && direction == DIRECTION_LEFT)
                    || (temp_d == DIRECTION_UP && direction == DIRECTION_DOWN)
                    || (temp_d == DIRECTION_DOWN && direction == DIRECTION_UP)))
                {
                    direction = temp_d;
                }

                // Delay time has reached, reset timer
                timer = 0;
                // Update position of object 
                update(s, f, sound_eatfood);

#pragma region Draw

                window.clear();

                // Draw background
                for (int x = width_board - img_size; x > -1; x -= img_size) { sprite3.setPosition(x, 0);  window.draw(sprite3); } // Draw top Border
                for (int y = height_board - 2 * img_size; y > 0; y -= img_size)
                {
                    sprite3.setPosition(0, y);  window.draw(sprite3); // Draw left Border
                    for (int x = width_board - 2 * img_size; x > 0; x -= img_size)
                    {
                        sprite1.setPosition(x, y);  window.draw(sprite1);
                    }
                    sprite3.setPosition(width_board - img_size, y);  window.draw(sprite3); // Draw right Border
                }
                for (int x = width_board - img_size; x > -1; x -= img_size) { sprite3.setPosition(x, height_board - img_size);  window.draw(sprite3); } // Draw bottom Border

                // Draw snake
                for (int i = 0; i < length; i++)
                {
                    sprite2.setPosition(s[i].x * img_size, s[i].y * img_size);  window.draw(sprite2);
                }

                // Draw food
                sprite4.setPosition(f.x * img_size, f.y * img_size);  window.draw(sprite4);

                // Draw UI
                txt.setString(std::to_string(score)); window.draw(txt); window.draw(txt_score);

                window.display();
#pragma endregion
            }
            else
            {
                // Wait delay time
                
            }

        }
        if (window.isOpen()) {
            if (!isAlive)
            {
                // Player is died, ask if player want to try again
#pragma region Draw

                window.clear();

                // Draw background
                for (int x = width_board - img_size; x > -1; x -= img_size) { sprite3.setPosition(x, 0);  window.draw(sprite3); } // Draw top Border
                for (int y = height_board - 2 * img_size; y > 0; y -= img_size)
                {
                    sprite3.setPosition(0, y);  window.draw(sprite3); // Draw left Border
                    for (int x = width_board - 2 * img_size; x > 0; x -= img_size)
                    {
                        sprite1.setPosition(x, y);  window.draw(sprite1);
                    }
                    sprite3.setPosition(width_board - img_size, y);  window.draw(sprite3); // Draw right Border
                }
                for (int x = width_board - img_size; x > -1; x -= img_size) { sprite3.setPosition(x, height_board - img_size);  window.draw(sprite3); } // Draw bottom Border

                // Draw snake
                for (int i = 0; i < length; i++)
                {
                    sprite2.setPosition(s[i].x * img_size, s[i].y * img_size);  window.draw(sprite2);
                }

                // Draw food
                sprite4.setPosition(f.x * img_size, f.y * img_size);  window.draw(sprite4);

                // Draw UI
                txt.setString(std::to_string(score)); window.draw(txt); window.draw(txt_score);

                window.display();
#pragma endregion
                op = checkPlayAgain(window);
            }
            else
            {
                cout << " Not died yet but the start()'s loop has been break?\n";
                delete[] s;
                return 2;
            }
        }
        else
        {
            delete[] s;
            return -1;
        }
    }
    delete[] s;
    return 2;
}

void gameOver()
{
    cout << "Game Over\n";
    isAlive = false;
	//system("pause");
}

int show_credit(RenderWindow& window)
{
    window.clear();

    Font font;
    if (!font.loadFromFile("Fonts/manaspc.ttf"))
    {
        cout << "ERROR: Could not load font";
    }
    Text txt;
    txt.setFont(font);
    txt.setFillColor(Color::Green);
    txt.setCharacterSize(32);

    txt.setPosition(50, 50);
    txt.setString(">Esc<");
    window.draw(txt);
    txt.setString("This is exercise for class SS004.K22");
    txt.setPosition(window.getSize().x / 2 - txt.getGlobalBounds().width / 2, height_board * 3 / 4 - txt.getGlobalBounds().height);
    window.draw(txt);
    txt.setString("~ VNUHCM ~ UIT ~");
    txt.setPosition(window.getSize().x / 2 - txt.getGlobalBounds().width / 2, txt.getPosition().y +  txt.getGlobalBounds().height + 15);
    window.draw(txt);

    txt.setFillColor(Color::Yellow);
    txt.setString("Authors :\n ~ Nguyen Pham Duy Bang - Dragnell\n ~ Nguyen Trong Nhan - C0R3M3N\n ~ Vu Ngoc Thach - DragonPow");
    txt.setPosition(100, height_board/3 - txt.getGlobalBounds().height);
    window.draw(txt);

    window.display();
    while (window.isOpen() && !(Keyboard::isKeyPressed(Keyboard::Escape)))
    {
        // Wait until user want to return
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }
    }

    if (window.isOpen()) return 2;
    else return -1;
}

void loadScreen()
{
    RenderWindow window(VideoMode(width_board + width_UI, height_board), "Snake Game!", Style::Titlebar | Style::Close);
    window.setPosition(Vector2i(15, 15));
    float width = width_board + width_UI;
    int op = 2;
    while (op > 0)
    {
        {
            Sound sound;
            SoundBuffer buffer;
            buffer.loadFromFile("audio/beforegame.ogg");
            sound.setBuffer(buffer);
            sound.setVolume(30.f);
            Font font;

            if (!font.loadFromFile("Fonts/ARCADECLASSIC.TTF"))
            {
                cout << "ERROR: Could not load font";
            }
            Text txt;
            txt.setFont(font);

            Font font2;
            if (!font2.loadFromFile("Fonts/manaspc.ttf"))
            {
                cout << "ERROR: Could not load font";
            }
            Text txt2;
            txt2.setFont(font2);
            txt2.setFillColor(Color::White);
            txt2.setString(">");
            txt2.setCharacterSize(50);

            bool isChanged = true;
            while (window.isOpen())
            {
                if (Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    break;
                }
                else if (Keyboard::isKeyPressed(Keyboard::Up))
                {
                    if (op < 2) op++;
                    isChanged = true;
                    //while (Keyboard::isKeyPressed(Keyboard::Up)) {}
                }
                else if (Keyboard::isKeyPressed(Keyboard::Down))
                {
                    if (op > 0) op--;
                    isChanged = true;
                    //while (Keyboard::isKeyPressed(Keyboard::Down)) {}
                }

                Event e;
                while (window.pollEvent(e))
                {
                    if (e.type == Event::Closed)
                        window.close();
                }

                if (isChanged)
                {
                    window.clear();
                    sound.play();

                    txt.setFillColor(Color::Green);
                    txt.setCharacterSize(72);
                    txt.setString("Snake Game");
                    txt.setPosition(width / 2 - txt.getGlobalBounds().width / 2, height_board / 3 - txt.getGlobalBounds().height / 2);
                    window.draw(txt);

                    float temp = txt.getPosition().y + txt.getGlobalBounds().height + 30;
                    txt.setCharacterSize(50);
                    txt.setFillColor(Color::Yellow);
                    txt.setString("Start");
                    txt.setPosition(width / 2 - txt.getGlobalBounds().width / 2, temp);
                    window.draw(txt);

                    if (op == 2)
                    {
                        txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                        window.draw(txt2);
                    }

                    txt.setString("Credit");
                    txt.setPosition(txt.getPosition().x, txt.getPosition().y + txt.getGlobalBounds().height + 15);
                    window.draw(txt);

                    if (op == 1)
                    {
                        txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                        window.draw(txt2);
                    }

                    txt.setString("Quit");
                    txt.setPosition(txt.getPosition().x, txt.getPosition().y + txt.getGlobalBounds().height + 15);
                    window.draw(txt);

                    if (op == 0)
                    {
                        txt2.setPosition(txt.getPosition().x - txt2.getGlobalBounds().width - 10, txt.getPosition().y);
                        window.draw(txt2);
                    }

                    window.display();
                    while (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down))
                    {
                        // wait for key up
                    }
                    isChanged = false;
                }

            }
        }

        if (op == 2) op = start(window);
        else if (op == 1) op = show_credit(window);
    }
}

int main()
{
    loadScreen();

    return 0;
}

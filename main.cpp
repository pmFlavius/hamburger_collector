#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace sf;
using namespace std;

struct ObiectRandom
{
    CircleShape forma;
    bool active;
};

int score = 0;
int timp = 0;
bool timer = true;
bool jocterminat = false;
Mutex scoreMutex;
Mutex timeMutex;

void gameTimer()
{
    while (timer)
    {
        sleep(seconds(1));
        timeMutex.lock();
        timp++;
        timeMutex.unlock();
    }
}

ObiectRandom createObiect(Texture &texture)
{
    ObiectRandom obj;
    obj.forma.setRadius(40);
    obj.forma.setPointCount(30);
    obj.forma.setTexture(&texture);

    int randomX = rand() % (800 - 60);
    int randomY = rand() % (600 - 60);
    obj.forma.setPosition(randomX, randomY);
    obj.active = true;

    return obj;
}

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(800, 600), "Stefan il mananca pe Razvan!", Style::Default);
    window.setPosition(Vector2i(10, 50));
    window.setVerticalSyncEnabled(true);

    Texture textura;
    textura.loadFromFile("gura.jpg");
    textura.setSmooth(true);

    CircleShape cerc(60);
    cerc.setPointCount(100);
    cerc.setTexture(&textura);
    cerc.setPosition(380, 600 - cerc.getRadius() * 2);

    float viteza = 0;
    float gravitatie = 0.5;
    bool inAer = true;
    float coeficientBounce = 0.75;
    int jumpCount = 0;
    const int maxJumps = 2;
    int diametru = cerc.getRadius() * 2;

    float vitezaOrizontala = 0;
    float acceleratie = 0.5;
    float frecare = 0.92;
    float vitezaMaxima = 8.0;

    Texture texturaColectabil;
    texturaColectabil.loadFromFile("burger.jpg");
    texturaColectabil.setSmooth(true);
    ObiectRandom colectabil = createObiect(texturaColectabil);

    Font font;
    font.loadFromFile("arial.ttf");

    Text textscore;
    textscore.setFont(font);
    textscore.setCharacterSize(30);
    textscore.setFillColor(Color::White);
    textscore.setPosition(10, 10);

    Text timetext;
    timetext.setFont(font);
    timetext.setCharacterSize(30);
    timetext.setFillColor(Color::White);
    timetext.setPosition(10, 70);

    Thread timerThread(&gameTimer);
    timerThread.launch();

    while (window.isOpen())
    {
        int y = cerc.getPosition().y;
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                timer = false, window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                if (jumpCount < maxJumps)
                {
                    viteza = -20.0;
                    jumpCount++;
                    inAer = true;
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            vitezaOrizontala -= acceleratie;
            if (vitezaOrizontala < -vitezaMaxima)
                vitezaOrizontala = -vitezaMaxima;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            vitezaOrizontala += acceleratie;
            if (vitezaOrizontala > vitezaMaxima)
                vitezaOrizontala = vitezaMaxima;
        }
        else
        {
            vitezaOrizontala *= frecare;
            if (abs(vitezaOrizontala) < 0.1)
                vitezaOrizontala = 0;
        }

        if (inAer)
        {
            viteza += gravitatie;
        }

        cerc.move(vitezaOrizontala, viteza);

        float x = cerc.getPosition().x;
        if (x < 0)
        {
            cerc.setPosition(0, cerc.getPosition().y);
            vitezaOrizontala *= -0.5;
        }
        else if (x + diametru > 800)
        {
            cerc.setPosition(800 - diametru, cerc.getPosition().y);
            vitezaOrizontala *= -0.5;
        }

        y = cerc.getPosition().y;
        if (y + diametru >= 600)
        {
            cerc.setPosition(cerc.getPosition().x, 600 - diametru);
            viteza = viteza * -coeficientBounce;

            if (abs(viteza) < 1.0)
            {
                viteza = 0;
                jumpCount = 0;
                inAer = false;
            }
            else
            {
                inAer = true;
            }
        }
        else
        {
            inAer = true;
        }

        if (colectabil.active)
        {
            FloatRect cercBounds = cerc.getGlobalBounds();
            FloatRect objBounds = colectabil.forma.getGlobalBounds();

            if (cercBounds.intersects(objBounds))
            {
                scoreMutex.lock();
                score += 10;
                scoreMutex.unlock();
                if (score == 100)
                {
                    jocterminat = true;
                    timer=0;
                }
                colectabil = createObiect(texturaColectabil);
            }
        }

        scoreMutex.lock();
        textscore.setString("Score: " + to_string(score));
        scoreMutex.unlock();

        timeMutex.lock();
        timetext.setString("Timp: " + to_string(timp));
        timeMutex.unlock();

        window.clear(Color::Black);

        if (jocterminat)
        {
            textscore.setString("Ai mancat toti burgerii! GG!");
            window.draw(textscore);
        }
        else
        {
            if (colectabil.active)
                window.draw(colectabil.forma);
            window.draw(cerc);
            window.draw(textscore);
            window.draw(timetext);
        }
        window.display();
    }
    timer = false;
    return 0;
}
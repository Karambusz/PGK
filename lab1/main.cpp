// Calosc mozna dowolnie edytowac wedle uznania. 

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "font.h"
#include <iostream>

class Menu : public sf::Drawable
{
private:
    sf::Font font;
    sf::Text* text;
    sf::RectangleShape* rectangle;

    sf::Texture* colors_texture;
    sf::Sprite* colors_sprite;
    sf::Uint8* colors_pixels;
    const unsigned int colors_size_x = 765;
    const unsigned int colors_size_y = 60;
    inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
    {
        colors_pixels[4 * (y * colors_size_x + x) + 0] = r;
        colors_pixels[4 * (y * colors_size_x + x) + 1] = g;
        colors_pixels[4 * (y * colors_size_x + x) + 2] = b;
        colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
    }

public:
    Menu()
    {
        font.loadFromMemory(font_data, font_data_size);
        text = new sf::Text;
        text->setFont(font);
        text->setCharacterSize(12);
        text->setFillColor(sf::Color::White);

        rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
        rectangle->setFillColor(sf::Color::Transparent);
        rectangle->setOutlineColor(sf::Color::White);
        rectangle->setOutlineThickness(1.0f);
        rectangle->setPosition(2, 62);

        unsigned int x, y;
        colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
        for (x = 0; x < 255; x++)
            for (y = 0; y < 30; y++)
            {
                draw_to_color_pixels(x, y, x, 255, 0);
                draw_to_color_pixels(x + 255, y, 255, 255 - x, 0);
                draw_to_color_pixels(x + 510, y, 255, 0, x);
                draw_to_color_pixels(254 - x, y + 30, 0, 255, 255 - x);
                draw_to_color_pixels(509 - x, y + 30, 0, x, 255);
                draw_to_color_pixels(764 - x, y + 30, 255 - x, 0, 255);
            }

        colors_texture = new sf::Texture();
        colors_texture->create(colors_size_x, colors_size_y);
        colors_texture->update(colors_pixels);

        colors_sprite = new sf::Sprite();
        colors_sprite->setTexture(*colors_texture);
        colors_sprite->setPosition(1, 1);
    }

    void outtextxy(sf::RenderTarget& target, float x, float y, const wchar_t* str) const
    {
        text->setPosition(x, y);
        text->setString(str);
        target.draw(*text);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        outtextxy(target, 5, 600, L"f - wybуr koloru rysowania");
        outtextxy(target, 5, 615, L"b - wybуr koloru wypeіniania");
        outtextxy(target, 5, 630, L"l - rysowanie linii");

        outtextxy(target, 200, 600, L"r - rysowanie prostok№ta");
        outtextxy(target, 200, 615, L"a - rysowanie wypeіnionego prostok№ta");
        outtextxy(target, 200, 630, L"c - rysowanie okrкgu");

        outtextxy(target, 470, 600, L"w - zapis do pliku");
        outtextxy(target, 470, 615, L"o - odczyt z pliku");
        outtextxy(target, 470, 630, L"esc - wyjњcie");

        outtextxy(target, 650, 615, L"Aktualne:");

        target.draw(*rectangle);
        target.draw(*colors_sprite);
    }
    // zwraca wskaznik text
    sf::Text* get_text() const 
    {
        return text;
    }
    // funkcja ktora sprawdza czy mysz jest w polu wyboru koloru 
    bool insideColorPick(int x, int y) const 
    {
        if (x > colors_size_x || y > colors_size_y) return false;
        return true;
    }
    // wybyramy kolor
    sf::Color pickColor(int x, int y) {
        sf::Uint8 r = colors_pixels[(y * colors_size_x + x) * 4 + 0];
        sf::Uint8 g = colors_pixels[(y * colors_size_x + x) * 4 + 1];
        sf::Uint8 b = colors_pixels[(y * colors_size_x + x) * 4 + 2];

        return sf::Color(r, g, b);
    }


};




int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    Menu menu;

    // zmienne
    char key = ' '; // przycisk
    // wchar_t pressedKey[1]; // nie dziala poprawn
    sf::Text* nText = menu.get_text(); 
    bool draw = false; // flaga rysowanie
    bool colorPick = false; // flaga, ktora sprawdza czy kolor jest wybrany
    bool keyPressed = false; // flaga, ktora sprawdza czy przycisk jest wcisniety
    int draw_x = 0; // polozenia myszki na osi x
    int draw_y = 0; // polozenia myszki na osi y
    sf::Color color = sf::Color::Black; // kolor rysowania
    sf::Color color_fill = sf::Color::Black; // kolor wypelnienia
    sf::VertexArray line(sf::LinesStrip, 2);
    sf::RectangleShape rectangle; // prostokat
    sf::CircleShape circle; // okrag
    
    // zmienne tekstury
    /// przchowuje jeden element
    sf::Texture one_texture;
    /// jej pole dzialania
    one_texture.create(800, 650);

    /// zbior wszystkich tekstur
    sf::Texture set_texture;
    /// pole
    set_texture.create(800, 650);

    /// obraz dla wyswietlania
    sf::Sprite obraz;
    /// nadajemy wartosc
    obraz.setTexture(set_texture);



    while (window.isOpen())
    {
        window.clear(sf::Color::Black);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            // klawiatura 
            if (event.type == sf::Event::KeyPressed) {
                keyPressed = true;
                if (event.key.code == sf::Keyboard::F)
                {
                    key = 'f';
                    //   *pressedKey = 'f'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::B)
                {
                    key = 'b';
                    //   *pressedKey = 'b'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::L)
                {
                    key = 'l';
                    //   *pressedKey = 'l'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    key = 'r';
                    //   *pressedKey = 'r'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::A)
                {
                    key = 'a';
                    //   *pressedKey = 'a'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::C)
                {
                    key = 'c';
                    //   *pressedKey = 'c'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::W)
                {
                    key = 'w';
                    set_texture.copyToImage().saveToFile("wyniki.png"); // zapis do pliku
                    //   *pressedKey = 'w'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::O)
                {
                    key = 'o';
                    set_texture.loadFromFile("wyniki.png"); // odczyt z pliku
                    //   *pressedKey = 'o'; // nie dziala poprawn
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }

            // myszka
            if (event.type == sf::Event::MouseButtonPressed) 
            {
                draw = true;
                draw_x = event.mouseButton.x;
                draw_y = event.mouseButton.y;
                colorPick = menu.insideColorPick(draw_x, draw_y);
 
              //  std::cout << "draw_x =  " << draw_x << " draw_y = " << draw_y<<"  "<<colorPick<< std::endl;
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                draw = false;

                // odswiezamy zbior tekstur
                set_texture.update(one_texture);
                
                // wybor koloru
                if (key == 'f' && colorPick)
                {
                    color = menu.pickColor(draw_x, draw_y);
                }
                if (key == 'b' && colorPick)
                {
                    color_fill = menu.pickColor(draw_x, draw_y);
                }
            }
        }
        

        //wyswietlamy obraz
        window.draw(obraz);
        //Draw BEGIN
        // Pomiкdzy "Draw BEGIN" a "Draw End" trzeba "cos" dopisac :-). Na pewno znajd№ siк tam te dwie linijki:
        if (draw) {
            if(key == 'l' && keyPressed) // rysujemy linije
            {
                line[0].position = sf::Vector2f(static_cast<float> (draw_x), static_cast<float> (draw_y));
                line[0].color = color;
                line[1].position = sf::Vector2f(static_cast<float> (sf::Mouse::getPosition(window).x), static_cast<float> (sf::Mouse::getPosition(window).y));
                line[1].color = color_fill;
                window.draw(line);
            }
            if (key == 'r' && keyPressed) //  rysujemy prostokat
            {
                rectangle.setSize(sf::Vector2f(static_cast<float> (sf::Mouse::getPosition(window).x - draw_x), static_cast<float> (sf::Mouse::getPosition(window).y - draw_y)));
                rectangle.setFillColor(sf::Color::Transparent);
                rectangle.setOutlineColor(color);
                rectangle.setOutlineThickness(1.0f);
                rectangle.setPosition(static_cast<float> (draw_x), static_cast<float>(draw_y));
                window.draw(rectangle);
            }
            if (key == 'a' && keyPressed)// rysujemy wypelniony prostokat
            {
                rectangle.setSize(sf::Vector2f(static_cast<float> (sf::Mouse::getPosition(window).x - draw_x), static_cast<float> (sf::Mouse::getPosition(window).y - draw_y)));
                rectangle.setFillColor(color_fill);
                rectangle.setOutlineColor(color);
                rectangle.setOutlineThickness(1.0f);
                rectangle.setPosition(static_cast<float> (draw_x), static_cast<float>(draw_y));
                window.draw(rectangle);
            }
            if (key == 'c' && keyPressed) // rysujemy okrag
            {   
                float promien = (sqrt(static_cast<float> (pow(sf::Mouse::getPosition(window).x - draw_x, 2) + pow(sf::Mouse::getPosition(window).y - draw_y, 2)))/2);
                circle.setRadius(promien);
                circle.setFillColor(color_fill);
                circle.setOutlineColor(color);
                circle.setOutlineThickness(3.0f);
                circle.setPosition(sf::Vector2f(static_cast<float> ( (draw_x - sf::Mouse::getPosition(window).x)/2 - promien + sf::Mouse::getPosition(window).x), static_cast<float> ((draw_y - sf::Mouse::getPosition(window).y) / 2 - promien + sf::Mouse::getPosition(window).y)));
                window.draw(circle);
            }
            
        }

        // odswieza teksture zawartoscia okna
        one_texture.update(window);
        // rysujemy prostokaty wyboru koloru

        //1 prostokąt
        rectangle.setSize(sf::Vector2f(34, 30));
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setOutlineThickness(0.0f);

        rectangle.setFillColor(color);
        rectangle.setPosition(765, 1);
        window.draw(rectangle);
        // 2 prostokąt
        rectangle.setFillColor(color_fill);
        rectangle.setPosition(765, 30);
        window.draw(rectangle);

        // wyswietlamy menu
        window.draw(menu);
        nText->setString(key);
        nText->setPosition(705, 615);
        window.draw(*nText);
        //menu.outtextxy(window, 705, 615, pressedKey); // nie dziala poprawn
        window.display();

        //Draw END
    }
    return 0;
}
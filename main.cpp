#include <iostream>
#include "handlers/Adaptive_Search.h"
#include "handlers/Hooke_Jeeves.h"
#include "handlers/Nelder_Mid.h"
#include "handlers/Visualizer.h"
#include "handlers/Panel.h"
#include <SFML/Graphics.hpp>

const int WIN_W  = 1200;
const int WIN_H  = 800;
const int PANEL_W = WIN_W / 2;
const int PANEL_H = WIN_H / 2;

int main() {

    Search s(-1.5, 1.5);
    while(!s.finished()) {
        s.step();
    }
    Point s_p = s.getCurrentPosition();
    std::cout << "Adaptive Search: " << "\n";
    std::cout << "x = " << s_p.x << "\n";
    std::cout << "y = " << s_p.y << "\n";
    std::cout << "f = " << s_p.value << "\n"; //значение в точке
    std::cout << "iter = " << s.getIter() << "\n";
    const std::vector<Point>& s_path = s.getPath();


    Hooke_Jeeves h(-1.5, 1.5);
    while(!h.finished()) {
        h.step();
    }
    Point h_p = h.getCurrentPosition();
    std::cout << "Hooke_Jeeves: " << "\n";
    std::cout << "x = " << h_p.x << "\n";
    std::cout << "y = " << h_p.y << "\n";
    std::cout << "f = " << h_p.value << "\n"; //значение в точке
    std::cout << "iter = " << h.getIter() << "\n";
    const std::vector<Point>& h_path = h.getPath();

    Nelder_Mid n(-1.5, 1.5);
    while(!n.finished()) {
        n.step();
    }
    Point n_p = n.getCurrentPosition();
    std::cout << "Nelder_Mid: " << "\n";
    std::cout << "x = " << n_p.x << "\n";
    std::cout << "y = " << n_p.y << "\n";
    std::cout << "f = " << n_p.value << "\n"; //значение в точке
    std::cout << "iter = " << n.getIter() << "\n";
    const std::vector<Point>& n_path = n.getPath();

    // ── Строим контурный график один раз ──
    sf::Image contourImg = buildContour(PANEL_W, PANEL_H,
                                        X_MIN, X_MAX, Y_MIN, Y_MAX);
    sf::Texture contourTex(contourImg);
    sf::Sprite  contourSprite(contourTex);

    // ── Шрифт ──
    sf::Font font;
    font.openFromFile("C:/Windows/Fonts/arial.ttf");

    // ── Окно ──
    sf::RenderWindow window(
        sf::VideoMode({(unsigned)WIN_W, (unsigned)WIN_H}),
        "Optimization Methods"
    );
    window.setFramerateLimit(60);

    // ── Главный цикл ──
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(20, 20, 30));

        // Три панели с методами
        drawPanel(window, contourSprite, s.getPath(), font,
                  "Adaptive Search", 0.f, 0.f,
                  PANEL_W, PANEL_H, sf::Color(100, 200, 255));

        drawPanel(window, contourSprite, h.getPath(), font,
                  "Hooke-Jeeves", (float)PANEL_W, 0.f,
                  PANEL_W, PANEL_H, sf::Color(255, 200, 100));

        drawPanel(window, contourSprite, n.getPath(), font,
                  "Nelder-Mead", 0.f, (float)PANEL_H,
                  PANEL_W, PANEL_H, sf::Color(100, 255, 150));

        drawStats(window, font, s, h, n,
          (float)PANEL_W, (float)PANEL_H);

        // Разделители
        sf::RectangleShape vline({1.f, (float)WIN_H});
        vline.setPosition({(float)PANEL_W, 0.f});
        vline.setFillColor(sf::Color(80, 80, 80));
        window.draw(vline);

        sf::RectangleShape hline({(float)WIN_W, 1.f});
        hline.setPosition({0.f, (float)PANEL_H});
        hline.setFillColor(sf::Color(80, 80, 80));
        window.draw(hline);

        window.display();
    }

    return 0;
}
/*
cmake --build .
.\Debug\main.exe
*/


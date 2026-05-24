#include <iostream>
#include <SFML/Graphics.hpp>
#include "handlers/Adaptive_Search.h"
#include "handlers/Hooke_Jeeves.h"
#include "handlers/Nelder_Mid.h"

#include "handlers/Visualizer.h"
#include "handlers/Panel.h"
#include "handlers/Visualizer.h" // Исправь 'z' на 's'
#include "handlers/Visualizer_3D.h"


const int WIN_W  = 1200;
const int WIN_H  = 800;
const int PANEL_W = WIN_W / 2;
const int PANEL_H = WIN_H / 2;

enum class ViewMode {
    Mode2D,
    Mode3D
};

int main() {
    //Для вывода в терминал
    Search search(-1.5, 1.5);
    while(!search.finished()) {
        search.step();
    }
    Point s_p = search.getCurrentPosition();
    std::cout << "Adaptive Search: " << "\n";
    std::cout << "x = " << s_p.x << "\n";
    std::cout << "y = " << s_p.y << "\n";
    std::cout << "f = " << s_p.value << "\n"; //значение в точке
    std::cout << "iter = " << search.getIter() << "\n";
    const std::vector<Point>& s_path = search.getPath();


    Hooke_Jeeves hooke(-1.5, 1.5);
    while(!hooke.finished()) {
        hooke.step();
    }
    Point h_p = hooke.getCurrentPosition();
    std::cout << "Hooke_Jeeves: " << "\n";
    std::cout << "x = " << h_p.x << "\n";
    std::cout << "y = " << h_p.y << "\n";
    std::cout << "f = " << h_p.value << "\n"; //значение в точке
    std::cout << "iter = " << hooke.getIter() << "\n";
    const std::vector<Point>& h_path = hooke.getPath();

    Nelder_Mid nelder(-1.5, 1.5);
    while(!nelder.finished()) {
        nelder.step();
    }
    Point n_p = nelder.getCurrentPosition();
    std::cout << "Nelder_Mid: " << "\n";
    std::cout << "x = " << n_p.x << "\n";
    std::cout << "y = " << n_p.y << "\n";
    std::cout << "f = " << n_p.value << "\n"; //значение в точке
    std::cout << "iter = " << nelder.getIter() << "\n";
    const std::vector<Point>& n_path = nelder.getPath();

    //Для вывода из sfml
    Search s(-1.5, 1.5);
    Hooke_Jeeves h(-1.5, 1.5);
    Nelder_Mid n(-1.5, 1.5);
   
    bool printed = false;
    ViewMode mode = ViewMode::Mode2D;

    sf::Image contourImg = buildContour(PANEL_W, PANEL_H, X_MIN, X_MAX, Y_MIN, Y_MAX);
    sf::Texture contourTex;
    contourTex.loadFromImage(contourImg);
    sf::Sprite contourSprite(contourTex);

    sf::Font font;
    font.openFromFile("C:/Windows/Fonts/arial.ttf");

    sf::RenderWindow window(
        sf::VideoMode({WIN_W, WIN_H}),
        "Optimization Animation"
    );

    window.setFramerateLimit(60);
    sf::Clock clock;
    float stepDelay = 0.000025f; // скорость анимации

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            // Обработка нажатий клавиш для переключения режима
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Num1) mode = ViewMode::Mode2D;
                if (keyPressed->code == sf::Keyboard::Key::Num2) mode = ViewMode::Mode3D;

                if (keyPressed->code == sf::Keyboard::Key::Left)  angleX += 0.05f;
                if (keyPressed->code == sf::Keyboard::Key::Right) angleX -= 0.05f;
                if (keyPressed->code == sf::Keyboard::Key::Up)    zoom += 0.05f;
                if (keyPressed->code == sf::Keyboard::Key::Down)  zoom -= 0.05f;
            }
        }

        if (clock.getElapsedTime().asSeconds() > stepDelay) {
            if (!s.finished()) s.step();
            if (!h.finished()) h.step();
            if (!n.finished()) n.step();
            clock.restart();
        }

        if (!printed && s.finished() && h.finished() && n.finished()) {
            Point s_p = s.getCurrentPosition();
            std::cout << "Adaptive Search: \nx = " << s_p.x << "\ny = " << s_p.y << "\nf = " << s_p.value << "\niter = " << s.getIter() << "\n\n";

            Point h_p = h.getCurrentPosition();
            std::cout << "Hooke_Jeeves: \nx = " << h_p.x << "\ny = " << h_p.y << "\nf = " << h_p.value << "\niter = " << h.getIter() << "\n\n";

            Point n_p = n.getCurrentPosition();
            std::cout << "Nelder_Mid: \nx = " << n_p.x << "\ny = " << n_p.y << "\nf = " << n_p.value << "\niter = " << n.getIter() << "\n\n";
            
            printed = true;
        }

        window.clear(sf::Color(20, 20, 30));

        if (mode == ViewMode::Mode2D) {
            // Отрисовка 2D панелей
            drawPanel(window, contourSprite, s.getPath(), font, "Adaptive Search", 0.f, 0.f, PANEL_W, PANEL_H, sf::Color(100, 200, 255));
            drawPanel(window, contourSprite, h.getPath(), font, "Hooke-Jeeves", (float)PANEL_W, 0.f, PANEL_W, PANEL_H, sf::Color(255, 200, 100));
            drawPanel(window, contourSprite, n.getPath(), font, "Nelder-Mead", 0.f, (float)PANEL_H, PANEL_W, PANEL_H, sf::Color(100, 255, 150));
            drawStats(window, font, s, h, n, (float)PANEL_W, (float)PANEL_H);
        } 
        else {
            // Отрисовка 3D поверхности
            drawSurface(window);
            drawPath3D(window, s.getPath(), sf::Color::Red, 2);//Адаптивный - белый
            drawPath3D(window, h.getPath(), sf::Color::Yellow, 0.5);//Хук-Дживс - желтый
            drawPath3D(window, n.getPath(), sf::Color::White, 0.8);//Нелдер-Мид голубой
        }

        window.display();
    }

    return 0;
}
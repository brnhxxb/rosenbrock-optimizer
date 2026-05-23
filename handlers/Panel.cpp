#include "Panel.h"
#include <string>

sf::Vector2f toScreen(double x, double y,
                      float panelX, float panelY,
                      int panelW, int panelH)
{
    float px = panelX + (float)((x - X_MIN) / (X_MAX - X_MIN)) * panelW;
    float py = panelY + (float)(1.0 - (y - Y_MIN) / (Y_MAX - Y_MIN)) * panelH;
    return {px, py};
}

void drawGrid(sf::RenderWindow& window,
              sf::Font& font,
              float panelX, float panelY,
              int panelW, int panelH)
{
    // Вертикальные линии по X
    for (int xi = -2; xi <= 2; xi++) {
        sf::Vector2f top = toScreen(xi, Y_MAX, panelX, panelY, panelW, panelH);
        sf::Vector2f bot = toScreen(xi, Y_MIN, panelX, panelY, panelW, panelH);

        sf::Color color = (xi == 0)
            ? sf::Color(255, 255, 255, 180)
            : sf::Color(255, 255, 255, 50);

        sf::Vertex line[2] = {
            sf::Vertex{top, color},
            sf::Vertex{bot, color}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);

        // Подпись по X
        sf::Text t(font, std::to_string(xi), 11);
        t.setPosition({bot.x + 2.f, bot.y - 16.f});
        t.setFillColor(sf::Color(255, 255, 255, 150));
        window.draw(t);
    }

    // Горизонтальные линии по Y
    for (int yi = -1; yi <= 3; yi++) {
        sf::Vector2f left  = toScreen(X_MIN, yi, panelX, panelY, panelW, panelH);
        sf::Vector2f right = toScreen(X_MAX, yi, panelX, panelY, panelW, panelH);

        sf::Color color = (yi == 0)
            ? sf::Color(255, 255, 255, 180)
            : sf::Color(255, 255, 255, 50);

        sf::Vertex line[2] = {
            sf::Vertex{left,  color},
            sf::Vertex{right, color}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);

        // Подпись по Y
        sf::Text t(font, std::to_string(yi), 11);
        t.setPosition({left.x + 2.f, left.y - 14.f});
        t.setFillColor(sf::Color(255, 255, 255, 150));
        window.draw(t);
    }
}

void drawPanel(sf::RenderWindow& window,
               sf::Sprite& contour,
               const std::vector<Point>& path,
               sf::Font& font,
               const std::string& label,
               float panelX, float panelY,
               int panelW, int panelH,
               sf::Color pathColor)
{
    // Контурный график
    contour.setPosition({panelX, panelY});
    window.draw(contour);

    // Сетка и оси
    drawGrid(window, font, panelX, panelY, panelW, panelH);

    // Траектория с градиентом яркости
    if (path.size() >= 2) {
        for (size_t i = 1; i < path.size(); i++) {
            sf::Vector2f p1 = toScreen(path[i-1].x, path[i-1].y,
                                    panelX, panelY, panelW, panelH);
            sf::Vector2f p2 = toScreen(path[i].x, path[i].y,
                                    panelX, panelY, panelW, panelH);

            // Вектор направления линии
            sf::Vector2f dir = p2 - p1;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len < 0.001f) continue;

            // Перпендикуляр — для толщины
            float thickness = 3.f;
            sf::Vector2f perp = {-dir.y / len * thickness * 0.5f,
                                dir.x / len * thickness * 0.5f};

            // Яркость растёт к концу траектории
            float alpha = 80.f + 175.f * (float)i / path.size();
            sf::Color c(255, 255, 255, (uint8_t)alpha); // белый

            // Рисуем как два треугольника (прямоугольник)
            sf::Vertex quad[4] = {
                sf::Vertex{p1 - perp, c},
                sf::Vertex{p1 + perp, c},
                sf::Vertex{p2 + perp, c},
                sf::Vertex{p2 - perp, c}
            };
            window.draw(quad, 4, sf::PrimitiveType::TriangleStrip);
        }
    }

    // Текущая точка
    if (!path.empty()) {
        sf::Vector2f last = toScreen(path.back().x, path.back().y,
                                     panelX, panelY, panelW, panelH);
        sf::CircleShape dot(5.f);
        dot.setOrigin({5.f, 5.f});
        dot.setPosition(last);
        dot.setFillColor(sf::Color::White);
        dot.setOutlineColor(sf::Color::Black);
        dot.setOutlineThickness(1.f);
        window.draw(dot);
    }

    // Глобальный минимум (1,1) зелёный ромб
    sf::Vector2f minPos = toScreen(1.0, 1.0, panelX, panelY, panelW, panelH);
    sf::CircleShape star(6.f, 4);
    star.setOrigin({6.f, 6.f});
    star.setPosition(minPos);
    star.setFillColor(sf::Color(50, 255, 50));
    star.setOutlineColor(sf::Color::White);
    star.setOutlineThickness(1.f);
    window.draw(star);

    // Подпись панели
    sf::Text t(font, label, 16);
    t.setPosition({panelX + 8.f, panelY + 6.f});
    t.setFillColor(sf::Color::White);
    window.draw(t);
}

void drawStats(sf::RenderWindow& window,
               sf::Font& font,
               Search& s,
               Hooke_Jeeves& h,
               Nelder_Mid& n,
               float panelX, float panelY)
{
    float x = panelX + 20.f;
    float y = panelY + 20.f;
    float lineH = 24.f;

    auto draw = [&](const std::string& txt, sf::Color color = sf::Color::White) {
        sf::Text t(font, txt, 14);
        t.setPosition({x, y});
        t.setFillColor(color);
        window.draw(t);
        y += lineH;
    };

    draw("=== Statistics ===");
    y += 8;

    draw("Adaptive Search:", sf::Color(100, 200, 255));
    draw("  x = " + std::to_string(s.getCurrentPosition().x).substr(0, 8));
    draw("  y = " + std::to_string(s.getCurrentPosition().y).substr(0, 8));
    draw("  f = " + std::to_string(s.getValue()));
    draw("  iter = " + std::to_string(s.getIter()));
    y += 8;

    draw("Hooke-Jeeves:", sf::Color(255, 200, 100));
    draw("  x = " + std::to_string(h.getCurrentPosition().x).substr(0, 8));
    draw("  y = " + std::to_string(h.getCurrentPosition().y).substr(0, 8));
    draw("  f = " + std::to_string(h.getValue()));
    draw("  iter = " + std::to_string(h.getIter()));
    y += 8;

    draw("Nelder-Mead:", sf::Color(100, 255, 150));
    draw("  x = " + std::to_string(n.getCurrentPosition().x).substr(0, 8));
    draw("  y = " + std::to_string(n.getCurrentPosition().y).substr(0, 8));
    draw("  f = " + std::to_string(n.getValue()));
    draw("  iter = " + std::to_string(n.getIter()));
}
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Adaptive_Search.h"
#include "Hooke_Jeeves.h"
#include "Nelder_Mid.h"
#include "Optimizer.h"

const double X_MIN = -2.0, X_MAX = 2.0;
const double Y_MIN = -1.0, Y_MAX = 3.0;

//координаты функции >пиксели панели
sf::Vector2f toScreen(double x, double y,
                      float panelX, float panelY,
                      int panelW, int panelH);

//контур, траектория, вцет
void drawPanel(sf::RenderWindow& window,
               sf::Sprite& contour,
               const std::vector<Point>& path,
               sf::Font& font,
               const std::string& label,
               float panelX, float panelY,
               int panelW, int panelH,
               sf::Color pathColor);

void drawStats(sf::RenderWindow& window,
               sf::Font& font,
               Search& s,
               Hooke_Jeeves& h,
               Nelder_Mid& n,
               float panelX, float panelY);
               
void drawGrid(sf::RenderWindow& window,
              sf::Font& font,
              float panelX, float panelY,
              int panelW, int panelH);
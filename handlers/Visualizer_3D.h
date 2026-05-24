#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Visualizer.h" // Для Point и rosenbrock

//extern переменные камеры
extern float angleX;
extern float zoom;

//функции для 3д отобраения
sf::Vector2f project(double x, double y, double z);
void drawSurface(sf::RenderWindow& window);
void drawPath3D(sf::RenderWindow& window, const std::vector<Point>& path, sf::Color color, double z_offset);
#pragma once
#include <SFML/Graphics.hpp>
#include "Optimizer.h"

sf::Image buildContour(int width, int height,
                       double xMin, double xMax,
                       double yMin, double yMax);
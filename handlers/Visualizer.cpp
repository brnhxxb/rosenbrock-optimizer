#include "Visualizer.h"
#include <cmath>
#include <cstdint>

static sf::Color valueToColor(double v) {
    double t = std::log1p(v) / std::log1p(2500.0);
    t = std::min(1.0, std::max(0.0, t));

    uint8_t r, g, b;
    if (t < 0.25) {
        r = 0;
        g = (uint8_t)(t * 4 * 255);
        b = 255;
    } else if (t < 0.5) {
        r = 0;
        g = 255;
        b = (uint8_t)((1 - (t - 0.25) * 4) * 255);
    } else if (t < 0.75) {
        r = (uint8_t)((t - 0.5) * 4 * 255);
        g = 255;
        b = 0;
    } else {
        r = 255;
        g = (uint8_t)((1 - (t - 0.75) * 4) * 255);
        b = 0;
    }
    return {r, g, b, 255};
}


sf::Image buildContour(int width, int height,
                       double xMin, double xMax,
                       double yMin, double yMax)
{
    sf::Image img(sf::Vector2u((unsigned)width, (unsigned)height));

    for (int py = 0; py < height; py++) {
        for (int px = 0; px < width; px++) {
            double x = xMin + (double)px / width  * (xMax - xMin);
            double y = yMax - (double)py / height * (yMax - yMin);
            double v = rosenbrock(x, y);
            img.setPixel({(unsigned)px, (unsigned)py}, valueToColor(v));
        }
    }
    return img;
}
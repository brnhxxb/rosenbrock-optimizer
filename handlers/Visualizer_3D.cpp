#include "Visualizer_3D.h"
#include <cmath>

float angleX = 0.5f;
float zoom = 1.0f;

sf::Vector2f project(double x, double y, double z) {
    float scale = 180.f * zoom; 
    float h = 0.2f * zoom;

    float rotX = x * cos(angleX) - y * sin(angleX);
    float rotY = x * sin(angleX) + y * cos(angleX);

    return sf::Vector2f(rotX * scale + 600.f, rotY * scale * 0.5f - static_cast<float>(z) * h + 400.f);
}

void drawSurface(sf::RenderWindow& window) {
    float step = 0.05f;

    for(double x=-2; x<2; x+=step)
    {
        for(double y=-1; y<3; y+=step)
        {
            double z1 = rosenbrock(x, y);
            double z2 = rosenbrock(x+step, y);
            double z3 = rosenbrock(x, y+step);
            double z4 = rosenbrock(x+step, y+step);

            sf::Vertex quad[4];

            quad[0].position = project(x, y, z1);
            quad[1].position = project(x+step, y, z2);
            quad[2].position = project(x+step, y+step, z4);
            quad[3].position = project(x, y+step, z3);

            float c = std::min(255.0, z1 * 20);

            for(int i=0;i<4;i++)
                quad[i].color = sf::Color(c, 255-c, 180);

            window.draw(quad, 4, sf::PrimitiveType::TriangleFan);
        }
    }
}

void drawPath3D(sf::RenderWindow& window, const std::vector<Point>& path, sf::Color color, double z_offset) {
    float thickness = 2.0f; //Толщина линии

    for (size_t i = 1; i < path.size(); i++) {
        // sf::Vector2f p1 = project(path[i-1].x, path[i-1].y, rosenbrock(path[i-1].x, path[i-1].y));
        // sf::Vector2f p2 = project(path[i].x, path[i].y, rosenbrock(path[i].x, path[i].y));
        sf::Vector2f p1 = project(path[i-1].x, path[i-1].y, rosenbrock(path[i-1].x, path[i-1].y) + z_offset);
        sf::Vector2f p2 = project(path[i].x, path[i].y, rosenbrock(path[i].x, path[i].y) + z_offset);

        //вычисляем вектор направления линии
        sf::Vector2f direction = p2 - p1;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length < 0.001f) continue;
        
        //вектор перпендикуляра для создания толщины
        sf::Vector2f normal = { -direction.y / length, direction.x / length };

        sf::Vertex quad[4];
        quad[0].position = p1 + normal * (thickness / 2.0f);
        quad[1].position = p2 + normal * (thickness / 2.0f);
        quad[2].position = p2 - normal * (thickness / 2.0f);
        quad[3].position = p1 - normal * (thickness / 2.0f);

        for (int j = 0; j < 4; j++) quad[j].color = color;

        window.draw(quad, 4, sf::PrimitiveType::TriangleFan);
    }
}
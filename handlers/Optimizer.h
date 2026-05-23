#pragma once
#include <vector>
#include <string>
#include <cmath>

inline double rosenbrock(double x, double y) {
    return (1 - x) * (1 - x) + 100 * (y - x * x) * (y - x * x);
}

struct Point {
    double x, y, value;
};

class Optimizer {
    public:
        virtual ~Optimizer() = default;

        virtual void step() = 0; //шаг 

        virtual void reset() = 0;

        virtual bool finished() const = 0;

        virtual double getValue() const = 0; //чисто значение по координатам в точке

        virtual int getIter() const = 0;

        //точка
        virtual Point getCurrentPosition() const = 0;
        //история точек
        virtual const std::vector<Point>& getPath() const = 0; 
};
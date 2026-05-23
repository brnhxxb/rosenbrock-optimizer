#include "Nelder_Mid.h"
#include <cmath>

Nelder_Mid::Nelder_Mid(double x0, double y0, double step, 
                         double alpha, double gamma, double rho, double sigma, 
                         double tol, int maxIter)
        : x0_(x0), y0_(y0), step_(step), 
        alpha_(alpha), gamma_(gamma), rho_(rho), sigma_(sigma), 
        tol_(tol), maxIter_(maxIter) 
    {
        reset();
    }

void Nelder_Mid::reset() {
    iter_ = 0;
    is_finished_ = false;
    history_point_.clear();
    simplex_.clear();

    //создание треугольника
    //базовая
    simplex_.push_back({x0_, y0_, rosenbrock(x0_, y0_)});
    //шаг по X
    simplex_.push_back({x0_ + step_, y0_, rosenbrock(x0_ + step_, y0_)});
    //шаг по Y
    simplex_.push_back({x0_, y0_ + step_, rosenbrock(x0_, y0_ + step_)});

    // Добавляем лучшую точку в историю
    history_point_.push_back(simplex_[0]);
}

void Nelder_Mid::step() {
    if (is_finished_) return;

    //сортировка точек
    std::sort(simplex_.begin(), simplex_.end(), [](const Point& a, const Point& b) {
        return a.value < b.value;
    });

    //лучшая точка сохраняется в историю точек
    history_point_.push_back(simplex_[0]);

    //разница между лучшей и худшей < tol_
    if (std::abs(simplex_[2].value - simplex_[0].value) < tol_ || iter_ >= maxIter_) {
        is_finished_ = true;
        return;
    }

    //находим центр
    double cx = (simplex_[0].x + simplex_[1].x) / 2.0;
    double cy = (simplex_[0].y + simplex_[1].y) / 2.0;

    //отряжаем
    double rx = cx + alpha_ * (cx - simplex_[2].x);
    double ry = cy + alpha_ * (cy - simplex_[2].y);
    double r_value = rosenbrock(rx, ry);

    if (r_value >= simplex_[0].value && r_value < simplex_[1].value) {
        //отраженная точка лучше худшей но не лучше самой лучшей , простая замена
        simplex_[2] = {rx, ry, r_value};
    } 
    //расстяжение
    else if (r_value < simplex_[0].value) {
        //получили хорошее направление пробуем дальше
        double ex = cx + gamma_ * (rx - cx);
        double ey = cy + gamma_ * (ry - cy);
        double e_value = rosenbrock(ex, ey);

        if (e_value < r_value) {
            simplex_[2] = {ex, ey, e_value}; //расстянули
        } else {
            simplex_[2] = {rx, ry, r_value}; //результата нету откатываемся
        }
    } 
    //сжатие
    else {
        //отраженная хуже второй(плохой)
        double cx_contract = cx + rho_ * (simplex_[2].x - cx);
        double cy_contract = cy + rho_ * (simplex_[2].y - cy);
        double c_value = rosenbrock(cx_contract, cy_contract);

        if (c_value < simplex_[2].value) {
            simplex_[2] = {cx_contract, cy_contract, c_value}; //сжали
        } else {
            //стягиваем треугольник к лучшей точке
            for (int i = 1; i <= 2; ++i) {
                simplex_[i].x = simplex_[0].x + sigma_ * (simplex_[i].x - simplex_[0].x);
                simplex_[i].y = simplex_[0].y + sigma_ * (simplex_[i].y - simplex_[0].y);
                simplex_[i].value = rosenbrock(simplex_[i].x, simplex_[i].y);
            }
        }
    }

    iter_++;
}

bool Nelder_Mid::finished() const { return is_finished_; }

int Nelder_Mid::getIter() const { return iter_; }

//возвращаем лучшую точку из симплекса
Point Nelder_Mid::getCurrentPosition() const { return simplex_[0]; } 

const std::vector<Point>& Nelder_Mid::getPath() const { return history_point_; }

double Nelder_Mid::getValue() const { return simplex_[0].value; }
//Адаптивный поиск
#include "Adaptive_Search.h"

Search::Search(double x0, double y0) : x0_(x0), y0_(y0) {
    reset();
}

void Search::reset() {
    current_x_ = x0_;
    current_y_ = y0_;

    step_ = 0.5;
    iter_ = 0;

    is_finished_ = false;

    double v = rosenbrock(current_x_, current_y_);

    history_point_.clear();
    history_point_.push_back({current_x_, current_y_, v});
}

void Search::step() {
    if (is_finished_) return;

    double best_x = current_x_;
    double best_y = current_y_;
    double best_v = rosenbrock(current_x_, current_y_); //значение в функции 

    double dx[] = { step_, -step_, 0, 0 }; //право, лево, вверх, вниз (ось х)
    double dy[] = { 0, 0, step_, -step_ }; //право лево вверх вниз (ось у)

    for (int i = 0; i < 4; i++) {
        double new_x = current_x_ + dx[i];
        double new_y = current_y_ + dy[i];
        double new_v = rosenbrock(new_x, new_y);

        if (new_v < best_v) { //если новая точка меньше то передаем значения
            best_v = new_v;
            best_x = new_x;
            best_y = new_y;
        }
    }

    //нашли улучшение переходим в него
    if (best_x != current_x_ || best_y != current_y_) {
        current_x_ = best_x;
        current_y_ = best_y;

        history_point_.push_back({current_x_, current_y_, best_v});
    } else {
        step_ *= 0.5;
    }

    iter_++;
    if (step_ < 1e-7) is_finished_ = true;
}

bool Search::finished() const { return is_finished_; }

int Search::getIter() const { return iter_; }

Point Search::getCurrentPosition() const { return {current_x_, current_y_, rosenbrock(current_x_, current_y_)}; }

const std::vector<Point>& Search::getPath() const { return history_point_; }

double Search::getValue() const {return rosenbrock(current_x_, current_y_); }
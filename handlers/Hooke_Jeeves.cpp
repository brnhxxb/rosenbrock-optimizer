#include "Hooke_Jeeves.h"

Hooke_Jeeves::Hooke_Jeeves(
    double x0,
    double y0,
    double step,
    double alpha,
    double tol,
    int maxIter
)
    : x0_(x0), y0_(y0),
      step_(step),
      alpha_(alpha),
      tol_(tol),
      maxIter_(maxIter)
{
    reset();
}

void Hooke_Jeeves::reset() {
    current_x_ = x0_;
    current_y_ = y0_;

    prev_x_ = current_x_;
    prev_y_ = current_y_;

    iter_ = 0;
    is_finished_ = false;

    history_point_.clear();

    double v = rosenbrock(current_x_, current_y_);
    history_point_.push_back( {current_x_, current_y_, v} );
}

void Hooke_Jeeves::step() {
    if (is_finished_) return;

    double best_x = current_x_;
    double best_y = current_y_;
    double best_v = rosenbrock(current_x_, current_y_);

    double dx[] = {step_, -step_, 0, 0};
    double dy[] = {0, 0, step_, -step_};

    for (int i = 0; i < 4; i++) {
        double new_x = current_x_ + dx[i];
        double new_y = current_y_ + dy[i];
        double new_v = rosenbrock(new_x, new_y);

        if (new_v < best_v) {
            best_v = new_v;
            best_x = new_x;
            best_y = new_y;
        }
    }

    bool improved = (best_x != current_x_ || best_y != current_y_);

    if (improved) {
        prev_x_ = current_x_;
        prev_y_ = current_y_;

        current_x_ = best_x;
        current_y_ = best_y;

        history_point_.push_back({current_x_, current_y_, best_v});

        double jump_x = current_x_ + (current_x_ - prev_x_);
        double jump_y = current_y_ + (current_y_ - prev_y_);
        double jump_v = rosenbrock(jump_x, jump_y);

        if (jump_v < best_v) {
            current_x_ = jump_x;
            current_y_ = jump_y;
            history_point_.push_back({current_x_, current_y_, jump_v});
        }

    } else {
        step_ *= alpha_;
    }

    iter_++;

    if (step_ < tol_ || iter_ >= maxIter_) {
        is_finished_ = true;
    } 
}

bool Hooke_Jeeves::finished() const { return is_finished_; }

int Hooke_Jeeves::getIter() const { return iter_; }

Point Hooke_Jeeves::getCurrentPosition() const {
    return {
        current_x_,
        current_y_,
        rosenbrock(current_x_, current_y_)
    };
}

const std::vector<Point>& Hooke_Jeeves::getPath() const { return history_point_; }

double Hooke_Jeeves::getValue() const { return rosenbrock(current_x_, current_y_); }
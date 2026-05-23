#pragma once
#include "Optimizer.h"
#include <vector>
#include <string>

class Hooke_Jeeves : public Optimizer {
    public:
        Hooke_Jeeves (double x0 = -1.5 , double y0 = -1.5,
                      double step = 0.5, double alpha = 0.5,
                      double tol = 1e-10, int maxIter = 10000);
        //tol - самый минимум шага
        //alpha - коэффициент уменьшения шага

        void step() override;

        void reset() override;

        bool finished() const override;

        Point getCurrentPosition() const override;

        const std::vector<Point>& getPath() const override;

        int getIter() const override;

        double getValue() const override;
        
    private:
            double x0_, y0_;
            double current_x_, current_y_;
            double prev_x_, prev_y_;
            double step_;

            int iter_;
            double tol_;
            int maxIter_;
            double alpha_;

            bool is_finished_;
            std::vector<Point> history_point_;
};
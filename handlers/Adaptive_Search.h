#pragma once
#include "Optimizer.h"

class Search : public Optimizer {
    public:
        Search(double x0 = -1.5, double y0 = 1.5);

        void step() override;

        bool finished() const override;
        
        void reset() override;

        int getIter() const override;

        double getValue() const override;

        Point getCurrentPosition() const override;
        const std::vector<Point>& getPath() const override;

    private:
        double x0_, y0_;
        double current_x_, current_y_;
        double step_;
        int iter_;
        bool is_finished_;
        std::vector<Point> history_point_;
};

        
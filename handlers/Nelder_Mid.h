#pragma once
#include "Optimizer.h"
#include <vector>
#include <algorithm> //std::sort

class Nelder_Mid : public Optimizer {
    public:
        Nelder_Mid(double x0 = -1.5, double y0 = -1.5,
                    double step = 0.5, 
                    double alpha = 1.0, double gamma = 2.0, 
                    double rho = 0.5, double sigma = 0.5,
                    double tol = 1e-10, int maxIter = 10000);

        void step() override;
        void reset() override;
        bool finished() const override;
        Point getCurrentPosition() const override;
        const std::vector<Point>& getPath() const override;
        int getIter() const override;
        double getValue() const override;
        
    private:
        double x0_, y0_;
        double step_; 
        
        //Коэф деформации симплекса
        double alpha_; //Коэф отражения
        double gamma_; //Коэф растяжения
        double rho_;   //Коэф сжатия
        double sigma_; //Коэф глобального сжатия

        int iter_;
        double tol_;
        int maxIter_;
        bool is_finished_;

        //3 точки
        std::vector<Point> simplex_; 
        std::vector<Point> history_point_;
};
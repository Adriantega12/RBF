#ifndef GRAPHPLOT_H
#define GRAPHPLOT_H

#include <cmath>

#include "qcustomplot.h"

class GraphPlot {
    private:
        // Plot
        QCustomPlot* plot;

        // Graphs
        QCPGraph* destFunc;
        QCPGraph* kMeans;

        void initPlot();

    public:
        GraphPlot(QCustomPlot* graphPlot);

        void setup();

        void setFuncData(QVector<double> x, QVector<double> y);
        void setKMeansData(QVector<double> x, QVector<double> y);

        void drawCircles(QVector<double> x, QVector<double> y, QVector<double> sigma);
        // void add(QVector<double> x, QVector<double> y);
    };

#endif // GRAPHPLOT_H

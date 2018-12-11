#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "randdouble.h"
#include "graphplot.h"
#include "errorplot.h"

namespace Ui {
    class MainWindow;
    }

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        // UI
        GraphPlot* gPlot;
        ErrorPlot* ePlot;
        Ui::MainWindow *ui;

        // Utilery
        Rand_double rdg;

        // Data
        QVector<double> x;
        QVector<double> y;
        QVector<double> centersX;
        QVector<double> centersY;
        QVector<double> sigmaK;
        QVector<QVector<double>> phis;
        QVector<double> outputs;
        QVector<double> weights;

        int kMeansSize;
        double learningRate;

        // Perceptrón
        int maxEpochs;

        // Algorithm
        // Phase 1
        void findMeanCenters();
        void getSigmas();
        void getPhis();

        // Phase 2
        void perceptron();

        // Update label values
        void updateLabels(int currentEpoch, int convergeEpoch, double error);

        double function(double x, int index);

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
private slots:
        void on_initBtn_clicked();
        void on_stepSlider_sliderMoved(int position);
        void on_trainBtn_clicked();
};

#endif // MAINWINDOW_H

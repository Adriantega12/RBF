#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "randdouble.h"
#include "graphplot.h"

namespace Ui {
    class MainWindow;
    }

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        // UI
        GraphPlot* gPlot;
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

        // Algorithm parts
        void findMeanCenters();
        void getSigmas();
        void getPhis();

        void perceptron();

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
private slots:
        void on_initBtn_clicked();
        void on_stepSlider_sliderMoved(int position);
        void on_trainBtn_clicked();
};

#endif // MAINWINDOW_H

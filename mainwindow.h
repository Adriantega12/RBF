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
    struct Pair {
        double x;
        double y;
        };

        GraphPlot* gPlot;
        Ui::MainWindow *ui;

        std::vector<Pair> funcGraph;

        Rand_double rdg;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
private slots:
        void on_initBtn_clicked();
        void on_stepSlider_sliderMoved(int position);
};

#endif // MAINWINDOW_H

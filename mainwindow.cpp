#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rdg(-5.0, 5.0) {
    ui->setupUi(this);

    gPlot = new GraphPlot(ui->gPlot);
    }

MainWindow::~MainWindow() {
    delete gPlot;
    delete ui;
    }

void MainWindow::on_initBtn_clicked() {
    // Initiate function points
    double step = ui->stepSlider->value() / 100.0;
    int size = 10 / step;
    QVector<double> x(size + 1), y(size + 1);
    for (int i = 0; i <= size; ++i) {
        x[i] = i * step - 5.0;
        y[i] = sin(x[i]);
        }
    gPlot->setFuncData(x, y);

    // Initiate K-Means points
    int kMeansSize = ui->kSB->value();
    QVector<double> xK(kMeansSize), yK(kMeansSize), sigmaK(kMeansSize, 0.0);
    for (int i = 0; i < kMeansSize; ++i) {
        xK[i] = rdg();
        yK[i] = rdg();
        }
    gPlot->setKMeansData(xK, yK);

    // Find minimized centers
    double learningRate = ui->lrSB->value();
    QVector<double> prevXK(kMeansSize), prevYK(kMeansSize);
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < size; ++i) { // For every input
            // Find closest K mean
            double minDistance = sqrt(pow(x[i] - xK[0], 2) + pow(y[i] - yK[0], 2)), distance;
            int minIndex = 0;
            for (int j = 1; j < kMeansSize; ++j) { // For every k-means
                distance = sqrt(pow(x[i] - xK[j], 2) + pow(y[i] - yK[j], 2));
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                    }
                }
            xK[minIndex] += learningRate * (x[i] - xK[minIndex]);
            yK[minIndex] += learningRate * (y[i] - yK[minIndex]);
            }
        for (int i = 0; i < kMeansSize; ++i) {
            if (xK[i] != prevXK[i] or yK[i] != prevYK[i]) {
                changed = true;
                }
            prevXK[i] = xK[i];
            prevYK[i] = yK[i];
            }
        gPlot->setKMeansData(xK, yK);
        } while (changed);

    // Get sigma
    for (int i = 0; i < kMeansSize; ++i) {
        double minDistance = 100.0;
        double distance;
        for (int j = 0; j < kMeansSize; ++j) {
            if (i != j) {
                distance = sqrt(pow(xK[i] - xK[j], 2) + pow(yK[i] - yK[j], 2));
                if (distance < minDistance) {
                    minDistance = distance;
                    }
                }
            }
        sigmaK[i] = minDistance;
        }
    gPlot->drawCircles(xK, yK, sigmaK);

    // Phis
    QVector<QVector<double>> phis(kMeansSize, QVector<double>(size + 1));
    for (int i = 0; i < kMeansSize; ++i) {
        for (int j = 0; j <= size; ++j) {
            phis[i][j] = exp(- (pow(xK[i] - x[j], 2) + pow(yK[i] - y[j], 2)) /
                    2 * sigmaK[i] * sigmaK[i]);
            }
        }
    qDebug() << phis;

    // Perceptron init
    QVector<double> weights(kMeansSize + 1);
    for (int i = 0; i < weights.size(); ++i) {
        weights[i] = rdg();
        }

    // Perceptron algorithm
    QVector<double> outputs(size);
    int epochs = 0;
    int maxEpochs = 1000;
    double error = 0;
    double errorPerEpoch;
    double accum = 0.0;
    bool done = false;

    while (epochs < maxEpochs and !done) {
        done = true;
        errorPerEpoch = 0.0;
        for (int i = 0; i < size; ++i) {
            accum = 0.0;
            for (int j = 0; j < weights.size(); ++j) {
                accum += (j == 0 ? -1 * weights[j] : phis[j - 1][i] * weights[j]);
                }
            outputs[i] = accum;
            error = y[i] - outputs[i];
            if (error != 0.0) {
                for(int j = 0; j < weights.size(); ++j) {
                    weights[j] += learningRate * error * (j == 0 ? -1 : phis[j - 1][i]);
                    }
                errorPerEpoch += error;
                done = false;
                }
            }
        epochs++;
        }

    gPlot->setOutputs(x, outputs);

    qDebug() << outputs;
    }

void MainWindow::on_stepSlider_sliderMoved(int position) {
    ui->stepLbl->setText(QString::number(position/100.0));
    }

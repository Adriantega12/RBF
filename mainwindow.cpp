#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::findMeanCenters() {
    // Find minimized centers
    QVector<double> prevcentersX(kMeansSize), prevcentersY(kMeansSize);
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < x.size(); ++i) { // For every input
            // Find closest K mean
            double minDistance = sqrt(pow(x[i] - centersX[0], 2) + pow(y[i] - centersY[0], 2)), distance;
            int minIndex = 0;
            for (int j = 1; j < kMeansSize; ++j) { // For every k-means
                distance = sqrt(pow(x[i] - centersX[j], 2) + pow(y[i] - centersY[j], 2));
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                    }
                }
            centersX[minIndex] += learningRate * (x[i] - centersX[minIndex]);
            centersY[minIndex] += learningRate * (y[i] - centersY[minIndex]);
            }
        for (int i = 0; i < kMeansSize; ++i) {
            if (centersX[i] != prevcentersX[i] or centersY[i] != prevcentersY[i]) {
                changed = true;
                }
            prevcentersX[i] = centersX[i];
            prevcentersY[i] = centersY[i];
            }
        gPlot->setKMeansData(centersX, centersY); // Plot data into graph
        } while (changed);
    }

void MainWindow::getSigmas() {
    for (int i = 0; i < kMeansSize; ++i) {
        double minDistance = 100.0;
        double distance;
        for (int j = 0; j < kMeansSize; ++j) {
            if (i != j) {
                distance = sqrt(pow(centersX[i] - centersX[j], 2) + pow(centersY[i] - centersY[j], 2));
                if (distance < minDistance) {
                    minDistance = distance;
                    }
                }
            }
        sigmaK[i] = minDistance;
        }
    gPlot->drawCircles(centersX, centersY, sigmaK); // Plot the circles
}

void MainWindow::getPhis() {
    for (int i = 0; i < kMeansSize; ++i) {
        for (int j = 0; j < x.size(); ++j) {
            phis[i][j] = exp(
                         - (pow(centersX[i] - x[j], 2) + pow(centersY[i] - y[j], 2)) /
                            2 * sigmaK[i] * sigmaK[i]
                           );
            }
        }
    }

void MainWindow::perceptron() {
    // Perceptron algorithm
    int epochs = 0;
    double error = 0;
    double errorPerEpoch;
    double accum = 0.0;
    bool done = false;

    while (epochs < maxEpochs and !done) {
        done = true;
        errorPerEpoch = 0.0;
        for (int i = 0; i < x.size(); ++i) {
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
        gPlot->setOutputs(x, outputs); // Plot outputs
        updateLabels(epochs, 0, error);
        }
    updateLabels(epochs, 0, error);
    }

void MainWindow::updateLabels(int currentEpoch, int convergeEpoch, double error) {
    ui->epochVal->setText(QString::number(currentEpoch));
    ui->convergeVal->setText(QString::number(convergeEpoch));
    //ui->errorVal->setText(QString::number(error));
    }

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
    // Initiate input points
    double step = ui->stepSlider->value() / 100.0;
    int size = 10 / step;
    x.clear();
    y.clear();
    x.resize(size + 1);
    y.resize(size + 1);
    for (int i = 0; i <= size; ++i) {
        x[i] = i * step - 5.0;
        y[i] = sin(x[i]);
        }
    gPlot->setFuncData(x, y);

    // Initiate K-Means points
    kMeansSize = ui->kSB->value();
    centersX.clear();
    centersY.clear();
    sigmaK.clear();
    centersX.resize(kMeansSize);
    centersY.resize(kMeansSize);
    sigmaK.resize(kMeansSize);
    for (int i = 0; i < kMeansSize; ++i) {
        centersX[i] = rdg();
        centersY[i] = rdg();
        }
    gPlot->setKMeansData(centersX, centersY);

    // Initiate learning rate
    learningRate = ui->lrSB->value();

    // Initiate phis
    phis.clear();
    phis.resize(kMeansSize);
    for (int i = 0; i < kMeansSize; ++i) {
        phis[i].clear();
        phis[i].resize(size + 1);
        }

    // Initiate outputs
    outputs.clear();
    outputs.resize(size + 1);

    // Initiate weights
    weights.clear();
    weights.resize(kMeansSize + 1);
    for (int i = 0; i < weights.size(); ++i) {
        weights[i] = rdg();
        }

    // Perceptron
    maxEpochs = ui->epochSB->value();

    // Initiate labels
    ui->epochVal->setText(QString::number(0));
    ui->convergeVal->setText("#");
    //ui->errorVal->setText("#");
    }

void MainWindow::on_stepSlider_sliderMoved(int position) {
    ui->stepLbl->setText("Step: "+ QString::number(position/100.0));
    }

void MainWindow::on_trainBtn_clicked() {
    // Phase 1 of RBF: Means, sigmas and phis
    findMeanCenters();
    getSigmas();
    getPhis();

    // Phase 2 of RBF: Train perceptron to get outputs
    perceptron();
    }

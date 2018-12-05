#include "graphplot.h"

void GraphPlot::initPlot() {
    plot->xAxis->setRange( -5.0, 5.0 );
    plot->yAxis->setRange( -5.0, 5.0 );
    plot->xAxis2->setRange( -5.0, 5.0 );
    plot->yAxis2->setRange( -5.0, 5.0 );
    // Colors of margins
    plot->xAxis->setBasePen( QPen( Qt::gray ) );
    plot->yAxis->setBasePen( QPen( Qt::gray ) );
    plot->xAxis2->setBasePen( QPen( Qt::gray ) );
    plot->yAxis2->setBasePen( QPen( Qt::gray ) );
    // Colors of axis
    plot->xAxis->grid()->setZeroLinePen( QPen( Qt::black ) );
    plot->yAxis->grid()->setZeroLinePen( QPen( Qt::black ) );
    }

GraphPlot::GraphPlot(QCustomPlot* graphPlot) {
    // Setup plot
    plot = graphPlot;
    initPlot();

    // Initiate func graph
    destFunc = new QCPGraph( plot->xAxis, plot->yAxis );
    destFunc->setPen(QPen(Qt::blue));
    destFunc->setLineStyle(QCPGraph::lsNone);
    destFunc->setScatterStyle(QCPScatterStyle::ssDisc);

    // Initiate K-Means graph
    kMeans = new QCPGraph( plot->xAxis, plot->yAxis );
    kMeans->setPen(QPen(Qt::red));
    kMeans->setLineStyle(QCPGraph::lsNone);
    kMeans->setScatterStyle(QCPScatterStyle::ssCross);
    }

void GraphPlot::setFuncData(QVector<double> x, QVector<double> y) {
    destFunc->setData(x, y);
    plot->replot();
    }

void GraphPlot::setKMeansData(QVector<double> x, QVector<double> y) {
    kMeans->setData(x, y);
    plot->replot();
    }

void GraphPlot::drawCircles(QVector<double> x, QVector<double> y, double sigma) {
    for (int i = 0; i < x.size(); ++i) {
        QCPItemEllipse *ellipse = new QCPItemEllipse(plot);
        ellipse->setAntialiased(true);
        ellipse->topLeft->setCoords(x[i] - sigma, y[i] + sigma);
        ellipse->bottomRight->setCoords(x[i] + sigma, y[i] - sigma);
        ellipse->setPen(QPen(Qt::red));
        ellipse->setBrush(QBrush(QColor(255, 0, 0, 20)));
        }

    plot->replot();
    }

/*void GraphPlot::add(double x, double y) {
    destFunc->addData(x, y);
    plot->replot();
    }*/

void GraphPlot::setup() {

    }

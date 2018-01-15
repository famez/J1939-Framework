#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QChartView>

#include <QDebug>

#include "spnchart.h"

SPNViewer::SPNViewer(u32 spnNumber, const QString& title, const QString& vertTitle, const QRect& ranges) :
    m_series(0),
    m_axis(new QValueAxis),
    m_step(0),
    m_x(5),
    m_y(1),
    mSPNNumber(spnNumber),
    mTitle(title),
    mVertTitle(vertTitle),
    mRanges(ranges)
{

    mChart = new QChart;

    QChartView* chartView = new QChartView(mChart);

    m_series = new QSplineSeries(mChart);
    QPen green(Qt::green);
    green.setWidth(3);
    m_series->setPen(green);
    m_series->append(m_x, m_y);

    mChart->addSeries(m_series);
    mChart->createDefaultAxes();
    mChart->setAxisX(m_axis, m_series);
    m_axis->setTickCount(5);
    mChart->axisX()->setRange(ranges.left(), ranges.right());
    mChart->axisY()->setRange(ranges.bottom(), ranges.top());

    mChart->setTitle(mTitle);
    mChart->legend()->hide();
    //mChart->setAnimationOptions(QChart::AllAnimations);

    chartView->setRenderHint(QPainter::Antialiasing);
    mWindow.setCentralWidget(chartView);
    mWindow.resize(400, 300);
}

SPNViewer::~SPNViewer() {

}


void SPNViewer::visualize() {
    mWindow.show();
}

void SPNViewer::append(qreal x, qreal y) {
    m_series->append(x, y);
}

void SPNViewer::appendPoints(QList<QPointF> points) {
    qDebug() << "[SPNViewer::appendPoints] Adding" << points.size() << "points";
    m_series->append(points);
}

#ifndef SPNCHART_H
#define SPNCHART_H


#include <QMainWindow>
#include <QtCharts/QChart>
#include <QRect>

#include <Types.h>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class SPNViewer : public QObject
{
    Q_OBJECT
public:
    SPNViewer(u32 spnNumber, const QString& title, const QString& vertTitle, const QRect& ranges);
    virtual ~SPNViewer();

    void visualize();
    void append(qreal x, qreal y);

    u32 getSPNNumber() const { return mSPNNumber; }

public slots:
    void appendPoints(QList<QPointF>);

private:
    QMainWindow mWindow;
    QChart* mChart;
    QSplineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axis;
    qreal m_step;
    qreal m_x;
    qreal m_y;

    u32 mSPNNumber;
    QString mTitle;
    QString mVertTitle;

    //Ranges
    QRect mRanges;


};

#endif // SPNCHART_H

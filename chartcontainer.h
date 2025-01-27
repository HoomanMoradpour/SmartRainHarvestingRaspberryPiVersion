#ifndef CHARTCONTAINER_H
#define CHARTCONTAINER_H
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QMap>

#include "noaaweatherfetcher.h"


class ChartContainer
{
public:
    ChartContainer();
    void plotWeatherData(const QVector<WeatherData>& weatherData, const QString& yAxisTitle);
    void plotWeatherDataMap(const QMap<QString, QVector<WeatherData>>& weatherDataMap);
    QtCharts::QChartView *GetChartView() {return chartview;}
private:
    QtCharts::QChart *chart = new QtCharts::QChart();
    QtCharts::QChartView *chartview = new QtCharts::QChartView();
    void removeAllAxes();
    QVector<QColor> colors;
};

#endif // CHARTCONTAINER_H

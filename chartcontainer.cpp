#include "chartcontainer.h"

using namespace QtCharts;

ChartContainer::ChartContainer()
{
    for (int i=0; i<10; i++)
        colors.append(QColor(rand()%256,rand()%256,rand()%256));
}

void ChartContainer::plotWeatherData(const QVector<WeatherData>& weatherData, const QString& yAxisTitle) {
        // Create a line series
        chart->removeAllSeries();
        removeAllAxes();
        QLineSeries* series = new QLineSeries();

        for (const auto& data : weatherData) {
            series->append(data.timestamp.toMSecsSinceEpoch(), data.value);
        }

        QPen pen = series->pen();
        pen.setWidth(4); // Set the desired line thickness
        pen.setColor(colors[0]);
        series->setPen(pen);

        // Create the chart
        chart->addSeries(series);
        chart->setTitle(yAxisTitle);
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Customize axes
        QDateTimeAxis* axisX = new QDateTimeAxis();
        axisX->setTitleText("Time");
        axisX->setFormat("dd/MM HH:mm");
        QFont xAxisFont = axisX->labelsFont();
        xAxisFont.setPointSize(8); // Make font smaller
        axisX->setLabelsFont(xAxisFont);
        axisX->setTickCount(20);
        axisX->setLabelsAngle(90.0); // Rotate labels by 90 degrees
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis* axisY = new QValueAxis();
        axisY->setTitleText(yAxisTitle);
        axisY->setLabelFormat("%.1f");
        QFont yAxisFont = axisY->labelsFont();
        yAxisFont.setPointSize(8); // Make font smaller
        axisY->setLabelsFont(yAxisFont);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chartview->setChart(chart);
        chartview->setRenderHint(QPainter::Antialiasing);

}

void ChartContainer::plotWeatherDataMap(const QMap<QString, QVector<WeatherData>>& weatherDataMap) {
        // Create the chart
        chart->removeAllSeries();
        removeAllAxes();
        //chart->setTitle("Weather Data");
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Find the range of the x-axis based on the longest timeseries
        QDateTime minTime = QDateTime::currentDateTime();
        QDateTime maxTime = QDateTime::fromSecsSinceEpoch(0);

        for (auto it = weatherDataMap.begin(); it != weatherDataMap.end(); ++it) {
            const QVector<WeatherData>& weatherData = it.value();
            if (!weatherData.empty()) {
                QDateTime seriesStartTime = weatherData.front().timestamp;
                QDateTime seriesEndTime = weatherData.back().timestamp;

                if (seriesStartTime < minTime) {
                    minTime = seriesStartTime;
                }
                if (seriesEndTime > maxTime) {
                    maxTime = seriesEndTime;
                }
            }
        }

        // Customize the X-axis (shared across all series)
        QDateTimeAxis* axisX = new QDateTimeAxis();
        axisX->setTitleText("Time");
        axisX->setFormat("dd/MM HH:mm");
        axisX->setTickCount(10);
        QFont xAxisFont = axisX->labelsFont();
        xAxisFont.setPointSize(8); // Make font smaller
        axisX->setLabelsFont(xAxisFont);
        axisX->setLabelsAngle(90.0); // Rotate labels by 90 degrees
        axisX->setRange(minTime,maxTime);
        chart->addAxis(axisX, Qt::AlignBottom);

        // Add series for each key in the map
        int counter = 0;
        for (auto it = weatherDataMap.begin(); it != weatherDataMap.end(); ++it) {
            counter++;
            const QString& yAxisTitle = it.key();
            const QVector<WeatherData>& weatherData = it.value();

            // Create a line series
            QLineSeries* series = new QLineSeries();
            series->setName(yAxisTitle);

            for (const auto& data : weatherData) {
                series->append(data.timestamp.toMSecsSinceEpoch(), data.value);
            }

            QPen pen = series->pen();
            pen.setWidth(4); // Set the desired line thickness
            pen.setColor(colors[counter]);
            series->setPen(pen);

            // Add the series to the chart
            chart->addSeries(series);
            series->attachAxis(axisX);

            // Create and attach a Y-axis for this series
            QValueAxis* axisY = new QValueAxis();
            axisY->setTitleText(yAxisTitle);
            QFont yAxisFont = axisY->labelsFont();
            yAxisFont.setPointSize(8); // Make font smaller
            axisY->setLabelsFont(yAxisFont);
            axisY->setLabelFormat("%.1f");
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);
        }

        QFont titleFont = chart->titleFont();
        titleFont.setPointSize(10); // Make font smaller
        chart->setTitleFont(titleFont);
        chartview->setChart(chart);
        chartview->setRenderHint(QPainter::Antialiasing);

}

void ChartContainer::removeAllAxes() {
    // Get a list of all axes associated with the chart
    QList<QAbstractAxis*> axes = chart->axes();

    // Loop through each axis and remove it
    for (QAbstractAxis* axis : axes) {
        chart->removeAxis(axis);
    }
}



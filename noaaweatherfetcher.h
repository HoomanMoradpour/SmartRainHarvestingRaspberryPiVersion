#ifndef NOAAWEATHERFETCHER_H
#define NOAAWEATHERFETCHER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <vector>
#include <iostream>

enum class datatype {ProbabilityofPrecipitation, Temperature, PrecipitationAmount, RelativeHumidity};

// Struct to hold the weather prediction data
struct WeatherData {
    QDateTime timestamp;
    double value;
};

class QChartView;

class NOAAWeatherFetcher : public QObject {
    Q_OBJECT

public:
    NOAAWeatherFetcher(QObject* parent = nullptr);
    QVector<WeatherData> getWeatherPrediction(int latitude, int longitude, datatype type);



private:
    QNetworkAccessManager* manager;
};

double calculateCumulativeValue(const QVector<WeatherData>& weatherData, int days);




#endif // NOAAWEATHERFETCHER_H

#include "noaaweatherfetcher.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <vector>
#include <iostream>


NOAAWeatherFetcher::NOAAWeatherFetcher(QObject* parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

QVector<WeatherData> NOAAWeatherFetcher::getWeatherPrediction(int latitude, int longitude, datatype type) {
    QVector<WeatherData> weatherData;

    // Construct NOAA API URL
    QString url;

    url = QString("https://api.weather.gov/gridpoints/LWX/%1,%2").arg(latitude).arg(longitude);
    qDebug()<<url;
    // Make the request
    QNetworkRequest request((QUrl(url)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString DataType;
    switch (type) {
        case datatype::PrecipitationAmount:
            DataType = "quantitativePrecipitation";
            break;
        case datatype::ProbabilityofPrecipitation:
            DataType = "probabilityOfPrecipitation";
            break;
        case datatype::RelativeHumidity:
            DataType = "relativeHumidity";
            break;
        case datatype::Temperature:
            DataType = "temperature";
            break;
    }
    qDebug()<<DataType;
    QNetworkReply* reply = manager->get(request);

    // Wait for the reply to finish (blocking approach for simplicity)
    QEventLoop loop;

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject properties = jsonObject["properties"].toObject();
        //qDebug()<<properties;
        qDebug()<<properties[DataType];
        QJsonArray periods = properties[DataType].toObject()["values"].toArray();
        qDebug()<<periods;
        for (const auto& period : periods) {
            QJsonObject obj = period.toObject();

            // Extract time and value (temperature or any other parameter)
            qDebug()<<obj["validTime"].toString().split("+")[0];
            QDateTime time = QDateTime::fromString(obj["validTime"].toString().split("+")[0], "yyyy-MM-ddTHH:mm:ss");
            qDebug()<<time;
            double value = obj["value"].toDouble();

            weatherData.push_back({time, value});
        }
    } else {
        qWarning() << "Error fetching weather data:" << reply->errorString();
    }

    reply->deleteLater();
    return weatherData;
}

double calculateCumulativeValue(const QVector<WeatherData>& weatherData, int days) {
    if (weatherData.isEmpty()) return 0.0;

    QDateTime startTime = weatherData.first().timestamp;
    QDateTime endTime = startTime.addDays(days);

    double cumulativeValue = 0.0;
    for (const auto& data : weatherData) {
        if (data.timestamp <= endTime) {
            cumulativeValue += data.value;
        } else {
            break;
        }
    }

    return cumulativeValue;
}





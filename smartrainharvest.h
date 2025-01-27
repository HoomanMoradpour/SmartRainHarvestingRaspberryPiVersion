#ifndef SMARTRAINHARVEST_H
#define SMARTRAINHARVEST_H

#include <QMainWindow>
#include "noaaweatherfetcher.h"
#include "chartcontainer.h"
#include "DistanceSensor.h"
#include "QTimer"
#include "QPushButton"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class SmartRainHarvest; }
QT_END_NAMESPACE

class QChartView;

class SmartRainHarvest : public QMainWindow
{
    Q_OBJECT

public:
    SmartRainHarvest(QWidget *parent = nullptr);
    ~SmartRainHarvest();
    QVector<WeatherData> cummulativerain;
    QVector<WeatherData> depth;
    QVector<WeatherData> openshut;
    // These parameters need to be adjusted
    double max_distance = 100; //Distance to the bottom of the barrel (cm)
    double waterdepthcriteria = 50; //If there is a rain in the forecast and water depth is higher than this, the valve will open (cm)
    double cummulativeraincriteria = 50; //This is how much to rain within the next two days so we consider rain in the forecast (mm)
    double bypassdepthcriteria = 100; //If the water depth in the tank exceeds this, water will be released (cm)
    double depthtoreleaseto = 75; // When above occures, water will be released to this depth (cm)
    double minumumdepth = 5; //When rain is in forecast, water will be released to this depth (cm)
    int Check_Weather_Interval = 10; //Time interval to check the weather and decide on opening or shutting the valve in seconds
    void StartRelease();
private:
    Ui::SmartRainHarvest *ui;
    NOAAWeatherFetcher fetcher;
    int latitude = 97;
    int longitude = 71;
    ChartContainer *ProbnQuanChartContainer = new ChartContainer();
    ChartContainer *CummulativeForcastChartContainer = new ChartContainer();
    ChartContainer *WaterDepthChartContainer = new ChartContainer();
    ChartContainer *OpenShutChartContainer = new ChartContainer();
    DistanceSensor distancesensor;
    QTimer *ReleaseTimer = new QTimer();
    void ShutTheValve();
    void OpenTheValve();
    bool overflow = false;
    bool state = false;
    QPushButton *ManualOpenShut;
    QLabel *DistanceLbl;
public slots:
    void on_Check_Timer();
    void on_Check_Distance();
    void on_ManualOpenShut();


};
#endif // SMARTRAINHARVEST_H

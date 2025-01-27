#include "smartrainharvest.h"
#include "ui_smartrainharvest.h"
#include "noaaweatherfetcher.h"
#include "chartcontainer.h"
#include <QMap>
#include <QTimer>
#include <DistanceSensor.h>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>
#include <wiringPi.h>

//QT_CHARTS_USE_NAMESPACE

SmartRainHarvest::SmartRainHarvest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartRainHarvest)
{
    ui->setupUi(this);


    //Latitude 38.9072° N
    //Longitude: 77.0369 W

    QSplitter *splitter = new QSplitter(Qt::Vertical, this);




    QTimer *CheckWeatherTimer = new QTimer();


    connect(CheckWeatherTimer,SIGNAL(timeout()), this, SLOT(on_Check_Timer()));
    connect(ReleaseTimer,SIGNAL(timeout()), this, SLOT(on_Check_Distance()));

    CheckWeatherTimer->start(Check_Weather_Interval*1000);


    splitter->addWidget(ProbnQuanChartContainer->GetChartView());
    QSplitter *hsplitter = new QSplitter(Qt::Horizontal, splitter);
    hsplitter->addWidget(CummulativeForcastChartContainer->GetChartView());
    hsplitter->addWidget(WaterDepthChartContainer->GetChartView());
    splitter->addWidget(OpenShutChartContainer->GetChartView());

    QWidget* layoutWidget1 = new QWidget(hsplitter);
    QVBoxLayout *buttonslayout = new QVBoxLayout(layoutWidget1);
    ManualOpenShut = new QPushButton(splitter);
    buttonslayout->addWidget(ManualOpenShut);
    ManualOpenShut->setText("Open Valve");
    DistanceLbl = new QLabel(splitter);
    buttonslayout->addWidget(DistanceLbl);
    connect(ManualOpenShut, SIGNAL(clicked()),this, SLOT(on_ManualOpenShut()) );
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    splitter->setStretchFactor(2, 1);
    hsplitter->setStretchFactor(0, 3);
    hsplitter->setStretchFactor(1, 3);
    hsplitter->setStretchFactor(2, 1);


    setCentralWidget(splitter);
    distancesensor.initialize();
    pinMode(18, OUTPUT);
    on_Check_Timer();

}

SmartRainHarvest::~SmartRainHarvest()
{
    delete ui;
}

void SmartRainHarvest::on_Check_Timer()
{

    qDebug()<<"Checking weather!";
    QMap<QString, QVector<WeatherData>> precip_data;

    QVector<WeatherData> rainamountdata = fetcher.getWeatherPrediction(latitude, longitude, datatype::PrecipitationAmount);
    precip_data["Precipitation [mm]"] = rainamountdata;

    QVector<WeatherData> rainprobdata = fetcher.getWeatherPrediction(latitude, longitude, datatype::ProbabilityofPrecipitation);
    precip_data["Precipitation probability (%)"] = rainprobdata;

    QVector<WeatherData> tempdata = fetcher.getWeatherPrediction(latitude, longitude, datatype::Temperature);
    precip_data["Temperature (<sup>o</sup>C)"] = tempdata;

    ProbnQuanChartContainer->plotWeatherDataMap(precip_data);
    ProbnQuanChartContainer->GetChartView()->setRenderHint(QPainter::Antialiasing);

    if (cummulativerain.count()>30) cummulativerain.removeFirst();

    cummulativerain.append({QDateTime::currentDateTime(),calculateCumulativeValue(rainamountdata,2) });
    CummulativeForcastChartContainer->plotWeatherData(cummulativerain,"Cummulative rain forecast [mm]");

    double distance = distancesensor.getDistance();
    qDebug()<<"Distance = " << distance << " cm";
    DistanceLbl->setText(QString::number(distance));
    if (depth.count()>30) depth.removeFirst();
    depth.append({QDateTime::currentDateTime(), max_distance - distance});
    WaterDepthChartContainer->plotWeatherData(depth, "Water Depth (cm)");

    if (depth.last().value>waterdepthcriteria && cummulativerain.last().value>cummulativeraincriteria)
    {
        overflow = false;
        StartRelease();
    }

    if (depth.last().value>bypassdepthcriteria)
    {
        overflow = true;
        StartRelease();
    }


    if (openshut.count()>100) openshut.removeFirst();
    openshut.append({QDateTime::currentDateTime(), double(int(state))});
    OpenShutChartContainer->plotWeatherData(openshut, "Valve State (on/off)");
    if (!state)
        ManualOpenShut->setText("Open the Valve");
    else
        ManualOpenShut->setText("Shut the Valve");

}

void SmartRainHarvest::StartRelease()
{
    ReleaseTimer->start(10000);
}

void SmartRainHarvest::on_Check_Distance()
{
    double distance = distancesensor.getDistance();
    if (depth.count()>30) depth.removeFirst();
    depth.append({QDateTime::currentDateTime(), max_distance - distance});
    WaterDepthChartContainer->plotWeatherData(depth, "Water Depth (cm)");



    if (openshut.count()>100) openshut.removeFirst();
    openshut.append({QDateTime::currentDateTime(), double(state)});
    OpenShutChartContainer->plotWeatherData(openshut, "Valve State (on/off");

    if (depth.last().value<(overflow?depthtoreleaseto:minumumdepth))
    {
        ReleaseTimer->stop();
        ShutTheValve();
        state = false;
    }
    else
    {
        OpenTheValve();
        state=true;
    }
}

void SmartRainHarvest::OpenTheValve(){

    digitalWrite(18, HIGH);
    qDebug()<<"The valve is now open";
}

void SmartRainHarvest::ShutTheValve(){

    digitalWrite(18, LOW);
    qDebug()<<"The valve is now shut";
}

void SmartRainHarvest::on_ManualOpenShut()
{
    if (state)
    {
        ShutTheValve();
        state = false;
    }
    else
    {
        OpenTheValve();
        state = true;
    }
    if (!state)
        ManualOpenShut->setText("Open the Valve");
    else
        ManualOpenShut->setText("Shut the Valve");

    if (openshut.count()>100) openshut.removeFirst();
    openshut.append({QDateTime::currentDateTime(), double(int(state))});
    OpenShutChartContainer->plotWeatherData(openshut, "Valve State (on/off)");
}



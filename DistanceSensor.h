#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include <QObject>
#include <QDebug>
#include <QThread>
#ifdef RasPi
#include <wiringPi.h>
#endif

class DistanceSensor : public QObject {
    Q_OBJECT

public:
    DistanceSensor();
    ~DistanceSensor();

    bool initialize();
    void cleanup();
    double getDistance();

private:
    const int TRIG_PIN = 4; // BCM pin 4
    const int ECHO_PIN = 17; // BCM pin 17



};

#endif // DISTANCESENSOR_H

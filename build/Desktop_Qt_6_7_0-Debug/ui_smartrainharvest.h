/********************************************************************************
** Form generated from reading UI file 'smartrainharvest.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMARTRAINHARVEST_H
#define UI_SMARTRAINHARVEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SmartRainHarvest
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SmartRainHarvest)
    {
        if (SmartRainHarvest->objectName().isEmpty())
            SmartRainHarvest->setObjectName("SmartRainHarvest");
        SmartRainHarvest->resize(800, 600);
        centralwidget = new QWidget(SmartRainHarvest);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        SmartRainHarvest->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SmartRainHarvest);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        SmartRainHarvest->setMenuBar(menubar);
        statusbar = new QStatusBar(SmartRainHarvest);
        statusbar->setObjectName("statusbar");
        SmartRainHarvest->setStatusBar(statusbar);

        retranslateUi(SmartRainHarvest);

        QMetaObject::connectSlotsByName(SmartRainHarvest);
    } // setupUi

    void retranslateUi(QMainWindow *SmartRainHarvest)
    {
        SmartRainHarvest->setWindowTitle(QCoreApplication::translate("SmartRainHarvest", "SmartRainHarvest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SmartRainHarvest: public Ui_SmartRainHarvest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMARTRAINHARVEST_H

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CameraDeSurveillance.h"
#include <QSerialPort>
#include "ControleCamera.h"

class CameraDeSurveillance : public QMainWindow
{
    Q_OBJECT

private:
    Ui::CameraDeSurveillanceClass ui;
    ControleCamera* controleCamera;
    QSerialPort* port;
    bool waitingForConfirmation = false;
    void setupConnections();

public:
    CameraDeSurveillance(QWidget* parent = nullptr);
    ~CameraDeSurveillance();

private slots:
    void openPort();
    void camInitialisation();
    void powerOn();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void autoMode();  
    void ChangeLanguage();
};

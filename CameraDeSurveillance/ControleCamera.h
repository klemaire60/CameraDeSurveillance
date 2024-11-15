#pragma once

#include <QSerialPort>
#include <QTimer>

class ControleCamera
{
private:
    QSerialPort* port = nullptr;
    bool isportOpen = false;

public:
    ControleCamera();
    ~ControleCamera();

public slots:
    bool waitingForConfirmation = false;

    bool openPort(QSerialPort* newPort);
    void camInitialisation();
    void powerON();
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void autoMode();
    void adjustZoom(int zoomValue);

private:
    bool checkPort();
    bool writeToPort(const QString& command);
    void onSerialPortReadyRead();
};

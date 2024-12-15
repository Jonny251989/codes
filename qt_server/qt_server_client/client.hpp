#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <iostream>
#include "item.hpp"  
#include <thread>
#include <chrono>
#include <QTimer>
#include <QThread>

class GameClient : public QObject {
    Q_OBJECT
public:
    GameClient();
    void connectToServer();
    void sendCoordinates(int x, int y);

private slots:
    void onConnected();
    void onDataReceived();
    void requestInput();

private:
    QTcpSocket* socket;
    bool waitingForInput;  // Флаг, который указывает, ожидаем ли мы ввода
};
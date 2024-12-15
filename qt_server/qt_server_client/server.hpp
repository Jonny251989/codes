#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QByteArray>   
#include "item.hpp"

class GameServer : public QObject {
    Q_OBJECT
public:
    GameServer();
    void start();

private slots:
    void onNewConnection();
    void onDataReceived();

private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;
};
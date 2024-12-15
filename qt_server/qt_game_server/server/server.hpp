#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QByteArray>   
#include "item.hpp"

class GameServer : public QTcpServer {
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    ~GameServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onDataReceived();

private:
    QList<QTcpSocket*> clients;  // Список подключенных клиентов
    int currentPlayerIndex;      // Индекс текущего игрока
};
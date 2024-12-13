#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "item.hpp"

class Server : public QObject {
    Q_OBJECT

public:
    Server(quint16 port);
    virtual ~Server();

public slots:
    void onNewConnection();
    void onClientMessage();

private:
    QTcpServer server;
    QList<QTcpSocket*> clients;
};

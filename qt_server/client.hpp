#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include <QTimer>
#include <iostream>
#include <list>
#include "item.hpp"


class Client : public QObject {
    Q_OBJECT

public:
    Client(const QString& host, quint16 port);
    void sendItem(const Item& item);
    virtual ~Client();

public slots:
    void onConnected();
    void onReadyRead();
    void onSendPeriodicItem();

private:
    QTcpSocket *socket;
    std::list<Item> items;  // Храним полученные Items
    QTimer *sendTimer;      // Таймер для периодической отправки данных
};
#include "client.hpp"

Client::Client(const QString& host, quint16 port){
    socket = new QTcpSocket(this);
    socket->connectToHost(host, port);
    
    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    
    // Таймер для периодической отправки данных
    sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &Client::onSendPeriodicItem);
    sendTimer->start(2000);  // Отправка каждый 2 секунды
}

void Client::sendItem(const Item& item) {
    QByteArray data = item.serialize();
    socket->write(data);
}

void Client::onConnected() {
    std::cout << "Connected to server!" << std::endl;
}

void Client::onReadyRead() {
    QByteArray data = socket->readAll();
    Item item = Item::deserialize(data);
    items.push_back(item);
    std::cout << "Received Item: " << item.id << " " << item.name.toStdString() << " " << item.value << std::endl;
}

void Client::onSendPeriodicItem() {
    static int counter = 1;
    Item item = { counter, "Item " + QString::number(counter), 10.5f * counter };
    sendItem(item);
    counter++;
}

Client::~Client(){
    delete socket;
    delete sendTimer;
};
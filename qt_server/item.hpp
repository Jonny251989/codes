#pragma once

#include <QString>
#include <QByteArray>
#include <QDataStream>

struct Item {
    int id;
    QString name;
    float value;

    // Сериализация
    QByteArray serialize() const {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << id << name << value;
        return data;
    }

    // Десериализация
    static Item deserialize(const QByteArray& data) {
        Item item;
        QDataStream in(data);
        in >> item.id >> item.name >> item.value;
        return item;
    }
};
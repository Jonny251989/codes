#pragma once

#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <iostream>
#include <QIODevice>
#include <QString>
#include <QByteArray>

#include <QDataStream>

// Простая структура для представления хода
struct Coordinate {
    int x;
    int y;

    // Конструктор для удобства
    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}
};

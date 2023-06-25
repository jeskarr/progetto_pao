#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QString>
#include <QVector>
#include <QDir>


enum err_types {
    FILE_IO_ERROR,
    EMPTY_FILE,
};

QJsonDocument read(const QString path);
void write(const QString path, QJsonDocument& doc);

#endif // MEMORYMANAGEMENT_H

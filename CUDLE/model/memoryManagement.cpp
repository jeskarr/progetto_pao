#include "memoryManagement.h"

QJsonDocument read(const QString path)
{
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly)){
        throw FILE_IO_ERROR;
    }
    QByteArray data = f.readAll();
    f.close();

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
    if (doc.isNull()){
        throw EMPTY_FILE;
    }
    return doc;
}


void write(const QString path, QJsonDocument& doc)
{
    QFile f(path);
    if(!f.open(QIODevice::WriteOnly)){
        throw FILE_IO_ERROR;
    }
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);
    f.write(bytes);
    f.close();
}


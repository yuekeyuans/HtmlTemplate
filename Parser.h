#pragma once

#include <QtCore>

struct Node
{
    virtual QString operator ()(QJsonValue) = 0;

    QJsonValue getValue(const QString& path, QJsonValue);
};

class Parser
{
public:
    Parser();

public:
    void parse(QString content);

private:
    QPair<Node*, QString> parseHtml(QString);
    QPair<Node*, QString> parsePlain(QString);
    QPair<Node*, QString> parseIf(QString content);
    QPair<Node*, QString> parseFor(QString content);
    QPair<Node*, QString> parseVar(QString content);

private:
    bool isCurrent(const QString& val, const QString& type);
    bool isCurrentPlain(QString);
    bool isCurrentIf(QString);
    bool isCurrentFor(QString);
    bool isCurrentVar(QString);
    bool isCurrentEnd(QString);

private:
    bool isVariableValid(const QString&);
    QPair<QString, QString> readVariable(QString content);
    QString eatVariable(const QString& content, QString val);

private:
    QString m_content;
};


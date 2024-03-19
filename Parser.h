#pragma once

#include <QtCore>

struct Node
{
    virtual QString operator ()(QJsonValue) = 0;
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
    bool isVaribleValid(const QString&);

private:
    QString m_content;
};


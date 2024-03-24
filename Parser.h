#pragma once

#include <QtCore>
#include "Node.h"

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
    QPair<Node*, QString> parseElif(QString content);
    QPair<Node*, QString> parseElse(QString content);
    QPair<Node*, QString> parseFor(QString content);
    QPair<Node*, QString> parseVar(QString content);

private:
    bool isCurrent(const QString& val, const QString& type);
    bool isCurrentIf(QString);
    bool isCurrentFor(QString);
    bool isCurrentVar(QString);
    bool isCurrentEnd(QString);

private:
    bool checkVariableValid(const QString&);
    QPair<QString, QString> readVariable(QString content, const QString& failReason);
    QString eatVariable(QString content, QString val);

private:
    QString m_content;
};


#pragma once

#include <QtCore>
#include "Nody.h"

class NodyParser
{
public:
    NodyParser();

public:
    void parse(QString content);

private:
    QPair<Nody*, QString> parseHtml(QString);
    QPair<Nody*, QString> parsePlain(QString);
    QPair<Nody*, QString> parseIf(QString content);
    QPair<Nody*, QString> parseElif(QString content);
    QPair<Nody*, QString> parseElse(QString content);
    QPair<Nody*, QString> parseFor(QString content);
    QPair<Nody*, QString> parseVar(QString content);

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


#pragma once

#include "Parser.h"

struct UnionNode : public Node
{
public:
    UnionNode(QList<Node*> nodes);
    virtual QString operator ()(QJsonValue root, QMap<QString, QJsonValue> context) final;

public:
    QList<Node*> m_nodes;
};

struct HtmlNode : public Node
{
public:
    HtmlNode(const QString& html);
    virtual QString operator ()(QJsonValue, QMap<QString, QJsonValue>) final;
public:
    QString m_html;
};

struct VariableNode : public Node
{
    VariableNode(const QString& path);
    virtual QString operator ()(QJsonValue, QMap<QString, QJsonValue>) final;
public:
    QString m_path;
};

struct IfNode : public Node
{
    virtual QString operator ()(QJsonValue, QMap<QString, QJsonValue>) final;
public:
    QString m_condition;
    Node* m_ifOps{};
    Node* m_elseOps{};
};

struct ForNode : public Node
{
    virtual QString operator ()(QJsonValue, QMap<QString, QJsonValue>) final;
public:
    QString m_iterator;
    QString m_path;
    Node* m_loopContent;
};


class ParserException : std::exception
{
public:
    ParserException(const QString& error);
    virtual const char *what() const final;

public:
    void addTrace(const QString& content);

private:
    QStringList m_error;
};

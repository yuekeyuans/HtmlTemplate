#pragma once

#include <QtCore>

struct Node
{
public:
    virtual ~Node() = default;
    // 使用引用，因为传递不需要拷贝，但是如果在函数内改变该内容了，则是需要拷贝备份的。
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) = 0;
    QJsonValue getValue(const QString& path, const QJsonValue&);
    QJsonValue getValue(const QString& path, const QMap<QString, QJsonValue>&);
};

struct UnionNode : public Node
{
public:
    UnionNode(QList<Node*> nodes);
    ~UnionNode();
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;

public:
    QList<Node*> m_nodes;
};

struct HtmlNode : public Node
{
public:
    HtmlNode(const QString& html);
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_html;
};

struct VariableNode : public Node
{
    VariableNode(const QString& path);
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_path;
};

struct IfNode : public Node
{
    IfNode() = default;
    ~IfNode();
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_condition;
    Node* m_ifOps{};
    Node* m_elseOps{};
};

struct ForNode : public Node
{
    ForNode() = default;
    ~ForNode();
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_iterator;
    QString m_path;
    Node* m_loopContent;
};

class ParserException : std::exception
{
public:
    ParserException(const QString& error, const QString& content);
    virtual const char *what() const final;

public:
    void addTrace(const QString& error, const QString& content);
    QString getTraces();

private:
    QString makeTrace(const QString& error, const QString& content);

private:
    QStringList m_error;
};


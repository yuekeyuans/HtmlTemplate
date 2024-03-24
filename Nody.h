#pragma once

#include <QtCore>

struct Nody
{
public:
    virtual ~Nody() = default;
    // 使用引用，因为传递不需要拷贝，但是如果在函数内改变该内容了，则是需要拷贝备份的。
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) = 0;
    QJsonValue getValue(const QString& path, const QJsonValue&);
    QJsonValue getValue(const QString& path, const QMap<QString, QJsonValue>&);
};

struct UnionNody : public Nody
{
public:
    UnionNody(QList<Nody*> nodes);
    ~UnionNody();
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;

public:
    QList<Nody*> m_nodes;
};

struct HtmlNody : public Nody
{
public:
    HtmlNody(const QString& html);
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_html;
};

struct VariableNody : public Nody
{
    VariableNody(const QString& path);
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_path;
};

struct IfNody : public Nody
{
    IfNody() = default;
    ~IfNody();
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_condition;
    Nody* m_ifOps{};
    Nody* m_elseOps{};
};

struct ForNody : public Nody
{
    ForNody() = default;
    ~ForNody();
    virtual QString operator ()(const QJsonValue&, QMap<QString, QJsonValue>&) final;
public:
    QString m_iterator;
    QString m_path;
    Nody* m_loopContent;
};

class NodyException : std::exception
{
public:
    NodyException(const QString& error, const QString& content);
    virtual const char *what() const final;

public:
    void addTrace(const QString& error, const QString& content);
    QString getTraces();

private:
    QString makeTrace(const QString& error, const QString& content);

private:
    QStringList m_error;
};


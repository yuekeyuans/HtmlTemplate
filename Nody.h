#pragma once

#include "core/base/IHeaderUtil.h"

$PackageWebCoreBegin

struct Nody
{
public:
    virtual ~Nody() = default;
    virtual QString execute(const QJsonObject&, QMap<QString, QJsonObject>&) = 0;         // 使用引用，因为传递不需要拷贝，但是如果在函数内改变该内容了，则是需要拷贝备份的。
    QJsonValue getValue(const QString& path, const QJsonObject&);
    QJsonValue getValue(const QString& path, const QMap<QString, QJsonObject>&);
};

struct UnionNody : public Nody
{
public:
    UnionNody(QList<Nody*> nodes);
    ~UnionNody();
    virtual QString execute(const QJsonObject&, QMap<QString, QJsonObject>&) final;

public:
    QList<Nody*> m_nodes;
};

struct HtmlNody : public Nody
{
public:
    HtmlNody(const QString& html);
    virtual QString execute(const QJsonObject&, QMap<QString, QJsonObject>&) final;
public:
    QString m_html;
};

struct VariableNody : public Nody
{
    VariableNody(const QString& path);
    virtual QString execute(const QJsonObject&, QMap<QString, QJsonObject>&) final;
public:
    QString m_path;
};

struct IfNody : public Nody
{
    IfNody() = default;
    ~IfNody();
    virtual QString execute(const QJsonObject&, QMap<QString, QJsonObject>&) final;
public:
    QString m_condition;
    Nody* m_ifNode{};
    Nody* m_elseNode{};
};

struct ForNody : public Nody
{
    ForNody() = default;
    ~ForNody();
    virtual QString execute(const QJsonObject&, QMap<QString, QJsonObject>&) final;
public:
    QString m_iterator;
    QString m_path;
    Nody* m_loopNode;
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

$PackageWebCoreEnd


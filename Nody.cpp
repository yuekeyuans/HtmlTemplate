#include "Nody.h"

QJsonValue Nody::getValue(const QString &, const QJsonValue&)
{
    return {};
}

QJsonValue Nody::getValue(const QString &, const QMap<QString, QJsonValue>&)
{
    return {};
}

UnionNody::UnionNody(QList<Nody *> nodes) : m_nodes(nodes)
{
}

UnionNody::~UnionNody()
{
    for(auto node: m_nodes){
        delete node;
    }
}

QString UnionNody::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context){
    QString ret;
    for(auto node : m_nodes){
        ret.append(node->operator ()(root, context));
    }
    return ret;
}

HtmlNody::HtmlNody(const QString &html) : m_html(html){}

QString HtmlNody::operator ()(const QJsonValue&, QMap<QString, QJsonValue>&){
    return m_html;
}

VariableNody::VariableNody(const QString &path) : m_path(path){}

QString VariableNody::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context){
    auto value = getValue(m_path, context);
    if(value.isNull() || value.isUndefined()){
        value = getValue(m_path, root);
    }
    return value.toString();
}

IfNody::~IfNody()
{
    delete m_ifOps;
    delete m_elseOps;
}

QString IfNody::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context)
{
    if(true){
        if(m_ifOps){
            return m_ifOps->operator ()(root, context);
        }
    }else{
        if(m_elseOps){
            return m_elseOps->operator ()(root, context);
        }
    }
    return "";
}

ForNody::~ForNody()
{
    delete m_loopContent;
    m_loopContent = nullptr;
}

QString ForNody::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context)
{
    return m_loopContent->operator ()(root, context);
}

NodyException::NodyException(const QString &error, const QString& content)
{
    m_error.append(makeTrace(error, content));
}

const char *NodyException::what() const
{
    return m_error.join("\n").toUtf8();
}

void NodyException::addTrace(const QString &error, const QString& content)
{
    m_error.append(makeTrace(error, content));
}

QString NodyException::getTraces()
{
    return m_error.join(" \n");
}

QString NodyException::makeTrace(const QString &error, const QString &content)
{
    QString brief = content.left(40);
    QString ret = QString("ERROR: ").append(error).append(" NEAR: ").append(brief);
    if(brief != content){
        ret.append("...");
    }
    return ret;
}

#include "Nody.h"
#include "core/base/IJsonUtil.h"

$PackageWebCoreBegin

QJsonValue Nody::getValue(const QString & path, const QJsonObject& obj)
{
    if(path.isEmpty()){
        return {};
    }
    QStringList args = path.split(".");
    if(!obj.contains(args.first())){
        return {};
    }
    QJsonValue temp = obj[args.first()];
    args.pop_front();
    for(auto arg : args){
        if(!temp.isObject()){
            return {};
        }
        auto obj = temp.toObject();
        if(!obj.contains(arg)){
            return {};
        }
        temp = obj[arg];
    }
    return temp;
}

QJsonValue Nody::getValue(const QString &path, const QMap<QString, QJsonObject>&map)
{
    if(path.isEmpty()){
        return {};
    }
    QStringList args = path.split(".");
    if(!map.contains(args.first())){
        return {};
    }
    const auto& obj = map[args.first()];
    args.pop_front();
    getValue(args.join("."), obj);
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

QString UnionNody::execute(const QJsonObject& root, QMap<QString, QJsonObject>& context){
    QString ret;
    for(auto node : m_nodes){
        ret.append(node->execute(root, context));
    }
    return ret;
}

HtmlNody::HtmlNody(const QString &html) : m_html(html){}

QString HtmlNody::execute(const QJsonObject&, QMap<QString, QJsonObject>&){
    return m_html;
}

VariableNody::VariableNody(const QString &path) : m_path(path){}

QString VariableNody::execute(const QJsonObject& root, QMap<QString, QJsonObject>& context){
    auto value = getValue(m_path, context);
    if(value.isNull() || value.isUndefined()){
        value = getValue(m_path, root);
    }
    return value.toString();
}

IfNody::~IfNody()
{
    delete m_ifNode;
    delete m_elseNode;
}

QString IfNody::execute(const QJsonObject& root, QMap<QString, QJsonObject>& context)
{
    if(true){
        if(m_ifNode){
            return m_ifNode->execute(root, context);
        }
    }else{
        if(m_elseNode){
            return m_elseNode->execute(root, context);
        }
    }
    return "";
}

ForNody::~ForNody()
{
    delete m_loopNode;
    m_loopNode = nullptr;
}

QString ForNody::execute(const QJsonObject& root, QMap<QString, QJsonObject>& context)
{
    return m_loopNode->execute(root, context);
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

$PackageWebCoreEnd

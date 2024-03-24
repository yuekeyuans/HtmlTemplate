#include "Node.h"

QJsonValue Node::getValue(const QString &, const QJsonValue&)
{
    return {};
}

QJsonValue Node::getValue(const QString &, const QMap<QString, QJsonValue>&)
{
    return {};
}

UnionNode::UnionNode(QList<Node *> nodes) : m_nodes(nodes)
{
}

UnionNode::~UnionNode()
{
    for(auto node: m_nodes){
        delete node;
    }
}

QString UnionNode::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context){
    QString ret;
    for(auto node : m_nodes){
        ret.append(node->operator ()(root, context));
    }
    return ret;
}

HtmlNode::HtmlNode(const QString &html) : m_html(html){}

QString HtmlNode::operator ()(const QJsonValue&, QMap<QString, QJsonValue>&){
    return m_html;
}

VariableNode::VariableNode(const QString &path) : m_path(path){}

QString VariableNode::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context){
    auto value = getValue(m_path, context);
    if(value.isNull() || value.isUndefined()){
        value = getValue(m_path, root);
    }
    return value.toString();
}

IfNode::~IfNode()
{
    delete m_ifOps;
    delete m_elseOps;
}

QString IfNode::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context)
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

ForNode::~ForNode()
{
    delete m_loopContent;
    m_loopContent = nullptr;
}

QString ForNode::operator ()(const QJsonValue& root, QMap<QString, QJsonValue>& context)
{
    return m_loopContent->operator ()(root, context);
}

ParserException::ParserException(const QString &error, const QString& content)
{
    m_error.append(makeTrace(error, content));
}

const char *ParserException::what() const
{
    return m_error.join("\n").toUtf8();
}

void ParserException::addTrace(const QString &error, const QString& content)
{
    m_error.append(makeTrace(error, content));
}

QString ParserException::getTraces()
{
    return m_error.join(" \n");
}

QString ParserException::makeTrace(const QString &error, const QString &content)
{
    QString brief = content.left(40);
    QString ret = QString("ERROR: ").append(error).append(" NEAR: ").append(brief);
    if(brief != content){
        ret.append("...");
    }
    return ret;
}

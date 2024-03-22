#include "Node.h"

UnionNode::UnionNode(QList<Node *> nodes) : m_nodes(nodes){}

QString UnionNode::operator ()(QJsonValue root, QMap<QString, QJsonValue> context){
    QString ret;
    for(auto node : m_nodes){
        qDebug() << "union nodes";
        ret.append(node->operator ()(root, context));
    }
    return ret;
}

HtmlNode::HtmlNode(const QString &html) : m_html(html){}

QString HtmlNode::operator ()(QJsonValue root, QMap<QString, QJsonValue> context){
    return m_html;
}

VariableNode::VariableNode(const QString &path) : m_path(path){}

QString VariableNode::operator ()(QJsonValue root, QMap<QString, QJsonValue> context){
    qDebug() << "variable node" << m_path;
    return m_path + " value";
}

QString IfNode::operator ()(QJsonValue root, QMap<QString, QJsonValue> context)
{
    qDebug() << "if node: " + m_condition;
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

QString ForNode::operator ()(QJsonValue root, QMap<QString, QJsonValue> context)
{
    qDebug() << "for node" << m_path << m_iterator;
    return m_loopContent->operator ()(root, context);
}


#include "Node.h"

UnionNode::UnionNode(QList<Node *> nodes) : m_nodes(nodes){}

QString UnionNode::operator ()(QJsonValue json){
    QString ret;
    for(auto node : m_nodes){
        qDebug() << "union nodes";
        ret.append(node->operator ()(json));
    }
    return ret;
}

HtmlNode::HtmlNode(const QString &html) : m_html(html){}

QString HtmlNode::operator ()(QJsonValue){
    qDebug() << "html node";
    return m_html;
}

VariableNode::VariableNode(const QString &path) : m_path(path){}

QString VariableNode::operator ()(QJsonValue){
    qDebug() << "variable node" << m_path;
    return m_path + " value";
}

QString IfNode::operator ()(QJsonValue value)
{
    qDebug() << "if node: " + m_condition;
    if(true){
        if(m_ifOps){
            return m_ifOps->operator ()(value);
        }
    }else{
        if(m_elseOps){
            return m_elseOps->operator ()(value);
        }
    }
    return "";
}

QString ForNode::operator ()(QJsonValue value)
{
    qDebug() << "for node" << m_path << m_iterator;
    return m_loopContent->operator ()(value);
}


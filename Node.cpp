#include "Node.h"

UnionNode::UnionNode(QList<Node *> nodes) : m_nodes(nodes){}

QString UnionNode::operator ()(QJsonValue json){
    QString ret;
    for(auto node : m_nodes){
        ret.append((*node)(json));
    }
    return ret;
}

HtmlNode::HtmlNode(const QString &html) : m_html(html){}

QString HtmlNode::operator ()(QJsonValue){
    return m_html;
}

VariableNode::VariableNode(const QString &path) : m_path(path){}

QString VariableNode::operator ()(QJsonValue){
    return m_path + " value";
}

QString IfNode::operator ()(QJsonValue value)
{
    qDebug() << "if condition: " + m_condition;
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


QString ForNode::operator ()(QJsonValue)
{
    return "hello world";
}


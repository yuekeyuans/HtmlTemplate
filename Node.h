#pragma once

#include "Parser.h"

struct UnionNode : public Node
{
public:
    UnionNode(QList<Node*> nodes) : m_nodes(nodes){}
    virtual QString operator ()(QJsonValue json) final{
        QString ret;
        for(auto node : m_nodes){
            ret.append((*node)(json));
        }
        return ret;
    }

public:
    QList<Node*> m_nodes;
};

struct HtmlNode : public Node{
public:
    HtmlNode(const QString& html) : m_html(html){}
    virtual QString operator ()(QJsonValue) final{
        return m_html;
    }
public:
    QString m_html;
};

struct VariableNode : public Node{
public:
    VariableNode(const QString& path) : m_path(path){}
    virtual QString operator ()(QJsonValue) final{
        return m_path + " value";
    }
public:
    QString m_path;
};


#include "Parser.h"
#include "Node.h"

#include "QtCore"

Parser::Parser()
{

}

void Parser::parse(QString content)
{
    auto values = parseHtml(content);
    qDebug() << values.first << values.second;
//    qDebug() << *(values.first)({}) << values.second;
}

QPair<Node*, QString> Parser::parseHtml(QString content)
{
    QList<Node*> nodes;
    while(!content.isEmpty()){
        if(isCurrentPlain(content)){
            auto ret = parsePlain(content);
            nodes.append(ret.first);
            content = ret.second;
        }
        else if(isCurrentIf(content)){
            auto ret = parseIf(content);
            nodes.append(ret.first);
            content = ret.second;
        }
        else if(isCurrentFor(content)){
            auto ret = parseFor(content);
            nodes.append(ret.first);
            content = ret.second;
        }
        else if(isCurrentVar(content)){
            auto ret = parseVar(content);
            nodes.append(ret.first);
            content = ret.second;
        }else{
            qFatal("error content");
        }
    }

    return {new UnionNode(nodes), content};
}

QPair<Node *, QString> Parser::parsePlain(QString)
{
    return {};
}

QPair<Node *, QString> Parser::parseIf(QString content)
{

    return {};
}

QPair<Node *, QString> Parser::parseFor(QString content)
{

    return {};
}

QPair<Node *, QString> Parser::parseVar(QString content)
{
    content = content.mid(2);
    QString args;
    int len = content.length();
    for(int i=0; i<len; i++){
        if(content[i] == "}"){
            args = content.mid(0, i-1).trimmed();
            if(args.isEmpty()){
                qFatal("error");
            }
            auto node = new VariableNode(args);
            return {node, content.mid(i)};
        }
    }
    qFatal("erorr");
    return {};
}

bool Parser::isCurrentPlain(QString)
{

    return {};
}

bool Parser::isCurrentIf(QString)
{

    return {};
}

bool Parser::isCurrentFor(QString)
{

    return {};
}

bool Parser::isCurrentVar(QString content)
{
    if(content.startsWith("${")){
        return true;
    }
    return false;
}

bool Parser::isCurrentEnd(QString)
{

    return {};
}

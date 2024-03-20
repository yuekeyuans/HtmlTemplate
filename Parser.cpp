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
    qDebug() << values.first->operator ()({}) << values.second;
}

QPair<Node*, QString> Parser::parseHtml(QString content)
{
    content = content.trimmed();
    QList<Node*> nodes;
    while(!content.isEmpty()){
        if(isCurrentIf(content)){
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
        }else if(isCurrentEnd(content)){
            break;     // 表示该结束了，这里直接返回
        }else {
            auto ret = parsePlain(content);
            nodes.append(ret.first);
            content = ret.second;
        }
    }

    return {new UnionNode(nodes), content};
}

QPair<Node *, QString> Parser::parsePlain(QString content)
{
    auto index = content.indexOf("$");
    auto node = new HtmlNode(content.mid(0, index));
    return {node, content.mid(index+1)};
}

QPair<Node *, QString> Parser::parseIf(QString content)
{

    return {};
}

QPair<Node *, QString> Parser::parseFor(QString content)
{
    if(!content.startsWith("$for ")){
        qFatal("$for should with space");
    }

    auto node = new ForNode();

    content = content.mid(4);
    auto ret = readVariable(content);
    node->m_iterator = ret.first;
    content = ret.second;

    content = eatVariable(content, "in");

    auto val = readVariable(content);
    node->m_path = val.first;
    content = val.second;

    content = eatVariable(content, "{{");

    auto contentVal = parseHtml(content);
    node->m_loopContent = contentVal.first;
    content =contentVal.second;
    content = eatVariable(content, "}}");

    return {node, content};
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
                qFatal("empty error");
            }
            if(!isVariableValid(args)){
                qFatal("not match");
            }

            auto node = new VariableNode(args);
            return {node, content.mid(i+1)};
        }
    }
    qFatal("erorr");
    return {};
}

bool Parser::isCurrent(const QString &val, const QString &type)
{
    return val.startsWith(type);
}

bool Parser::isCurrentPlain(QString content)
{
    return !isCurrentIf(content)
            && !isCurrentFor(content)
            && !isCurrentVar(content)
            && !isCurrentEnd(content);
}

bool Parser::isCurrentIf(QString content)
{
    return isCurrent(content, "$if");
}

bool Parser::isCurrentFor(QString content)
{
    return isCurrent(content, "$for");
}

bool Parser::isCurrentVar(QString content)
{
    return isCurrent(content, "${");
}

bool Parser::isCurrentEnd(QString content)
{
    content = content.trimmed();
    return isCurrent(content, "}") || isCurrent(content, "}}");
}

bool Parser::isVariableValid(const QString &value)
{
    QRegExp regExp("^[a-zA-Z]+(\\.[a-zA-Z]+)*$");
    return regExp.exactMatch(value);
}

QPair<QString, QString> Parser::readVariable(QString content)
{
    content = content.trimmed();
    auto index1 = content.indexOf(' ');
    index1 = index1 == -1? std::numeric_limits<int>::max() : index1;
    auto index2 = content.indexOf('}');
    index2 = index2 == -1 ? std::numeric_limits<int>::max() : index2;
    auto pos = std::min(index1, index2);
    if(pos == std::numeric_limits<int>::max()){
        qFatal("content is not vaild for parse args");
    }

    auto text = content.mid(0, pos).trimmed();
    if(!isVariableValid(text)){
        qFatal("variable not valid");
    }
    return {text, content.mid(pos+1)};
}

QString Parser::eatVariable(const QString &content, QString val)
{
    if(!content.trimmed().startsWith(val)){
        qFatal("error, can not eat ", val.toUtf8());
    }
    return content.trimmed().mid(val.length());
}

QJsonValue Node::getValue(const QString &path, QJsonValue)
{
    return {};
}

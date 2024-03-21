#include "Parser.h"
#include "Node.h"

#include <QtCore>

Parser::Parser()
{
}

void Parser::parse(QString content)
{
    auto values = parseHtml(content);
    qDebug() << values.first->operator ()({}, {}) << values.second;
}

QPair<Node*, QString> Parser::parseHtml(QString content)
{
    content = content.trimmed();
    QList<Node*> nodes;
    while(!content.isEmpty() && !isCurrentEnd(content)){
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
        }else {
            auto ret = parsePlain(content);
            nodes.append(ret.first);
            content = ret.second;
        }
    }

    if(nodes.isEmpty()){
        return {nullptr, content};
    }
    if(nodes.length() == 1){
        return {nodes.first(), content};
    }else{
        return {new UnionNode(nodes), content};
    }
}

QPair<Node *, QString> Parser::parsePlain(QString content)
{
    auto index1 = content.indexOf("$");
    index1 = index1 == -1 ? std::numeric_limits<int>::max() : index1;
    auto index2 = content.indexOf("}");
    index2 = index2 == -1 ? std::numeric_limits<int>::max() : index2;
    auto pos = std::min(index1, index2);

    auto node = new HtmlNode(content.mid(0, pos));
    return {node, content.mid(pos)};
}

QPair<Node *, QString> Parser::parseIf(QString content)
{
    content = content.trimmed();
    if(!content.startsWith("$if ")){
        qFatal("$if should with space");
    }

    auto node = new IfNode;
    content = content.mid(4).trimmed();
    auto condition = readVariable(content, "$if statement can not read condition");
    node->m_condition = condition.first;
    content = condition.second;

    content = eatVariable(content, "{{");
    auto contentVal = parseHtml(content);
    node->m_ifOps = contentVal.first;
    content = contentVal.second;

    content = eatVariable(content, "}}").trimmed();
    if(content.startsWith("$elif ")){
        auto val = parseElif(content);
        node->m_elseOps = val.first;
        content = val.second;
    }else if(content.startsWith("$else")){
        auto val = parseElse(content);
        node->m_elseOps = val.first;
        content = val.second;
    }

    return {node, content};
}

QPair<Node *, QString> Parser::parseElif(QString content)
{
    content = content.trimmed();
    if(!content.startsWith("$elif ")){
        qFatal("$elif should with space");
    }

    auto node = new IfNode;
    content = content.mid(6).trimmed();
    auto condition = readVariable(content, "$elif statement can not read condition");
    node->m_condition = condition.first;
    content = condition.second;

    content = eatVariable(content, "{{");
    auto contentVal = parseHtml(content);
    node->m_ifOps = contentVal.first;
    content = contentVal.second;

    content = eatVariable(content, "}}").trimmed();
    if(content.startsWith("$elif")){
        auto val = parseElif(content);
        node->m_elseOps = val.first;
        content = val.second;
    }else if(content.startsWith("$else")){
        auto val = parseElse(content);
        node->m_elseOps = val.first;
        content = val.second;
    }

    content = eatVariable(content, "}}").trimmed();
    return {node, content};
}

QPair<Node *, QString> Parser::parseElse(QString content)
{
    content = content.trimmed();
    if(content.startsWith("$else")){
        qFatal("$elif should with space");
    }

    content = content.mid(5).trimmed();
    content = eatVariable(content, "{{");
    auto contentVal = parseHtml(content);
    auto node = contentVal.first;
    content = contentVal.second;
    content = eatVariable(content, "}}").trimmed();
    return {node, content};
}

QPair<Node *, QString> Parser::parseFor(QString content)
{
    if(!content.startsWith("$for ")){
        qFatal("$for should with space");
    }

    auto node = new ForNode();

    content = content.mid(4);
    auto ret = readVariable(content, "$for statement can not read loop iterator");
    node->m_iterator = ret.first;
    content = ret.second;

    content = eatVariable(content, "in");

    auto val = readVariable(content, "$for statement can not read loop body");
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
    content = content.trimmed();
    content = content.mid(2);
    QString args;
    int len = content.length();
    for(int i=0; i<len; i++){
        if(content[i] == "}"){
            args = content.mid(0, i).trimmed();
            if(args.isEmpty()){
                qFatal("empty error");
            }
            checkVariableValid(args);

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

bool Parser::checkVariableValid(const QString &value)
{
    QRegExp regExp("^[a-zA-Z]+(\\.[a-zA-Z]+)*$");
    bool valid = regExp.exactMatch(value);
    if(!valid){
        QString tip = "error occured when parse variable: " + value;
        qFatal(tip.toUtf8());
    }
    return valid;
}

QPair<QString, QString> Parser::readVariable(QString content, const QString& failReason)
{
    content = content.trimmed();
    auto index1 = content.indexOf(' ');     // TODO: 这里不仅是 空格，也有可能是 tab 或者 换行
    index1 = index1 == -1? std::numeric_limits<int>::max() : index1;
    auto index2 = content.indexOf('}');
    index2 = index2 == -1 ? std::numeric_limits<int>::max() : index2;
    auto index3 = content.indexOf("{");
    index3 = index3 == -1 ? std::numeric_limits<int>::max() : index3;
    auto pos = std::min({index1, index2, index3});
    if(pos == std::numeric_limits<int>::max()){
        qFatal("content is not vaild for parse any args");
    }

    auto text = content.mid(0, pos).trimmed();
    if(!checkVariableValid(text)){
        qFatal(failReason.toUtf8());
    }

    return {text, content.mid(pos)};
}

QString Parser::eatVariable(QString content, QString val)
{
    content = content.trimmed();
    if(!content.startsWith(val)){
        qFatal("error, can not eat ", val.toUtf8());
    }
    return content.mid(val.length());
}


QJsonValue Node::getValue(const QString &path, QJsonValue)
{
    return {};
}

QJsonValue Node::getValue(const QString &path, QMap<QString, QJsonValue>)
{
    return {};
}

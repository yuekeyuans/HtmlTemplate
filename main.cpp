#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    Parser parser;
    parser.parse("${ hello.world }");
    qDebug() << "end";
}

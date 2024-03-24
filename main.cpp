#include <QCoreApplication>
#include "NodyParser.h"

int main(int argc, char *argv[])
{
    NodyParser parser;

//    parser.parse("${ hello.world }");

    parser.parse("$for val in config.vals{{${val}<h1>uieleuai</h2> }}");

    parser.parse("$if error hello {{ <h1>hello world></h1> }}");

    qDebug() << "end";
}

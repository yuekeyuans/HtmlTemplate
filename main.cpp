﻿#include <QCoreApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    Parser parser;

//    parser.parse("${ hello.world }");

    parser.parse("$for val in config.vals {{  ${val} <h1>uieleuai</h2> }}");

    qDebug() << "end";
}

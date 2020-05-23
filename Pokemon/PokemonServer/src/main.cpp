#include <PreCompile.h>

#include "UnitTest.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    TestReflect();
    
    return app.exec();
}

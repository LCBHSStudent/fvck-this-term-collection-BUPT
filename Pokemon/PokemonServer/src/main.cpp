#include <PreCompile.h>

#include "UnitTest.hpp"

#include "ServerBackend/ServerBackend.h"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    std::unique_ptr<ServerBackend> backend(new ServerBackend);
    
    return app.exec();
}

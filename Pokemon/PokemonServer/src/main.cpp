#include <PreCompile.h>

#include "UnitTest.h"
#include "ServerBackend/ServerBackend.h"

#undef UNIT_TEST_FLAG
#ifdef UNIT_TEST_FLAG
int main(int argc, char * argv[]) {
    UnitTest::TestPokemonProperties();
}

#else
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    ServerBackend* pBackend = nullptr;
    
#ifdef Q_OS_WINDOWS
    // system("chcp 65001");
    // system("cls");
#endif
    pBackend = new ServerBackend;
    system("cls");
    qDebug() << "new server backend instance was built up";
    
    return app.exec();
}
#endif

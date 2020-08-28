#include <PreCompile.h>

#include "UnitTest.h"
#include "ServerBackend/ServerBackend.h"

#undef UNIT_TEST_FLAG
#ifdef UNIT_TEST_FLAG

#define TEST_STAGE_1

int main(int argc, char * argv[]) {
#ifdef TEST_STAGE_1
    
#endif
#ifdef TEST_STAGE_2
    
#endif
#ifdef TEST_STAGE_3
    
#endif
    (void)(argc);
    (void)(argv);
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
//    QMetaObject::connect(&app.destroyed,      &QCoreApplication::quit,
//                     pBackend,  &ServerBackend::~ServerBackend);
    
    system("cls");
    qDebug() << "new server backend instance was built up";
    
    int ret = app.exec();
    delete pBackend;
    
    
    return ret;
}
#endif

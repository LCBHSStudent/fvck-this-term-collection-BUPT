#include <PreCompile.h>

#include "UnitTest.hpp"

#include "ServerBackend/ServerBackend.h"


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    ServerBackend* pBackend = nullptr;
    
#ifdef Q_OS_WINDOWS
    system("chcp 65001");
    system("cls");
#endif
    
    while(true) {
        if(_getch() == 'y') {
            pBackend = new ServerBackend;
            std::cout << "new server backend instance was built up\n";
            TestUserBattle();
        } else {
            if(pBackend != nullptr) {
                delete pBackend;
                pBackend = nullptr;
                std::cout << "destroy server backend instance\n";
            }
        }
    }
    
    
    return app.exec();
}

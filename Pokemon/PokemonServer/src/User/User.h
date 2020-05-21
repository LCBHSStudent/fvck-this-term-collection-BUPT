#ifndef USER_H
#define USER_H

#include <PreCompile.h>

class User : public QObject {
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    
signals:
    
};

#endif // USER_H

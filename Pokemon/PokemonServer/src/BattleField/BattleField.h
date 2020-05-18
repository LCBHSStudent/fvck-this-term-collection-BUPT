#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <PreCompile.h>

class BattleField: public QObject {
    Q_OBJECT
public:
    explicit 
        BattleField(QObject* parent):
            QObject(parent) {}
};

#endif // BATTLEFIELD_H

QT -= gui
QT += sql
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

debug: DEFINES += _ITERATOR_DEBUG_LEVEL=0

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PROTOBUF_USE_DLLS
PRECOMPILED_HEADER = include/PreCompile.h
INCLUDEPATH += \
		include/				\
		../protobuf_headers/	\
		../protocol/			\

#CONFIG(debug, debug|release) {

#       LIBS += 

#}else{

#     LIBS+= 

#} 

LIBS += \
	-L$$PWD/../lib/ -llibprotobuf	\
	-L$$PWD/../lib/ -llibprotobufd	\
	
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/BattleField/BattleField.cpp \
        src/ORMHelper/ORMHelper.cpp \
        src/Pokemons/HighAtkPkm.cpp \
        src/Pokemons/PokemonBase/PokemonBase.cpp \
        src/Pokemons/PokemonBase/PokemonFactory.cpp \
	src/Pokemons/PokemonBase/PokemonSkill.cpp \
		src/SocketHelper/SocketHelper.cpp \
		src/User/User.cpp \
        src/PreCompile.cpp \
        src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	include/PreCompile.h \
	include/Reflect.hpp \
	src/BattleField/BattleField.h \
	src/ORMHelper/ORMHelper.h \
	src/Pokemons/HighAtkPkm.h \
	src/Pokemons/PokemonBase/PokemonBase.h \
	src/Pokemons/PokemonBase/PokemonFactory.h \
	src/Pokemons/PokemonBase/PokemonSkill.h \
	src/Pokemons/TestPokemonFunction.hpp \
	src/SocketHelper/SocketHelper.h	\
	src/UnitTest.hpp \
	src/User/User.h \

win32-msvc* {
    QMAKE_CXXFLAGS *=  /wd"4244"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34244) : QMAKE_CXXFLAGS_WARN_ON -= -w34244
    QMAKE_CXXFLAGS *=  /wd"4267"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34267) : QMAKE_CXXFLAGS_WARN_ON -= -w34267
    QMAKE_CXXFLAGS *=  /wd"4100"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34100) : QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS *=  /wd"4251"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34251) : QMAKE_CXXFLAGS_WARN_ON -= -w34251
}

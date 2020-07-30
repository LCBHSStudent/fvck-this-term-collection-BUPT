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

win32: {
	DEFINES += PROTOBUF_USE_DLLS
	LIBS += \
		-L$$PWD/../lib/ -llibprotobuf	\
		-L$$PWD/../lib/ -llibprotobufd	\
}
android: {
	LIBS += $$PWD/../lib/libprotobuf.a
}
	
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	../protocol/UserProtocol.pb.cc \
	../protocol/BattleProtocol.pb.cc \
	src/BattleField/BattleField.cpp \
	src/Pokemons/PokemonBase/PokemonBase.cpp \
	src/Pokemons/PokemonBase/PokemonFactory.cpp \
	src/Pokemons/PokemonBase/PokemonSkill.cpp \
	src/Pokemons/VariousPokemon.cpp \
	src/ServerBackend/ServerBackend.cpp \
	src/NetworkHelper/NetworkHelper.cpp \
	src/StorageHelper/StorageHelper.cpp \
	src/UnitTest.cpp \
	src/User/User.cpp \
	src/PreCompile.cpp \
	src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	../protocol/MessageTypeGlobal.h \
	include/PreCompile.h \
	include/Reflect.hpp \
	src/BattleField/BattleField.h \
	src/Pokemons/PokemonBase/PokemonBase.h \
	src/Pokemons/PokemonBase/PokemonFactory.h \
	src/Pokemons/PokemonBase/PokemonSkill.h \
	src/Pokemons/VariousPokemon.h \
	src/ServerBackend/ServerBackend.h \
	src/NetworkHelper/NetworkHelper.h \
	src/StorageHelper/StorageHelper.h \
	src/UnitTest.h \
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

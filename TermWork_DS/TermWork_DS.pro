QT += quick sql
QT -= network
CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/Backend/AlgorithmHelper.cpp \
        src/Backend/BackendBase.cpp \
        src/Backend/Customer/Customer.cpp \
        src/Backend/Customer/Logger.cpp \
        src/Backend/DataModel/AbstractData.cpp \
        src/Backend/DataModel/CityModel.cpp \
        src/Backend/DataModel/ScheduleModel.cpp \
        src/main.cpp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

RESOURCES += qml.qrc \
	res.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	src/Backend/BackendBase.h \
	src/Backend/Customer/Customer.h \
	src/Backend/Customer/Logger.h \
	src/Backend/DataModel/AbstractData.h \
	src/Backend/DataModel/CityModel.h \
	src/Backend/DataModel/ScheduleModel.h

DISTFILES += \
	android/AndroidManifest.xml \
	android/build.gradle \
	android/gradle/wrapper/gradle-wrapper.jar \
	android/gradle/wrapper/gradle-wrapper.properties \
	android/gradlew \
	android/gradlew.bat \
	android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

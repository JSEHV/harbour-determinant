# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-determinant

CONFIG += sailfishapp
CONFIG += link_pkgconfig

PKGCONFIG += sailfishsecrets sailfishapp nemonotifications-qt5

include(libquotient/libquotient.pri)

SOURCES += src/harbour-determinant.cpp \
    src/applicationservice.cpp \
    src/connectionsmanager.cpp \
    src/detroom.cpp \
    src/emojiparser.cpp \
    src/humanize.cpp \
    src/models/messagerenderer.cpp \
    src/models/publicroomlistmodel.cpp \
    src/models/roomlistmodel.cpp \
    src/models/roommembersmodel.cpp \
    src/models/sortfiltermodel.cpp \
    src/notificationsservice.cpp \
    src/qmlutils/stringutils.cpp \
    src/quotientintegration.cpp \
    src/secretsservice.cpp \
    src/store.cpp \
    src/models/roomeventsmodel.cpp \
    src/thumbnailprovider.cpp

DISTFILES += qml/harbour-determinant.qml \
    TODO.txt \
    qml/components/Avatar.qml \
    qml/components/ContentThumbnail.qml \
    qml/components/EventsView.qml \
    qml/components/MessageInput.qml \
    qml/components/PressEffect.qml \
    qml/cover/CoverPage.qml \
    qml/delegates/EventDelegate.qml \
    qml/delegates/ImageMessageDelegate.qml \
    qml/delegates/MessageDelegateBase.qml \
    qml/delegates/StateMessageDelegate.qml \
    qml/delegates/TextMessageDelegate.qml \
    qml/pages/ErrorPage.qml \
    qml/pages/ImageViewPage.qml \
    qml/pages/LoginPage.qml \
    qml/pages/LoginProgressPage.qml \
    qml/pages/PublicRoomListPage.qml \
    qml/pages/RoomDetailsPage.qml \
    qml/pages/RoomPage.qml \
    qml/pages/StartupPage.qml \
    rpm/harbour-determinant.changes \
    rpm/harbour-determinant.spec \
    translations/*.ts \
    harbour-determinant.desktop \
    qml/pages/RoomListPage.qml

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x050600
# DEFINES +=QT_USE_QSTRINGBUILDER # Problems with libquotient
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-determinant-de.ts \
    translations/harbour-determinant-es.ts \
    translations/harbour-determinant-et.ts \
    translations/harbour-determinant-sv.ts

HEADERS += \
    src/applicationservice.h \
    src/connectionsmanager.h \
    src/detroom.h \
    src/emojiparser.h \
    src/humanize.h \
    src/models/messagerenderer.h \
    src/models/publicroomlistmodel.h \
    src/models/roomlistmodel.h \
    src/models/roommembersmodel.h \
    src/models/sortfiltermodel.h \
    src/notificationsservice.h \
    src/qmlutils/stringutils.h \
    src/quotientintegration.h \
    src/secretsservice.h \
    src/stdhash.h \
    src/store.h \
    src/models/roomeventsmodel.h \
    src/thumbnailprovider.h

RESOURCES += \
    resources.qrc


twemoji.path = /usr/share/$${TARGET}/twemoji/svg
twemoji.files = twemoji/assets/svg/*
INSTALLS += twemoji

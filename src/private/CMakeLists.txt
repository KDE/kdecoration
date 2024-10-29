set(libkdecoration3Private_SRCS
    decoratedclientprivate.cpp
    decoratedclientprivate.h
    decorationbridge.cpp
    decorationbridge.h
    decorationsettingsprivate.cpp
    decorationsettingsprivate.h
)

add_library(kdecorations2private SHARED ${libkdecoration3Private_SRCS})

generate_export_header(
    kdecorations2private
BASE_NAME
    KDECORATIONS_PRIVATE
EXPORT_FILE_NAME
    kdecoration3/private/kdecoration3_private_export.h
)

add_library(KDecoration3::KDecorationPrivate ALIAS kdecorations2private)

target_link_libraries(kdecorations2private
    PUBLIC
        Qt::Core
        Qt::Gui
)

target_include_directories(kdecorations2private INTERFACE "$<INSTALL_INTERFACE:${KDECORATION3_INCLUDEDIR}>" )

set_target_properties(kdecorations2private PROPERTIES VERSION   ${KDECORATION3_VERSION}
                                                      SOVERSION 1
                                                      EXPORT_NAME KDecoration3Private
)

ecm_generate_headers(KDecoration3Private_CamelCase_HEADERS
  HEADER_NAMES
    DecoratedClientPrivate
    DecorationBridge
    DecorationSettingsPrivate
  PREFIX
    KDecoration3/Private
  REQUIRED_HEADERS KDecoration3Private_HEADERS
)
install(FILES ${KDecoration3Private_CamelCase_HEADERS}
        DESTINATION ${KDECORATION3_INCLUDEDIR}/KDecoration3/Private
        COMPONENT Devel)

install(TARGETS kdecorations2private EXPORT KDecoration3Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/kdecoration3/private/kdecoration3_private_export.h
        ${KDecoration3Private_HEADERS}
    DESTINATION
        ${KDECORATION3_INCLUDEDIR}/kdecoration3/private
    COMPONENT
        Devel
)

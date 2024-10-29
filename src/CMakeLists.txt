add_definitions(-DTRANSLATION_DOMAIN=\"kdecoration\")

add_subdirectory(private)

set(libkdecoration3_SRCS
    decoratedclient.cpp
    decoratedclient.h
    decorationbutton.cpp
    decorationbuttongroup.cpp
    decorationbuttongroup.h
    decorationbuttongroup_p.h
    decorationbutton.h
    decorationbutton_p.h
    decoration.cpp
    decorationdefines.h
    decoration.h
    decoration_p.h
    decorationsettings.cpp
    decorationsettings.h
    decorationshadow.cpp
    decorationshadow.h
    decorationshadow_p.h
    decorationthemeprovider.cpp
    decorationthemeprovider.h

)

add_library(kdecorations2 SHARED ${libkdecoration3_SRCS})
ecm_generate_export_header(kdecorations2
    VERSION ${PROJECT_VERSION}
    EXPORT_FILE_NAME kdecoration3/kdecoration3_export.h
    DEPRECATION_VERSIONS 5.21
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

add_library(KDecoration3::KDecoration ALIAS kdecorations2)

target_link_libraries(kdecorations2
    PUBLIC
        Qt::Core
        Qt::Gui
    PRIVATE
        kdecorations2private
        KF6::I18n
)

target_include_directories(kdecorations2 INTERFACE "$<INSTALL_INTERFACE:${KDECORATION3_INCLUDEDIR}>" )

set_target_properties(kdecorations2 PROPERTIES VERSION   ${KDECORATION3_VERSION}
                                               SOVERSION ${KDECORATION3_SOVERSION}
                                               EXPORT_NAME KDecoration
)

ecm_generate_headers(KDecoration3_CamelCase_HEADERS
  HEADER_NAMES
    DecoratedClient
    Decoration
    DecorationButton
    DecorationButtonGroup
    DecorationSettings
    DecorationShadow
    DecorationThemeProvider
  PREFIX
    KDecoration3
  REQUIRED_HEADERS KDecoration3_HEADERS
)
install(FILES ${KDecoration3_CamelCase_HEADERS}
        DESTINATION ${KDECORATION3_INCLUDEDIR}/KDecoration3
        COMPONENT Devel)

install(TARGETS kdecorations2 EXPORT KDecoration3Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/kdecoration3/kdecoration3_export.h
        ${KDecoration3_HEADERS}
        decorationdefines.h
    DESTINATION
        ${KDECORATION3_INCLUDEDIR}/kdecoration3
    COMPONENT
        Devel
)

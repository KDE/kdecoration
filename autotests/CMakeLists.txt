include(ECMMarkAsTest)

set(decorationButtonTest_SRCS
    mockbridge.cpp
    mockbutton.cpp
    mockclient.cpp
    mockdecoration.cpp
    mocksettings.cpp
    decorationbuttontest.cpp
    )
add_executable(decorationButtonTest ${decorationButtonTest_SRCS})
target_link_libraries(decorationButtonTest kdecorations2 kdecorations2private Qt5::Test)
add_test(kdecoration2-decorationButtonTest decorationButtonTest)
ecm_mark_as_test(decorationButtonTest)

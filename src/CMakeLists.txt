set(SOURCES
    colorizer.cpp
	completerloader.cpp
	consoledialog.cpp
	highlighter.cpp
	treemodelcomplteter.cpp
)

add_library(QtPseudoTerminal STATIC ${SOURCES})
target_link_libraries(QtPseudoTerminal PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(QtPseudoTerminal PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

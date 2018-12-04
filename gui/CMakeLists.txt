set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MIN_VERSION "5.6.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG
    REQUIRED COMPONENTS
        Widgets
)

set(SOURCE
	main.cpp
	libretuner.h
	libretuner.cpp

        rustapp.h
        rustapp.cpp
        rust_bindings.h

	error.h
	error.cpp

	mainwindow.h
	mainwindow.cpp

	consolewidget.h
	consolewidget.cpp
	romswidget.h
	romswidget.cpp
	sidebarwidget.h
	sidebarwidget.cpp
        downloadwidget.h
        downloadwidget.cpp
)

add_executable(gui ${SOURCE})
get_target_property(RUST_LIB rust LOCATION)

set_target_properties(gui PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

add_custom_target(
	rust_copy
        #COMMAND ${CMAKE_COMMAND} -E copy "${RUST_DLL}" "${CMAKE_CURRENT_BINARY_DIR}"
	DEPENDS rust
)

add_dependencies(gui rust_copy)
target_link_libraries(gui Qt5::Widgets ${RUST_LIB})

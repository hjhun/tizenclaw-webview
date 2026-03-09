cmake_minimum_required(VERSION 3.0)
project(tizenclaw-webview C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Required packages
find_package(PkgConfig REQUIRED)

# Standard Tizen EFL + WebKit + AppControl dependencies
# elementary, ecore, ecore-evas, eina, evas, ewebkit, capi-appfw-application, capi-appfw-app-control, dlog
pkg_check_modules(PKGS REQUIRED 
    elementary
    ecore
    ecore-evas
    eina
    evas
    ewebkit
    capi-appfw-application
    capi-appfw-app-control
    dlog
)

include_directories(
    inc
    ${PKGS_INCLUDE_DIRS}
)

link_directories(
    ${PKGS_LIBRARY_DIRS}
)

add_executable(tizenclaw-webview src/tizenclaw-webview.c)

target_link_libraries(tizenclaw-webview
    ${PKGS_LIBRARIES}
)

# Install executable to /usr/bin
install(TARGETS tizenclaw-webview
        DESTINATION bin)

# Install manifest XML to /usr/share/packages
install(FILES packaging/org.tizen.tizenclew-webview.xml
        DESTINATION share/packages)

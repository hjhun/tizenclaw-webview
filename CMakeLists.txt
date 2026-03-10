# Copyright 2024-2026 Samsung Electronics Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(tizenclaw-webview CXX)

## Compiler flags
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC")

# Suppress GCC ARM false-positive warnings on 32-bit ARM
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi -Wno-stringop-overread -Wno-stringop-overflow")
ENDIF()

## Paths
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(BINDIR "${PREFIX}/bin")

## Target names
SET(TARGET_WEBVIEW "tizenclaw-webview")

## Find all needed packages once
INCLUDE(FindPkgConfig)

PKG_CHECK_MODULES(WEBVIEW_DEPS REQUIRED 
    elementary
    ecore
    ecore-evas
    eina
    evas
    efl-extension
    vc-webview
    capi-system-system-settings
    capi-appfw-application
    capi-appfw-app-control
    dlog
)

ADD_EXECUTABLE(${TARGET_WEBVIEW} src/tizenclaw-webview.cc)

TARGET_INCLUDE_DIRECTORIES(${TARGET_WEBVIEW} PRIVATE 
    src
    ${WEBVIEW_DEPS_INCLUDE_DIRS}
)

TARGET_LINK_DIRECTORIES(${TARGET_WEBVIEW} PRIVATE 
    ${WEBVIEW_DEPS_LIBRARY_DIRS}
)

TARGET_LINK_LIBRARIES(${TARGET_WEBVIEW} PRIVATE 
    ${WEBVIEW_DEPS_LIBRARIES}
)

# Install executable to /usr/apps
INSTALL(TARGETS ${TARGET_WEBVIEW}
        DESTINATION /usr/apps/org.tizen.tizenclew-webview/bin)

# Install manifest XML to /usr/share/packages
INSTALL(FILES packaging/org.tizen.tizenclew-webview.xml
        DESTINATION share/packages)

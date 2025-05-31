cmake_minimum_required(VERSION 4.0.2)
project(FloydWarshallAlgorithm VERSION 1.0 LANGUAGES CXX)

# Enable Qt6 automatic MOC, UIC and RCC handling
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Use C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Allow out-of-source builds
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Find Qt6
find_package(Qt6 COMPONENTS Core Widgets PrintSupport REQUIRED)

# Include headers
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# Source files
set(SOURCES
    main.cpp
    GraphPlotter.cpp
    MainWindow.cpp
    MatrixCalculator.cpp
    qcustomplot.cpp
)

# Header files (for IDEs)
set(HEADERS
    GraphPlotter.h
    MainWindow.h
    MatrixCalculator.h
    qcustomplot.h
)

# Define executable
if(APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE
      ${SOURCES}
      ${HEADERS}
  )
elseif(WIN32)
  add_executable(${PROJECT_NAME} WIN32
      ${SOURCES}
      ${HEADERS}
  )
else()
  add_executable(${PROJECT_NAME}
      ${SOURCES}
      ${HEADERS}
  )
endif()

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::PrintSupport
)

# Add application version macro from CMake
target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSION="${PROJECT_VERSION}")

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "v${PROJECT_VERSION}")
# License file for installer
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
if(WIN32)
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
  set(CPACK_GENERATOR "NSIS")
elseif(APPLE)
  install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
  set(CPACK_GENERATOR "DragNDrop")
else()
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
  set(CPACK_GENERATOR "TGZ")
endif()
include(CPack)

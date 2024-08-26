cmake_minimum_required(VERSION 3.11)

project(hope_thread VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(hope_thread INTERFACE)

target_include_directories(hope_thread INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
install(TARGETS hope_thread
    EXPORT hope_thread_targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY hope_thread/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hope_thread)

install(EXPORT hope_thread_targets
    FILE hope_thread-targets.cmake
    NAMESPACE hope_thread::
    DESTINATION share/hope_thread
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hope_threadConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/hope_thread-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hope_thread-config.cmake"
    @ONLY
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hope_thread-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hope_threadConfigVersion.cmake"
    DESTINATION share/hope_thread
)

# © Joseph Cameron - MIT License

cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

if (TARGET jfcnet_enet)
    return()
endif()

project(jfc-net_enet
    VERSION 0.1
    DESCRIPTION "the jfc-net transport interface, implemented over ENet"
    LANGUAGES C CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFCNET_ENET_BUILD_TESTS "Build unit tests" ${PROJECT_IS_TOP_LEVEL})
option(JFCNET_ENET_BUILD_DOCS "Build documentation" OFF)
option(JFCNET_ENET_INSTALL "Generate install rules, a vendored ENet's included" ${PROJECT_IS_TOP_LEVEL})

if (JFCNET_ENET_BUILD_DOCS)
    jfc_generate_documentation_doxygen(
        README_PATH      README.md
        OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docs

        INCLUDE_DIRECTORIES
            include/
    )

    if (NOT JFCNET_ENET_BUILD_TESTS)
        return()
    endif()
endif()

if (EMSCRIPTEN)
    message(FATAL_ERROR "jfc-net_enet needs UDP sockets, which a browser does not offer: see jfc-net_webrtc")
endif()

add_subdirectory(extern)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/permissive->)
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-pedantic>)
    endif()

    include(CheckCXXCompilerFlag)

    foreach (_strict_flag "-Werror=c++20-extensions" "-Werror=c++23-extensions")
        string(MAKE_C_IDENTIFIER "HAS${_strict_flag}" _strict_flag_symbol)

        check_cxx_compiler_flag("${_strict_flag}" ${_strict_flag_symbol})

        if (${_strict_flag_symbol})
            add_compile_options($<$<COMPILE_LANGUAGE:CXX>:${_strict_flag}>)
        endif()
    endforeach()
endif()

include(GNUInstallDirs)

add_library(jfcnet_enet STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/enet_host.cpp")

add_library(jfc::net_enet ALIAS jfcnet_enet)

set_target_properties(jfcnet_enet PROPERTIES EXPORT_NAME net_enet PREFIX "lib")

target_include_directories(jfcnet_enet PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_compile_features(jfcnet_enet PUBLIC cxx_std_20)

target_link_libraries(jfcnet_enet
    PUBLIC
        jfcnet
    PRIVATE
        enet)

if (JFCNET_ENET_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS jfcnet_enet enet
        EXPORT jfc-net_enet-targets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(EXPORT jfc-net_enet-targets
        FILE jfc-net_enet-targets.cmake
        NAMESPACE jfc::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/jfc-net_enet")

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/jfc-net_enet-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

    file(WRITE "${PROJECT_BINARY_DIR}/jfc-net_enet-config.cmake"
"include(CMakeFindDependencyMacro)
find_dependency(jfcnet 0.1)
include(\"\${CMAKE_CURRENT_LIST_DIR}/jfc-net_enet-targets.cmake\")
")

    install(FILES
        "${PROJECT_BINARY_DIR}/jfc-net_enet-config.cmake"
        "${PROJECT_BINARY_DIR}/jfc-net_enet-config-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/jfc-net_enet")
endif()

if (JFCNET_ENET_BUILD_TESTS)
    add_subdirectory(test)
endif()

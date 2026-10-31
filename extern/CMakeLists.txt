# © Joseph Cameron - MIT License

if (NOT TARGET jfcnet)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/jfc-net")
endif()

if (NOT TARGET enet)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/enet" "${CMAKE_CURRENT_BINARY_DIR}/enet" EXCLUDE_FROM_ALL)

    target_include_directories(enet PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/enet/include>")

    if (WIN32)
        target_link_libraries(enet PUBLIC ws2_32 winmm)
    endif()
endif()

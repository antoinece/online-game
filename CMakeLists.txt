cmake_minimum_required(VERSION 3.2)
project(OnlineGame)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(externals/SFML)
add_subdirectory(game)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/externals/imgui/" CACHE STRING "")
add_subdirectory(externals/imgui-sfml)
add_subdirectory(externals/Physics_923)



# Create the photon library.
file(GLOB_RECURSE PHOTON_SRC_FILES externals/LoadBalancing-cpp/inc/*.h externals/LoadBalancing-cpp/src/*.cpp)
add_library(photon ${PHOTON_SRC_FILES})
set_target_properties(photon PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(photon PUBLIC externals/)
#ReleaseVersion:
target_link_libraries(photon PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/Common-cpp/lib/Common-cpp_vc17_release_windows_md_x64.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/externals/Photon-cpp/lib/Photon-cpp_vc17_release_windows_md_x64.lib
)
#DebugVersion
#target_link_libraries(photon PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/externals/Common-cpp/lib/Common-cpp_vc17_debug_windows_md_x64.lib
#        ${CMAKE_CURRENT_SOURCE_DIR}/externals/Photon-cpp/lib/Photon-cpp_vc17_debug_windows_md_x64.lib
#)


#serv
add_executable(server main/server.cc)
target_include_directories(server PRIVATE externals/SFML/include include)
target_link_libraries(server PRIVATE sfml-network)
target_link_libraries(server PRIVATE api)


add_executable(client main/client.cc)
set(NET_DIR externals)
target_include_directories(client PUBLIC ${NET_DIR}/include)
#client

link_directories(${NET_DIR}/lib/windows/)

target_include_directories(client PRIVATE externals/SFML/include externals/imgui-sfml externals/imgui externals/Physics_923)
target_link_libraries(client PRIVATE sfml-network sfml-graphics ImGui-SFML api crackitos_physics photon)
get_target_property(client_includes client INCLUDE_DIRECTORIES)
message(STATUS "Client includes after photon: ${client_includes}")

if(MSVC)
    target_compile_options(client PRIVATE "/W3" "/WX" "/permissive-")
endif ()


#copy resources in build
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

get_target_property(client_includes client INCLUDE_DIRECTORIES)
message(STATUS "Client includes: ${client_includes}")
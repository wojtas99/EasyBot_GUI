cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/third_party/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_BUILD_TYPE ${CMAKE_BUILD_TYPE})

set(VCPKG_TARGET_TRIPLET "x64-windows-static-release" CACHE STRING "VCPKG target triplet" FORCE)
set(VCPKG_HOST_TRIPLET "x64-windows-static-release" CACHE STRING "VCPKG host triplet" FORCE)
set(GRPC_CPP_PLUGIN_EXECUTABLE "${CMAKE_SOURCE_DIR}/third_party/vcpkg/packages/grpc_x64-windows-static-release/tools/grpc/grpc_cpp_plugin.exe")

project(EasyBotGUI)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.10.0/msvc2022_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Qml Network NetworkAuth)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_path(LUABRIDGE3_INCLUDE_DIRS "luabridge3/LuaBridge/Array.h")


set(PROTO_FILE ${CMAKE_SOURCE_DIR}/bot.proto)
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GEN_DIR})

set(GENERATED_SRCS ${GEN_DIR}/bot.pb.cc ${GEN_DIR}/bot.grpc.pb.cc)
set(GENERATED_HDRS ${GEN_DIR}/bot.pb.h ${GEN_DIR}/bot.grpc.pb.h)

add_custom_command(
        OUTPUT ${GENERATED_SRCS} ${GENERATED_HDRS}
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        --proto_path=${CMAKE_SOURCE_DIR}
        --cpp_out=${GEN_DIR}
        --grpc_out=${GEN_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
        ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        VERBATIM
)

qt_add_resources(QRC_SOURCES icon.qrc)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/MVC/View
        ${CMAKE_CURRENT_SOURCE_DIR}/MVC/Model
        ${CMAKE_CURRENT_SOURCE_DIR}/MVC/Controller
        ${LUA_INCLUDE_DIR}
        ${LUABRIDGE3_INCLUDE_DIRS}
)

file(GLOB_RECURSE MVC
        "${CMAKE_CURRENT_SOURCE_DIR}/MVC/View/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/MVC/Controller/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/MVC/Model/*.cpp"
        BotEngine.cpp
        LuaEngine.cpp
)

add_executable(EasyBotGUI
        EasyBot_GUI.cpp
        ${MVC}
        ${QRC_SOURCES}
        ${CMAKE_SOURCE_DIR}/proto_functions_client.cpp
        app_icon.rc
)

add_library(bot_proto STATIC ${GENERATED_SRCS} ${GENERATED_HDRS})
target_include_directories(bot_proto PUBLIC ${GEN_DIR})
target_link_libraries(bot_proto PUBLIC protobuf::libprotobuf gRPC::grpc++)

target_link_libraries(EasyBotGUI PRIVATE
        Qt6::Widgets
        Qt6::Core
        Qt6::Qml
        Qt6::Network
        Qt6::NetworkAuth
        bot_proto
        ${LUA_LIBRARIES}
)

set_target_properties(EasyBotGUI PROPERTIES
        OUTPUT_NAME "Bot/EasyBot"
        RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}"
        LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}"
        AUTOUIC ON
        AUTOMOC ON
)


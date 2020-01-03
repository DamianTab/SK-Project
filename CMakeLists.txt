cmake_minimum_required(VERSION 3.15)
project(SK_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Client)
include_directories(Libs)
include_directories(Server)

add_executable(client Client/main_client.cpp)

add_executable(server Server/mainServer.cpp)
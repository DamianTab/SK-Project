cmake_minimum_required(VERSION 3.15)
project(SK_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Client)
include_directories(Server)
include_directories(Classes)
include_directories(Utils)


add_executable(client Client/main_client.cpp Classes/Client.cpp Classes/Client.h Classes/SocketHandler.h Classes/Server.cpp Classes/Server.h Utils/utils.h Utils/utils.cpp)

add_executable(server Server/main_Server.cpp Classes/Client.cpp Classes/Client.h Classes/SocketHandler.h Classes/Server.cpp Classes/Server.h Utils/utils.h Utils/utils.cpp)
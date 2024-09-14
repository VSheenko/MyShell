cmake_minimum_required(VERSION 3.28)
project(MyShell)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyShell main.cpp
        commands/src/ls.cpp
        commands/ls.h
        commands/commands_include.h
        Shell.cpp
        Shell.h
        utils/XmlWorker.cpp
        utils/XmlWorker.h)

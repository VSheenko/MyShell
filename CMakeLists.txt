cmake_minimum_required(VERSION 3.28)
project(MyShell)
project(ls)
set(CMAKE_CXX_STANDARD 17)

include_directories("libs/tinyxml2")
set(CMAKE_EXE_LINKER_FLAGS "-static")

#-------------------------------------Libs and headers---------------------------------------------
find_package(miniz CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

if (NOT miniz_FOUND)
    message(FATAL_ERROR "miniz NOT FOUND")
endif ()

if (NOT tinyxml2_FOUND)
    message(FATAL_ERROR "tinyxml2 NOT FOUND")
endif ()


set(SHARED_SOURCES
        utils/ArchiveZipWorker.cpp
        utils/ArchiveZipWorker.h
        utils/UtilsMini.h
        struct/SharedData.cpp
        struct/SharedData.h
)

set(SHARED_LIBS
        miniz::miniz
)


#-------------------------------------commands-----------------------------------------------------
# Наверно лучше было бы отдельно для них в бубен бить, но пусть будут тут
add_executable(ls
        commands/src/ls.cpp
        ${SHARED_SOURCES}
)
target_link_libraries(ls ${SHARED_LIBS})

add_executable(tac
        commands/src/tac.cpp
        ${SHARED_SOURCES}
)
target_link_libraries(tac ${SHARED_LIBS})

add_executable(echo
        commands/src/echo.cpp
        ${SHARED_SOURCES}
)
target_link_libraries(echo ${SHARED_LIBS})

add_executable(cal commands/src/cal.cpp)

#-------------------------------------MySHell------------------------------------------------------
add_executable(MyShell main.cpp
        Shell.cpp
        Shell.h
        utils/XmlConfigWorker.cpp
        utils/XmlConfigWorker.h
        utils/Logger.cpp
        utils/Logger.h
        ${SHARED_SOURCES}
)

target_link_libraries(MyShell
        tinyxml2::tinyxml2
        ${SHARED_LIBS}
)
#-------------------------------------Properties--------------------------------------------------

set_target_properties(ls PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY commands
)
set_target_properties(tac PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY commands
)
set_target_properties(echo PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY commands
)
set_target_properties(cal PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY commands
)


add_subdirectory(test)


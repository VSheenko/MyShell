cmake_minimum_required(VERSION 3.28)
project(ArchiveZipWorkerTest)

include_directories("../../utils")
file(GLOB MANAGER
        ../utils/ArchiveZipWorker.h
        ../utils/ArchiveZipWorker.cpp

        ../utils/XmlConfigWorker.h
        ../utils/XmlConfigWorker.cpp
)

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}
        ArchiveZipWorkerTest/ArchiveZipWorkerTest.cpp
        XmlConfigWorkerTest/XmlConfigWorkerTest.cpp
        ${MANAGER}
)

target_link_libraries(${PROJECT_NAME} GTest::GTest GTest::Main miniz::miniz tinyxml2::tinyxml2)
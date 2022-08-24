CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (ftpserver)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

OPTION (BUILD_TESTING "Build testing" OFF)

SET (CMAKE_CXX_STANDARD 17)

ADD_SUBDIRECTORY (thirdparty)

IF (BUILD_TESTING)
    ENABLE_TESTING ()
    ADD_SUBDIRECTORY (tests tests)

    ADD_TEST (tests tests/tests)
ENDIF ()


SET (SOURCES
        ${SRC_DIR}/servercore.cpp
        ${SRC_DIR}/serverconnection.cpp
        ${SRC_DIR}/fileoperator.cpp
        ${SRC_DIR}/ftpserver.cpp
        ${SRC_DIR}/OSApiFactory.cpp
    )

ADD_EXECUTABLE (${PROJECT_NAME} ${SOURCES})

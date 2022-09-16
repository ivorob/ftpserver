CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (ftpserver)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

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
        ${SRC_DIR}/OSApi.cpp
        ${SRC_DIR}/OSApiImpl.cpp
        ${SRC_DIR}/Socket.cpp
        ${SRC_DIR}/FTPCommandFactory.cpp
        ${SRC_DIR}/OSUtils.cpp
        # commands
        ${SRC_DIR}/FTPCommand.cpp
        ${SRC_DIR}/ftpcommands/DefaultCommand.cpp
        ${SRC_DIR}/ftpcommands/UserCommand.cpp
        ${SRC_DIR}/ftpcommands/QuitCommand.cpp
        ${SRC_DIR}/ftpcommands/CurrentDirectoryCommand.cpp
        ${SRC_DIR}/ftpcommands/ChangeDirectoryCommand.cpp
        ${SRC_DIR}/ftpcommands/SystemTypeCommand.cpp
        ${SRC_DIR}/ftpcommands/PassiveModeCommand.cpp
    )

SET (HEADERS
        ${INCLUDE_DIR}/servercore.h
        ${INCLUDE_DIR}/serverconnection.h
        ${INCLUDE_DIR}/fileoperator.h
        ${INCLUDE_DIR}/AbstractOSApi.h
        ${INCLUDE_DIR}/OSApi.h
        ${INCLUDE_DIR}/OSApiImpl.h
        ${INCLUDE_DIR}/Socket.h
        ${INCLUDE_DIR}/FTPCommandFactory.h
        ${INCLUDE_DIR}/OSUtils.h
        # commands
        ${INCLUDE_DIR}/FTPContext.h
        ${INCLUDE_DIR}/FTPCommand.h
        ${INCLUDE_DIR}/ftpcommands/DefaultCommand.h
        ${INCLUDE_DIR}/ftpcommands/UserCommand.h
        ${INCLUDE_DIR}/ftpcommands/Commands.h
        ${INCLUDE_DIR}/ftpcommands/QuitCommand.h
        ${INCLUDE_DIR}/ftpcommands/CurrentDirectoryCommand.h
        ${INCLUDE_DIR}/ftpcommands/ChangeDirectoryCommand.h
        ${INCLUDE_DIR}/ftpcommands/SystemTypeCommand.h
        ${INCLUDE_DIR}/ftpcommands/PassiveModeCommand.h
    )

ADD_EXECUTABLE (${PROJECT_NAME} ${SOURCES} ${HEADERS})
TARGET_INCLUDE_DIRECTORIES (${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

IF (WIN32)
    TARGET_LINK_LIBRARIES (${PROJECT_NAME} ws2_32)
ELSE ()
    TARGET_COMPILE_OPTIONS (${PROJECT_NAME} PRIVATE "-Wall")
ENDIF (WIN32)

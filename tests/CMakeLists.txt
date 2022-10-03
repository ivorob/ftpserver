CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (tests)

SET (MAIN_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
SET (MAIN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (SOURCES 
        ${SRC_DIR}/tests.cpp
        ${SRC_DIR}/ScopedStreamRedirector.cpp
        ${SRC_DIR}/FtpServerTest.cpp
        ${SRC_DIR}/ChangeDirectoryTest.cpp
        ${SRC_DIR}/CreateDirectoryTest.cpp
        ${SRC_DIR}/ReadFileTest.cpp
        ${SRC_DIR}/AppendFileTest.cpp
        ${SRC_DIR}/DeleteFileTest.cpp
        ${SRC_DIR}/FakeOSApi.cpp
        ${SRC_DIR}/ServerConnectionTest.cpp
        ${SRC_DIR}/LoggerTest.cpp
        ${SRC_DIR}/TransmitterTest.cpp
        # main sources
        ${MAIN_SRC_DIR}/servercore.cpp
        ${MAIN_SRC_DIR}/serverconnection.cpp
        ${MAIN_SRC_DIR}/fileoperator.cpp
        ${MAIN_SRC_DIR}/Socket.cpp
        ${MAIN_SRC_DIR}/FTPCommandFactory.cpp
        ${MAIN_SRC_DIR}/OSUtils.cpp
        ${MAIN_SRC_DIR}/DataConnection.cpp
        ${MAIN_SRC_DIR}/Logger.cpp
        ${MAIN_SRC_DIR}/Transmitter.cpp
        ${MAIN_SRC_DIR}/FTPCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/DefaultCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/UserCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/QuitCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/CurrentDirectoryCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/ChangeDirectoryCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/SystemTypeCommand.cpp
        ${MAIN_SRC_DIR}/ftpcommands/PassiveModeCommand.cpp
    )

SET (HEADERS
        ${INCLUDE_DIR}/ScopedStreamRedirector.h
        ${INCLUDE_DIR}/FakeOSApi.h
        # main includes
        ${MAIN_INCLUDE_DIR}/servercore.h
        ${MAIN_INCLUDE_DIR}/serverconnection.h
        ${MAIN_INCLUDE_DIR}/fileoperator.h
        ${MAIN_INCLUDE_DIR}/OSApi.h
        ${MAIN_INCLUDE_DIR}/Socket.h
        ${MAIN_INCLUDE_DIR}/FTPCommandFactory.h
        ${MAIN_INCLUDE_DIR}/OSUtils.h
        ${MAIN_INCLUDE_DIR}/DataConnection.h
        ${MAIN_INCLUDE_DIR}/Logger.h
        ${MAIN_INCLUDE_DIR}/Transmitter.h
        ${MAIN_INCLUDE_DIR}/FTPCommand.h
        ${MAIN_INCLUDE_DIR}/FTPContext.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/Commands.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/DefaultCommand.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/UserCommand.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/QuitCommand.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/CurrentDirectoryCommand.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/ChangeDirectoryCommand.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/SystemTypeCommand.h
        ${MAIN_INCLUDE_DIR}/ftpcommands/PassiveModeCommand.h
    )

ADD_EXECUTABLE (tests ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES (tests gtest gmock)
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${MAIN_INCLUDE_DIR})

IF (WIN32)
    TARGET_LINK_LIBRARIES (tests ws2_32)
ENDIF (WIN32)

ADD_CUSTOM_TARGET (JUnitTestReport
    COMMAND $<TARGET_FILE:tests> --gtest_output="xml:$ENV{BINARY_DIR}/tests.xml"
    DEPENDS tests
)

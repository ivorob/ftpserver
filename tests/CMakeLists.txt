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
        ${SRC_DIR}/FakeOSApi.cpp
        # main sources
        ${MAIN_SRC_DIR}/servercore.cpp
        ${MAIN_SRC_DIR}/serverconnection.cpp
        ${MAIN_SRC_DIR}/fileoperator.cpp
    )

SET (HEADERS
        ${INCLUDE_DIR}/ScopedStreamRedirector.h
        ${INCLUDE_DIR}/FakeOSApi.h
        # main includes
        ${MAIN_INCLUDE_DIR}/servercore.h
        ${MAIN_INCLUDE_DIR}/serverconnection.h
        ${MAIN_INCLUDE_DIR}/fileoperator.h
        ${MAIN_INCLUDE_DIR}/OSApi.h
    )

ADD_EXECUTABLE (tests ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES (tests gtest gmock)
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${MAIN_INCLUDE_DIR})

ADD_CUSTOM_TARGET (JUnitTestReport
    COMMAND $<TARGET_FILE:tests> --gtest_output="xml:$ENV{BINARY_DIR}/tests.xml"
    DEPENDS tests
)

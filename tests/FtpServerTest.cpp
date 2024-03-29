#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "servercore.h"
#include "ScopedStreamRedirector.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"

namespace {

class FtpServerTest : public ::testing::Test {
public:
    void TearDown() override {
        setImpl({});
    }
};

std::shared_ptr<MockOSApiImpl> makeImpl() {
    auto impl = std::make_shared<MockOSApiImpl>();
    setImpl(impl);
    return impl;
}

}

TEST_F(FtpServerTest, error_during_changing_directory_prints_errors)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, chdir)
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(*impl, socket)
        .WillRepeatedly(Return(INVALID_SOCKET));

    // Act
    servercore ftpServer(4242, "/");

    // Assert
    ASSERT_EQ(
        "Directory could not be changed to '/'!\n"
        "socket() failed\n",
        out.str());
}

TEST_F(FtpServerTest, error_during_allocating_socket_shutdown_server)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, chdir)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, socket)
        .WillRepeatedly(Return(INVALID_SOCKET));

    // Act
    servercore ftpServer(4242, "/");

    // Assert
    ASSERT_EQ(
        "socket() failed\n",
        out.str());
}

TEST_F(FtpServerTest, error_during_setsockopt_shutdown_server)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, chdir)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, socket)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, setsockopt)
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(*impl, close)
        .Times(1)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly([](SOCKET, struct sockaddr* name, socklen_t* namelen) {
            return 0;
        });

    // Act
    servercore ftpServer(4242, "/");

    // Assert
    ASSERT_EQ(
        "setsockopt() failed\n",
        out.str());
}

TEST_F(FtpServerTest, error_during_bind_shutdown_server)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, chdir)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, socket)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, setsockopt)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, makeNonBlocking)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*impl, bind)
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(*impl, close)
        .Times(1)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly([](SOCKET, struct sockaddr* name, socklen_t* namelen) {
            return 0;
        });

    // Act
    servercore ftpServer(4242, "/");

    // Assert
    ASSERT_EQ(
        "bind() failed (do you have the apropriate rights? is the port unused?)\n",
        out.str());
}

TEST_F(FtpServerTest, error_during_listen_shutdown_server)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, chdir)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, socket)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, makeNonBlocking)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*impl, setsockopt)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, bind)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, listen)
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(*impl, close)
        .Times(1)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly([](SOCKET, struct sockaddr* name, socklen_t* namelen) {
            return 0;
        });

    // Act
    servercore ftpServer(4242, "/");

    // Assert
    ASSERT_EQ(
        "listen () failed\n",
        out.str());
}

TEST_F(FtpServerTest, server_is_started_successfully)
{
    // Arrange
    std::ostringstream errorOut, out;
    ScopedStreamRedirector errorStreamRedirector(std::cerr, errorOut);
    ScopedStreamRedirector streamRedirector(std::cout, out);

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, chdir)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, socket)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, makeNonBlocking)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*impl, setsockopt)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, bind)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, listen)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, close)
        .Times(1)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, select)
        .WillRepeatedly(Return(-1));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly([](SOCKET, struct sockaddr* name, socklen_t* namelen) {
            return 0;
        });

    // Act
    servercore ftpServer(4242, "/");

    // Assert
    ASSERT_EQ(
        "Error calling select\n",
        errorOut.str());
    ASSERT_EQ(
        "Server started and listening at port 4242, default server directory '/'\n",
        out.str());
}

#include <gtest/gtest.h>
#include <string.h>
#include <filesystem>

#include "serverconnection.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"
#include "ScopedStreamRedirector.h"
#include "BrowseUtils.h"

namespace fs = std::filesystem;

namespace {

class ServerConnectionTest : public ::testing::Test {
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

std::vector<struct dirent> makeTestDir() {
    std::vector<struct dirent> testDir;
    testDir.push_back(BrowseUtils::makeDirEntry(".", DT_DIR));
    testDir.push_back(BrowseUtils::makeDirEntry("..", DT_DIR));
    testDir.push_back(BrowseUtils::makeDirEntry("testDir", DT_DIR));
    testDir.push_back(BrowseUtils::makeDirEntry("file1", DT_REG));
    testDir.push_back(BrowseUtils::makeDirEntry("file2", DT_REG));
    return testDir;
}

}

TEST_F(ServerConnectionTest, create_regular_server_connection_is_succeeded)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    auto impl = makeImpl();
    std::string message;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&message](int, const void* msg, size_t len, int) -> int {
            if (len != 0) {
                message.assign(reinterpret_cast<const char*>(msg), len);
                return 0;
            }

            return -1;
        });

    // Act
    serverconnection serverConnection(1, 1, "/", "127.0.0.1");

    // Assert
    ASSERT_EQ(
        "220 FTP server ready.\n",
        message);
    ASSERT_EQ(
        "Connection to client '127.0.0.1' established\n",
        out.str());
}

TEST_F(ServerConnectionTest, send_502_code_for_unimplemented_command)
{
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();

    EXPECT_CALL(*impl, recv)
        .WillRepeatedly([](int, void* buf, size_t len, int) -> int {
            std::string command = "invalid command\n";
            strncpy(reinterpret_cast<char*>(buf), command.c_str(), len);
            return static_cast<int>(command.size());
        });
    std::string response;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&response](int, const void* msg, size_t len, int) -> int {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            response.assign(reinterpret_cast<const char*>(msg), len);
            return 0;
        });
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));

    serverconnection serverConnection(1, 1, "/", "127.0.0.1");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    serverConnection.respondToQuery();

    // Assert
    ASSERT_EQ(
        "502 Command not implemented.\n",
        response);
    ASSERT_EQ(
        "Connection 1: Unknown command encountered 'invalid'!\n",
        out.str());
}

TEST_F(ServerConnectionTest, user_command_is_processed_successfully)
{
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();

    EXPECT_CALL(*impl, recv)
        .WillRepeatedly([](int, void* buf, size_t len, int) -> int {
            std::string command = "USER unknown\n";
            strncpy(reinterpret_cast<char*>(buf), command.c_str(), len);
            return static_cast<int>(command.size());
        });
    std::string response;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&response](int, const void* msg, size_t len, int) -> int {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            response.assign(reinterpret_cast<const char*>(msg), len);
            return 0;
        });
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));

    serverconnection serverConnection(1, 1, "/", "127.0.0.1");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    serverConnection.respondToQuery();

    // Assert
    ASSERT_EQ(
        "230 User logged in, proceed.\n",
        response);
    ASSERT_EQ(
        "Connection 1: User 'unknown' is logged successfully!\n",
        out.str());
}

TEST_F(ServerConnectionTest, current_directory_command_is_processed_successfully)
{
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();

    EXPECT_CALL(*impl, recv)
        .WillRepeatedly([](int, void* buf, size_t len, int) -> int {
            std::string command = "PWD\n";
            strncpy(reinterpret_cast<char*>(buf), command.c_str(), len);
            return static_cast<int>(command.size());
        });
    std::string response;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&response](int, const void* msg, size_t len, int) -> int {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            response.assign(reinterpret_cast<const char*>(msg), len);
            return 0;
        });
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));

    serverconnection serverConnection(1, 1, "./", "127.0.0.1");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    serverConnection.respondToQuery();

    // Assert
    ASSERT_EQ(
        "257 \"<root>/\"\n",
        response);
    ASSERT_EQ(
        "Connection 1: Working dir requested\n",
        out.str());
}

TEST_F(ServerConnectionTest, change_current_directory_command_is_processed_successfully)
{
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();

    EXPECT_CALL(*impl, recv)
        .WillRepeatedly([](int, void* buf, size_t len, int) -> int {
            std::string command = "CWD tests\n";
            strncpy(reinterpret_cast<char*>(buf), command.c_str(), len);
            return static_cast<int>(command.size());
        });
    std::string response;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&response](int, const void* msg, size_t len, int) -> int {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            response.assign(reinterpret_cast<const char*>(msg), len);
            return 0;
        });
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, opendir)
        .WillRepeatedly(Return(reinterpret_cast<DIR*>(0x12345678)));
    EXPECT_CALL(*impl, closedir)
        .WillRepeatedly(Return(0));

    serverconnection serverConnection(1, 1, "./", "127.0.0.1");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    serverConnection.respondToQuery();

    // Assert
    ASSERT_EQ(
        "250 directory changed to <root>/tests/\n",
        response);
    ASSERT_EQ(
        "Connection 1: Change of working dir to 'tests' requested\n"
        "Directory change to 'tests' successful!\n",
        out.str());
}

TEST_F(ServerConnectionTest, bye_command_is_processed_successfully)
{
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();

    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, recv)
        .WillRepeatedly([](int, void* buf, size_t len, int) -> int {
            std::string command = "BYE\n";
            strncpy(reinterpret_cast<char*>(buf), command.c_str(), len);
            return static_cast<int>(command.size());
        });
    std::string response;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&response](int, const void* msg, size_t len, int) -> int {
            response.clear();
            if (msg == nullptr || len == 0) {
                return -1;
            }

            response.assign(reinterpret_cast<const char*>(msg), len);
            return 0;
        });

    serverconnection serverConnection(1, 1, "./", "127.0.0.1");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    bool closeStatusBeforeProcessCommand = serverConnection.getCloseRequestStatus();

    // Act
    serverConnection.respondToQuery();

    // Assert
    ASSERT_EQ(
        "221 Goodbye.\n",
        response);
    ASSERT_EQ(
        "Connection 1: Shutdown of connection requested\n",
        out.str());
    ASSERT_FALSE(closeStatusBeforeProcessCommand);
    ASSERT_TRUE(serverConnection.getCloseRequestStatus());
}

TEST_F(ServerConnectionTest, quit_command_is_processed_successfully)
{
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();

    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, recv)
        .WillRepeatedly([](int, void* buf, size_t len, int) -> int {
            std::string command = "QUIT\n";
            strncpy(reinterpret_cast<char*>(buf), command.c_str(), len);
            return static_cast<int>(command.size());
        });
    std::string response;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&response](int, const void* msg, size_t len, int) -> int {
            response.clear();
            if (msg == nullptr || len == 0) {
                return -1;
            }

            response.assign(reinterpret_cast<const char*>(msg), len);
            return 0;
        });

    serverconnection serverConnection(1, 1, "./", "127.0.0.1");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    bool closeStatusBeforeProcessCommand = serverConnection.getCloseRequestStatus();

    // Act
    serverConnection.respondToQuery();

    // Assert
    ASSERT_EQ(
        "221 Goodbye.\n",
        response);
    ASSERT_EQ(
        "Connection 1: Shutdown of connection requested\n",
        out.str());
    ASSERT_FALSE(closeStatusBeforeProcessCommand);
    ASSERT_TRUE(serverConnection.getCloseRequestStatus());
}

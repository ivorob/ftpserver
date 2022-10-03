#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>

#include "Transmitter.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"

namespace {

class TransmitterTest : public ::testing::Test {
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

TEST_F(TransmitterTest, send_data_is_succeeded) {
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly(Return(0));

    std::string data;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&data](SOCKET, const void* msg, size_t len, int) -> ssize_t {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            data.assign(reinterpret_cast<const char*>(msg), len);
            return data.size();
        });

    Socket dataSocket(1);
    Transmitter transmitter(dataSocket, ".\n..\ntestFile\n");

    // Act
    ssize_t bytesSent = transmitter.transmit();

    // Assert
    ASSERT_EQ(14, bytesSent);
    ASSERT_EQ(".\n..\ntestFile\n", data);
}

TEST_F(TransmitterTest, send_data_by_parts_is_succeeded) {
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly(Return(0));

    std::string data;
    EXPECT_CALL(*impl, send)
        .WillRepeatedly([&data](SOCKET, const void* msg, size_t len, int) -> ssize_t {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            size_t currentLength = std::min<size_t>(len, 8L);
            data.append(reinterpret_cast<const char*>(msg), currentLength);
            return static_cast<ssize_t>(currentLength);
        });

    Socket dataSocket(1);
    Transmitter transmitter(dataSocket, ".\n..\ntestFile\n");

    // Act
    ssize_t bytesSent = transmitter.transmit();

    // Assert
    ASSERT_EQ(14, bytesSent);
    ASSERT_EQ(".\n..\ntestFile\n", data);
}

TEST_F(TransmitterTest, transmit_is_interrupted) {
    // Arrange
    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, close)
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*impl, getsockname)
        .WillRepeatedly(Return(0));

    std::string data;
    EXPECT_CALL(*impl, send)
        .WillOnce([&data](SOCKET, const void* msg, size_t len, int) -> ssize_t {
            if (msg == nullptr || len == 0) {
                return -1;
            }

            size_t currentLength = std::min<size_t>(len, 8L);
            data.append(reinterpret_cast<const char*>(msg), currentLength);
            return static_cast<ssize_t>(currentLength);
        })
        .WillRepeatedly(Return(-1));

    Socket dataSocket(1);
    Transmitter transmitter(dataSocket, ".\n..\ntestFile\n");

    // Act
    std::string exception;
    bool hasException = [&transmitter, &exception]() -> bool {
        try {
            std::ignore = transmitter.transmit();
            return false;
        } catch (const std::exception& e) {
            exception = e.what();
            return true;
        }
    }();

    // Assert
    ASSERT_TRUE(hasException);
    ASSERT_EQ("Cannot send data", exception);
}

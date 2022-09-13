#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "fileoperator.h"
#include "ScopedStreamRedirector.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"

namespace {

class CreateDirectoryTest : public ::testing::Test {
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

TEST_F(CreateDirectoryTest, cannot_create_parent_directory)
{
    fileoperator fileOperator("/");
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    std::string dirName = "../";
    bool actualResult = fileOperator.createDirectory(dirName);

    ASSERT_TRUE(actualResult);
    ASSERT_EQ("./", dirName);
    ASSERT_EQ(
        "Error: Deletion of dir beyond server root requested (prohibited)!\n",
        out.str());
}

TEST_F(CreateDirectoryTest, cannot_create_directory_if_it_exists)
{
    // Arrange
    fileoperator fileOperator("/");

    auto impl = makeImpl();
    EXPECT_CALL(*impl, mkdir)
        .WillRepeatedly([](const char* path, uint16_t) -> int {
            if (std::string(path) == "existingDirectory") {
                return -1;
            }

            return 0;
        });

    // Act
    std::string dirName = "existingDirectory";
    bool actualResult = fileOperator.createDirectory(dirName);

    // Assert
    ASSERT_FALSE(actualResult);
    ASSERT_EQ("existingDirectory/", dirName);
}

TEST_F(CreateDirectoryTest, directory_is_created_successfully)
{
    // Arrange
    fileoperator fileOperator("/");

    auto impl = makeImpl();
    EXPECT_CALL(*impl, mkdir)
        .WillRepeatedly([](const char* path, uint16_t) -> int {
            if (std::string(path) == "existingDirectory") {
                return -1;
            }

            return 0;
        });

    // Act
    std::string dirName = "newDir";
    bool actualResult = fileOperator.createDirectory(dirName);

    // Assert
    ASSERT_FALSE(actualResult);
    ASSERT_EQ("newDir/", dirName);
}

TEST_F(CreateDirectoryTest, path_is_unchangeable_without_strict_mode)
{
    // Arrange
    fileoperator fileOperator("/");

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, mkdir)
        .WillRepeatedly(Return(0));

    // Act
    std::string dirName = "../../../../newDir";
    bool actualResult = fileOperator.createDirectory(dirName, false);

    // Assert
    ASSERT_FALSE(actualResult);
    ASSERT_EQ("../../../../newDir", dirName);
}

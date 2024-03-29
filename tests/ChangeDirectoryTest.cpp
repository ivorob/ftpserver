#include <gtest/gtest.h>

#include "fileoperator.h"
#include "ScopedStreamRedirector.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"

namespace {

class ChangeDirectoryTest : public ::testing::Test {
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

TEST_F(ChangeDirectoryTest, cannot_change_directory_to_parent_if_we_is_in_higher_directory)
{
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);
    fileoperator fileOperator("/");

    bool actualResult = fileOperator.changeDir("..");

    ASSERT_TRUE(actualResult);
    ASSERT_EQ(
        "Error: Change beyond server root requested (prohibited)!\n",
        out.str());
}

TEST_F(ChangeDirectoryTest, cannot_change_directory_to_parent_if_we_is_in_higher_directory2)
{
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);
    fileoperator fileOperator("/");

    bool actualResult = fileOperator.changeDir("../");

    ASSERT_TRUE(actualResult);
    ASSERT_EQ(
        "Error: Change beyond server root requested (prohibited)!\n",
        out.str());
}

TEST_F(ChangeDirectoryTest, cannot_change_directory_to_the_same_directory)
{
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);
    fileoperator fileOperator("/");

    bool actualResult = fileOperator.changeDir(".");

    ASSERT_FALSE(actualResult);
    ASSERT_EQ(
        "Change to same dir requested (nothing done)!\n",
        out.str());
}

TEST_F(ChangeDirectoryTest, cannot_change_directory_to_the_same_directory2)
{
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);
    fileoperator fileOperator("/");

    bool actualResult = fileOperator.changeDir("./");

    ASSERT_FALSE(actualResult);
    ASSERT_EQ(
        "Change to same dir requested (nothing done)!\n",
        out.str());
}

TEST_F(ChangeDirectoryTest, cannot_change_directory_to_invalid_directory)
{
    // Arrange
    fileoperator fileOperator("/");

    using ::testing::Return;
    auto impl = makeImpl();
    EXPECT_CALL(*impl, canOpenDirectory)
        .WillRepeatedly(Return(false));

    // Act
    bool actualResult = fileOperator.changeDir("invalidDirectory");

    // Assert
    ASSERT_TRUE(actualResult);
}

TEST_F(ChangeDirectoryTest, directory_is_changed_successfully)
{
    // Arrange
    fileoperator fileOperator("/");

    using ::testing::Return;
    auto impl = makeImpl();

    EXPECT_CALL(*impl, canOpenDirectory)
        .WillRepeatedly(Return(true));

    // Act
    bool actualResult = fileOperator.changeDir("validDirectory");

    // Assert
    ASSERT_FALSE(actualResult);
}

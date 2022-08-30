#include <gtest/gtest.h>

#include "fileoperator.h"
#include "ScopedStreamRedirector.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"

namespace {

class DeleteFileTest : public ::testing::Test {
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

TEST_F(DeleteFileTest, cannot_delete_file_with_invalid_name)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, remove)
        .WillRepeatedly(Return(-1));

    // Act
    int actualResult = fileOperator.deleteFile("");

    // Assert
    ASSERT_EQ(EXIT_FAILURE, actualResult);
    ASSERT_EQ(
        "Error deleting file ''\n",
        out.str());
}

TEST_F(DeleteFileTest, cannot_delete_not_existing_file)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, remove)
        .WillRepeatedly(Return(-1));

    // Act
    int actualResult = fileOperator.deleteFile("invalidFile");

    // Assert
    ASSERT_EQ(EXIT_FAILURE, actualResult);
    ASSERT_EQ(
        "Error deleting file 'invalidFile'\n",
        out.str());
}

TEST_F(DeleteFileTest, file_is_deleted_successfully)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, remove)
        .WillRepeatedly(Return(0));

    // Act
    int actualResult = fileOperator.deleteFile("emptyFile");

    // Assert
    ASSERT_EQ(EXIT_SUCCESS, actualResult);
    ASSERT_EQ(
        "File 'emptyFile' deleted\n",
        out.str());
    const auto& deletedFiles = fileOperator.getListOfDeletedFiles();
    ASSERT_EQ(std::vector<std::string>{"emptyFile"}, deletedFiles);
}

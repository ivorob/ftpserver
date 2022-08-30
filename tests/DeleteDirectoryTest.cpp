#include <gtest/gtest.h>

#include "fileoperator.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"
#include "ScopedStreamRedirector.h"
#include "BrowseUtils.h"

namespace {

class DeleteDirectoryTest : public ::testing::Test {
public:
    void TearDown() override {
        setImpl({});
    }
};

std::shared_ptr<MockOSApiImpl> makeImpl()
{
    auto impl = std::make_shared<MockOSApiImpl>();
    setImpl(impl);
    return impl;
}

std::vector<struct dirent> makeTestDir() {
    std::vector<struct dirent> testDir;
    testDir.push_back(BrowseUtils::makeDirEntry(".", DT_DIR));
    testDir.push_back(BrowseUtils::makeDirEntry("..", DT_DIR));
    testDir.push_back(BrowseUtils::makeDirEntry("file1", DT_REG));
    testDir.push_back(BrowseUtils::makeDirEntry("file2", DT_REG));
    testDir.push_back(BrowseUtils::makeDirEntry("file3", DT_REG));
    return testDir;
}

}

TEST_F(DeleteDirectoryTest, cancel_operation_doesnt_delete_anything)
{
    // Arrange
    fileoperator fileOperator("/");

    // Act
    bool actualResult = fileOperator.deleteDirectory("test", true, "./");

    // Assert
    ASSERT_TRUE(actualResult);
}

TEST_F(DeleteDirectoryTest, directory_is_deleted_successfully)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, opendir)
        .WillRepeatedly(Return(reinterpret_cast<DIR*>(0x12345678)));
    EXPECT_CALL(*impl, closedir)
        .WillRepeatedly(Return(0));

    std::vector<struct dirent> dirs = makeTestDir();
    size_t i = 0;
    EXPECT_CALL(*impl, readdir)
        .WillRepeatedly([&dirs, &i](DIR*) -> struct dirent* {
            if (i < dirs.size()) {
                return &dirs[i++];
            }

            return nullptr;
        });

    int countDeletedFiles = 0;
    EXPECT_CALL(*impl, remove)
        .WillRepeatedly([&countDeletedFiles](const char* path) -> int {
            if (path == nullptr) {
                return -1;
            }

            std::string currentPath(path);
            if (currentPath == "./test/file1" ||
                currentPath == "./test/file2" ||
                currentPath == "./test/file3") {
                ++countDeletedFiles;
                return 0;
            }

            return -1;
        });
    EXPECT_CALL(*impl, rmdir)
        .WillRepeatedly([&countDeletedFiles](const char* path) -> int {
            if (path == nullptr) {
                return -1;
            }

            std::string currentPath(path);
            if (currentPath == "./test/") {
                return 0;
            }

            return -1;
        });

    // Act
    bool actualResult = fileOperator.deleteDirectory("test", false, "./");

    // Assert
    ASSERT_FALSE(actualResult);
    ASSERT_EQ(3, countDeletedFiles);
    ASSERT_EQ(
        "Browsing './test/'\n"
        "File './test/file1' deleted\n"
        "File './test/file2' deleted\n"
        "File './test/file3' deleted\n"
        "Directory './test/' deleted\n",
        out.str());
}

TEST_F(DeleteDirectoryTest, unavailability_to_delete_file_inside_directory_cancels_delete_directory_operation)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, opendir)
        .WillRepeatedly(Return(reinterpret_cast<DIR*>(0x12345678)));
    EXPECT_CALL(*impl, closedir)
        .WillRepeatedly(Return(0));

    std::vector<struct dirent> dirs = makeTestDir();
    size_t i = 0;
    EXPECT_CALL(*impl, readdir)
        .WillRepeatedly([&dirs, &i](DIR*) -> struct dirent* {
            if (i < dirs.size()) {
                return &dirs[i++];
            }

            return nullptr;
        });

    int countDeletedFiles = 0;
    EXPECT_CALL(*impl, remove)
        .WillRepeatedly([&countDeletedFiles](const char* path) -> int {
            if (path == nullptr) {
                return -1;
            }

            std::string currentPath(path);
            if (currentPath == "./test/file1" ||
                currentPath == "./test/file3") {
                ++countDeletedFiles;
                return 0;
            }

            return -1;
        });
    EXPECT_CALL(*impl, rmdir)
        .WillRepeatedly(Return(-1));

    // Act
    bool actualResult = fileOperator.deleteDirectory("test", false, "./");

    // Assert
    ASSERT_TRUE(actualResult);
    ASSERT_EQ(2, countDeletedFiles);
    ASSERT_EQ(
        "Browsing './test/'\n"
        "File './test/file1' deleted\n"
        "File './test/file3' deleted\n",
        out.str());
}

#include <gtest/gtest.h>

#include "fileoperator.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"
#include "ScopedStreamRedirector.h"

namespace {

class BrowseDirectoryTest : public ::testing::Test {
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

struct dirent makeDirEntry(const std::string& name, uint8_t type)
{
    struct dirent dirEntry = {0};
#if defined(__APPLE_CC__)
    dirEntry.d_ino = 0;
#else
    dirEntry.d_fileno = 0;
    dirEntry.d_off = 0;
#endif
    dirEntry.d_reclen = sizeof(struct dirent);
    dirEntry.d_type = type;
#if defined(__FreeBSD__) || defined(__APPLE_CC__)
    dirEntry.d_namlen = name.size();
#endif
    strncpy(dirEntry.d_name, name.c_str(), sizeof(dirEntry.d_name));
    return dirEntry;
}

std::vector<struct dirent> makeDirEntries() {
    std::vector<struct dirent> dirEntries;
    dirEntries.push_back(makeDirEntry(".", DT_DIR));
    dirEntries.push_back(makeDirEntry("..", DT_DIR));
    dirEntries.push_back(makeDirEntry("test", DT_DIR));
    dirEntries.push_back(makeDirEntry("file1", DT_REG));
    dirEntries.push_back(makeDirEntry("file2", DT_REG));
    dirEntries.push_back(makeDirEntry("file3", DT_REG));
    return dirEntries;
}

std::vector<struct dirent> dirEntries = makeDirEntries();

}

TEST_F(BrowseDirectoryTest, cannot_browse_invalid_directory)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out, errorOut;
    ScopedStreamRedirector streamRedirector(std::cout, out);
    ScopedStreamRedirector errorStreamRedirector(std::cerr, errorOut);

    using ::testing::Return;

    auto impl = makeImpl();
    EXPECT_CALL(*impl, opendir)
        .WillRepeatedly(Return(nullptr));

    // Act
    std::vector<std::string> directories;
    std::vector<std::string> files;
    fileOperator.browse("invalidDirectory", directories, files);

    // Assert
    ASSERT_TRUE(directories.empty());
    ASSERT_TRUE(files.empty());
    ASSERT_EQ(
        "Browsing 'invalidDirectory/'\n",
        out.str());
    ASSERT_EQ(
        "Error: Directory 'invalidDirectory/' could not be opened!\n",
        errorOut.str());
}

TEST_F(BrowseDirectoryTest, browse_directory_is_succeeded)
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
    EXPECT_CALL(*impl, readdir)
        .WillRepeatedly([](DIR*) -> struct dirent* {
            static size_t i = 0;
            if (i < dirEntries.size()) {
                return &dirEntries[i++];
            }

            return nullptr;
        });

    // Act
    std::vector<std::string> directories;
    std::vector<std::string> files;
    fileOperator.browse("validDirectory", directories, files);

    // Assert
    ASSERT_EQ(2, directories.size());
    ASSERT_EQ(3, files.size());
    ASSERT_EQ(
        "Browsing 'validDirectory/'\n",
        out.str());
}

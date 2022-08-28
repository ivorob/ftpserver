#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>

#include "fileoperator.h"
#include "FakeOSApi.h"
#include "MockOSApiImpl.h"
#include "ScopedStreamRedirector.h"

namespace fs = std::filesystem;

namespace {

class ReadFileTest : public ::testing::Test {
public:
    void SetUp() override {
        std::error_code errorCode;
        fs::remove("invalidFile", errorCode);
    }
};

}

TEST_F(ReadFileTest, cannot_open_not_existing_file)
{
    // Arrange
    fileoperator fileOperator("/");
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    std::ifstream input = fileOperator.readFile("invalidFile");

    // Assert
    ASSERT_FALSE(input);
    ASSERT_EQ(
        "Reading file 'invalidFile' failed!\n",
        out.str());
}

TEST_F(ReadFileTest, existing_file_is_open_successfully)
{
    // Arrange
    fileoperator fileOperator("/");

    std::string validFileName = "validFileName";
    std::ofstream outFile(validFileName);

    // Act
    std::ifstream input = fileOperator.readFile(validFileName);

    // Assert
    ASSERT_TRUE(input);
}

TEST_F(ReadFileTest, cannot_read_data_from_empty_file)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    std::string validFileName = "validFileName";
    std::ofstream outFile(validFileName);

    std::ifstream input = fileOperator.readFile(validFileName);

    // Act
    unsigned long fileSize = 0;
    std::unique_ptr<char[]> memoryBlock = fileOperator.readFileBlock(input, fileSize);

    // Assert
    ASSERT_FALSE(memoryBlock);
    ASSERT_EQ(0, fileSize);
    ASSERT_EQ(
        "Try to read empty file!\n",
        out.str());
}

TEST_F(ReadFileTest, read_block_operation_provides_full_file_content)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    std::string validFileName = "validFileName";
    std::ofstream outFile(validFileName);
    outFile << "test data";
    outFile.close();

    std::ifstream input = fileOperator.readFile(validFileName);

    // Act
    unsigned long fileSize = 0;
    std::unique_ptr<char[]> memoryBlock = fileOperator.readFileBlock(input, fileSize);

    // Assert
    ASSERT_TRUE(memoryBlock);
    ASSERT_EQ(9, fileSize);
    ASSERT_EQ("test data", std::string(memoryBlock.get(), 9));
    ASSERT_EQ(
        "Reading 9 Bytes\n",
        out.str());
}

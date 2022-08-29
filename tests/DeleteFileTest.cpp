#include <gtest/gtest.h>

#include "fileoperator.h"
#include "ScopedStreamRedirector.h"

TEST(DeleteFileTest, cannot_delete_file_with_invalid_name)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    // Act
    int actualResult = fileOperator.deleteFile("");

    // Assert
    ASSERT_EQ(EXIT_FAILURE, actualResult);
    ASSERT_EQ(
        "Error deleting file ''\n",
        out.str());
}

TEST(DeleteFileTest, cannot_delete_not_existing_file)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    // Act
    int actualResult = fileOperator.deleteFile("invalidFile");

    // Assert
    ASSERT_EQ(EXIT_FAILURE, actualResult);
    ASSERT_EQ(
        "Error deleting file 'invalidFile'\n",
        out.str());
}

TEST(DeleteFileTest, file_is_deleted_successfully)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    std::ofstream emptyFile("emptyFile");

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

#include <gtest/gtest.h>

#include "fileoperator.h"
#include "FakeOSApi.h"
#include "ScopedStreamRedirector.h"

TEST(AppendFileTest, cannot_open_file_with_invalid_filename)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    // Act
    int actualResult = fileOperator.beginWriteFile("");

    // Assert
    ASSERT_EQ(EXIT_FAILURE, actualResult);
    ASSERT_EQ(
        "Cannot open output file ''\n",
        out.str());
}

TEST(AppendFileTest, open_file_for_appending_is_succeeded)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    int actualResult = fileOperator.beginWriteFile("validFile");

    // Assert
    ASSERT_EQ(EXIT_SUCCESS, actualResult);
    ASSERT_EQ(
        "Beginning writing to file 'validFile'\n",
        out.str());
}

TEST(AppendFileTest, cannot_write_to_invalid_stream)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    // Act
    int actualResult = fileOperator.writeFileBlock("test data");

    // Assert
    ASSERT_EQ(EXIT_FAILURE, actualResult);
    ASSERT_EQ(
        "Cannot write to output file\n",
        out.str());
}

TEST(AppendFileTest, append_data_to_file_is_succeeded)
{
    // Arrange
    fileoperator fileOperator("/");
    fileOperator.beginWriteFile("validFile");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    int actualResult = fileOperator.writeFileBlock("test data");

    // Assert
    ASSERT_EQ(EXIT_SUCCESS, actualResult);
    ASSERT_EQ(
        "Appending to file\n",
        out.str());
}

TEST(AppendFileTest, closing_not_opened_file_does_nothing)
{
    // Arrange
    fileoperator fileOperator("/");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    fileOperator.closeWriteFile();

    // Assert
    ASSERT_EQ(
        "",
        out.str());
}

TEST(AppendFileTest, opened_file_is_closed_successfully)
{
    // Arrange
    fileoperator fileOperator("/");
    fileOperator.beginWriteFile("validFile");

    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    fileOperator.closeWriteFile();

    // Assert
    ASSERT_EQ(
        "Closing open file\n",
        out.str());
}

TEST(AppendFileTest, opened_file_is_closed_after_destroying_fileoperator_object)
{
    // Arrange
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cout, out);

    // Act
    {
        fileoperator fileOperator("/");
        fileOperator.beginWriteFile("validFile");
    }

    // Assert
    ASSERT_EQ(
        "Beginning writing to file 'validFile'\n"
        "Closing open file\n",
        out.str());
}

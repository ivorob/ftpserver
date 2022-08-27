#include <gtest/gtest.h>

#include "fileoperator.h"
#include "ScopedStreamRedirector.h"

TEST(FileOperatorTest, cannot_change_directory_to_parent_if_we_is_in_higher_directory)
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

TEST(FileOperatorTest, cannot_change_directory_to_parent_if_we_is_in_higher_directory2)
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

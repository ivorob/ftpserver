#include <gtest/gtest.h>

#include "servercore.h"
#include "ScopedStreamRedirector.h"
#include "FakeOSApi.h"
#include "FakeOSApiImpl.h"

TEST(FtpServerTest, error_allocating_socket_prints_errors)
{
    setImpl(std::make_shared<DummyOSApi>());
    std::ostringstream out;
    ScopedStreamRedirector streamRedirector(std::cerr, out);

    servercore ftpServer(4242, "/");

    ASSERT_EQ(
        "Directory could not be changed to '/'!\n"
        "socket() failed\n",
        out.str());
}

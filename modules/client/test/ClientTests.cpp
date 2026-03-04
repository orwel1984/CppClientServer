#include "Client.h"

#include <boost/asio/io_context.hpp>
#include <gtest/gtest.h>

// quick smoke test to make sure the templated client builds and
// exercises the policy functions.  No real network is required; we
// connect to localhost on a port unlikely to be open and simply verify
// that the retry/logging code path doesn't throw.

TEST(ClientPolicy, TcpAndUdpConnect)
{
    boost::asio::io_context io;

    Client<impl::protocol::TCP> tcp(io, "127.0.0.1", 65000);
    EXPECT_NO_THROW(tcp.connect());
    tcp.disconnect();

    Client<impl::protocol::UDP> udp(io, "127.0.0.1", 65000);
    EXPECT_NO_THROW(udp.connect());
    udp.disconnect();
}

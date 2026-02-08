#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <thread>

#include "server.h"
#include "policy_udp_boost.hpp"
#include "policy_mode.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;
using endpoint = boost::asio::ip::udp::endpoint;
using udp_socket = boost::asio::ip::udp::socket;

class UDPServerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        worker = std::make_unique<std::thread>([this]() { context.run(); });
        startServer();
    }

    void TearDown() override
    {
        context.stop();
        if (worker && worker->joinable())
        {
            worker->join();
        }
    }

private:

    void setupTestPacketHandler()
    {
        auto packetHandler =
            [this](const std::error_code& ec, std::size_t len, std::string_view data)
        {
            if (!ec)
            {
                packet = std::string(data.data(), len);
                packetReceived = true;
            }
        };
        server->setPacketHandler(packetHandler);
    }

    void initServer()
    {
        server = std::make_unique<Server<impl::protocol::UDP, impl::mode::Async>>(context, serverPort);
        setupTestPacketHandler();
    }
    
    void startServer()
    {
        initServer();
        auto result = server->start();
        ASSERT_FALSE(result) << "Server failed to start: " << result.message();        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // give server time to start
    }

protected:
    // Test configuration
    const uint16_t serverPort = 12345;

    io_context context;
    io_context::work workGuard{context};  // Initialize directly with context

    std::unique_ptr<std::thread> worker;
    std::unique_ptr<Server<impl::protocol::UDP, impl::mode::Async>> server;

    // Test state
    std::string packet;
    bool packetReceived = false;
};

TEST_F(UDPServerTest, ReceiveHelloPacket)
{
    // Reset test state
    packet.clear();
    packetReceived = false;

    // Server Endpoint
    udp::endpoint serverEndpoint(address::from_string("127.0.0.1"), serverPort);

    // Test Message
    const std::string testMessage = "Hello World";

    // Create a client-socket to send test message
    udp::socket socket(context);
    socket.open(udp::v4());
    socket.send_to(buffer(testMessage), serverEndpoint);

    // Wait for processing
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Verify results
    EXPECT_TRUE(packetReceived) << "Server did not receive any data";
    EXPECT_EQ(packet, testMessage) << "Received: '" << packet << "', Expected: " << testMessage;
}
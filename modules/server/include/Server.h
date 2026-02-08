#pragma once

#include <atomic>
#include <functional>

#include "GenericServer.h"

#define UDP_PACKET_SIZE 1024
using PacketHandler =
    std::function<void(const std::error_code &, std::size_t, std::string_view)>;

/**
    A Server class that implements the GenericServer interface based on 
    selected plicies for Protocol and Mode  :

   - The Protocol Policy:  Decides whether this is a TCP/UDP/CAN based server and provide a Socket that can support it. 
   - The Mode Policy: Determines if the server should run in asynchronous or synhcronous mode.

   Usage Example:

    Server<UDP, Async> server(io, port, packet_handler);
    server.start();
    server.stop();

 */
template <typename Protocol, template <typename> class Mode>
class Server : public GenericServer
{
  using Socket = typename Protocol::Socket;
  using Endpoint = typename Protocol::Endpoint;
  using Context = typename Protocol::Context;
  using Error_Code = typename Protocol::error_code;

public:
  Server(Context &io, uint16_t port, PacketHandler handler = nullptr);

  // Generic-Server interface
  std::error_code start() override;
  std::error_code stop() override;
  std::error_code shutdown() override;

  // Packet Handling
  void setPacketHandler(PacketHandler handler) {
    m_packetHandler = std::move(handler);
  }

protected:
  void onPacketReceived(const Error_Code &error, std::size_t len, std::string_view packet);

private:
  Socket m_socket;
  Endpoint m_remoteEndpoint;
  uint16_t m_port;

  std::vector<char> m_buffer;
  std::atomic<bool> m_running;
  
  PacketHandler m_packetHandler;
};

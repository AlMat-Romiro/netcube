#pragma once

#include <thread>
#include <string>
#include <boost/asio.hpp>
#include <glm/glm.hpp>

namespace Network
{
    class Server_connection
    {
    public:
        Server_connection(boost::asio::io_service* service, int port);

        void send(std::string message);
        void send(float message);
        void send(glm::vec3 message);

        std::string recive();
        float recive_f();
        glm::vec3 recive_vec3();

        void disconnect();
    private:
        boost::asio::ip::tcp::socket* socket;
    };

    class Server
    {
    public:
        Server(int port);
        void async_acceptConnections(bool* running);
        std::vector<Network::Server_connection> clients;
    private:
        void m_f_acceptConnection();

        boost::asio::io_service m_service;
        int m_port;
        std::thread m_acceptConnectionsThread;
        bool* m_ptr_running;
    };

    class Client
    {
    public:
        Client(std::string ip_v4, int port);

        void send(std::string message);
        void send(float message);
        void send(glm::vec3 message);

        std::string recive();
        float recive_f();
        glm::vec3 recive_vec3();

        void disconnect();
    private:
        boost::asio::io_service m_service;
        boost::asio::ip::tcp::socket* m_socket;
    };
}

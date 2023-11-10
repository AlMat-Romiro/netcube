#include "../include/Network.h"

void f_send(std::string message, boost::asio::ip::tcp::socket* socket)
{
    int size = message.size();
    socket->send(boost::asio::buffer(&size, sizeof(int)));

    char data [size];
    strncpy(data, message.c_str(), size);

    socket->send(boost::asio::buffer(data, size));
}

void f_send(float message, boost::asio::ip::tcp::socket* socket)
{
    socket->send(boost::asio::buffer(&message, sizeof(float)));
}

void f_send(glm::vec3 message, boost::asio::ip::tcp::socket* socket)
{
    socket->send(boost::asio::buffer(&message.x, sizeof(float)));
    socket->send(boost::asio::buffer(&message.y, sizeof(float)));
    socket->send(boost::asio::buffer(&message.z, sizeof(float)));
}

std::string f_recive(boost::asio::ip::tcp::socket* socket)
{
    int size;
    socket->receive(boost::asio::buffer(&size, sizeof(int)));
    char data [size];
    socket->receive(boost::asio::buffer(data, size));

    std::string message(data, size);

    return message;
}

float f_recive_float(boost::asio::ip::tcp::socket* socket)
{
    float message;
    socket->receive(boost::asio::buffer(&message, sizeof(float)));

    return message;
}

glm::vec3 f_recive_vec3(boost::asio::ip::tcp::socket* socket)
{
    glm::vec3 message;

    message.x = f_recive_float(socket);
    message.y = f_recive_float(socket);
    message.z = f_recive_float(socket);

    return message;
}


Network::Server_connection::Server_connection(boost::asio::io_service* service, int port)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    boost::asio::ip::tcp::acceptor acceptor(*service, endpoint);
    socket = new boost::asio::ip::tcp::socket(*service);

    acceptor.accept(*socket);
}


void Network::Server_connection::send(std::string message)
{
    f_send(message, socket);
}

void Network::Server_connection::send(float message)
{
    f_send(message, socket);
}

void Network::Server_connection::send(glm::vec3 message)
{
    f_send(message, socket);
}



std::string Network::Server_connection::recive()
{
    return f_recive(socket);
}

float Network::Server_connection::recive_f()
{
    return f_recive_float(socket);
}

glm::vec3 Network::Server_connection::recive_vec3()
{
    return f_recive_vec3(socket);
}



Network::Server_connection::~Server_connection()
{
    //socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    //socket->close();
    //delete socket;
}


/* ------------------------------ */

Network::Server::Server(int port)
{
    m_port = port;
}

void Network::Server::m_f_acceptConnection()
{
    while (*m_ptr_running)
    {
        clients.push_back(Network::Server_connection(&m_service, m_port));
    }
}


void Network::Server::async_acceptConnections(bool* running)
{
    m_acceptConnectionsThread = std::thread(&Network::Server::m_f_acceptConnection, this);
    m_ptr_running = running;
}


/* ------------------------------ */

Network::Client::Client(std::string ip_v4, int port)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip_v4), port);
    m_socket = new boost::asio::ip::tcp::socket(m_service);
    m_socket->connect(endpoint);
}

void Network::Client::send(std::string message)
{
    f_send(message, m_socket);
}

void Network::Client::send(float message)
{
    f_send(message, m_socket);
}

void Network::Client::send(glm::vec3 message)
{
    f_send(message, m_socket);
}



std::string Network::Client::recive()
{
    return f_recive(m_socket);
}

float Network::Client::recive_f()
{
    return f_recive_float(m_socket);
}

glm::vec3 Network::Client::recive_vec3()
{
    return f_recive_vec3(m_socket);
}



Network::Client::~Client()
{
    //m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    //m_socket->close();

    //delete m_socket;
}

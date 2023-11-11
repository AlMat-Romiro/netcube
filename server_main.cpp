#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include "include/Network.h"
#include "include/Player.h"
#include "include/Log.h"


std::vector <Player> players(0);
bool running = true;
std::mutex mtx;

void client_handler(Network::Server_connection* client)
{
    Log("Новый поток с клиентом");

    const int playerID = players.size();
    Log("ID: " + std::to_string(playerID));

    mtx.lock();
    players.push_back(Player());
    players[playerID].nickname = client->recive();
    Log("Имя: " + players[playerID].nickname);

    players[playerID].position = client->recive_vec3();
    players[playerID].rotation = client->recive_vec3();
    players[playerID].scale    = client->recive_vec3();
    mtx.unlock();

    while(running)
    {
        int playersCount = players.size();
        client->send(playersCount - 1);
        Log("Количество клиентов: " + std::to_string(playersCount));
        Log("Отправленное количество клиентов: " + std::to_string(playersCount-1));
        for (int i = 0; i < playersCount; i++)
        {
            if(i != playerID)
            {
                Log("1");
                client->send(players[i].nickname);
                client->send(players[i].position);
                client->send(players[i].rotation);
                client->send(players[i].scale);
            }
        }
        mtx.lock();
        Log("2");
        players[playerID].position = client->recive_vec3();
        players[playerID].rotation = client->recive_vec3();
        players[playerID].scale    = client->recive_vec3();
        mtx.unlock();
    }
    Log("Завершения потока " + std::to_string(playerID));
    client->disconnect();
}


int main(int argc, char** argv)
{
    int clientsCount = 0;
    int port;
    std::vector<std::thread> clientsHandlers;

    if (argc > 1)
        port = atoi(argv[1]);
    else
        port = 6666;

    Network::Server server(port);
    Log("Сервер запущен с портом " + std::to_string(port));

    server.async_acceptConnections(&running);
    Log("Готов принимать соединения!");

    players.push_back(Player());
    players[0].nickname = "aff";

    while(running)
    {
        if(clientsCount < server.clients.size())
        {
            Log("Подключен новый клиент.");
            clientsHandlers.push_back(std::thread(std::bind(client_handler, &server.clients[clientsCount])));
            clientsCount++;
        }
    }

    return 0;
}

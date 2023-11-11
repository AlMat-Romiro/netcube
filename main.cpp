#include <fmt/core.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <string>
#include <vector>
#include <random>
#include <chrono>

#include <iostream>

#include "include/Texture.h"
#include "include/Controls.h"
#include "include/Shaders.h"
#include "include/Network.h"
#include "include/Player.h"
#include "include/Log.h"


Player player;
std::vector<Player> players;
bool running = true;


void client_f(std::string IP_v4, int port)
{
	Network::Client client(IP_v4, port);
	Log("Подключен к серверу!");

	client.send(player.nickname);
	client.send(player.position);
	client.send(player.rotation);
	client.send(player.scale);
	Log("1");

	while(running)
	{
		int srv_playersCount = client.recive_f();
		if(players.size() < srv_playersCount)
		{
			for(int i = 0; i < srv_playersCount; i++)
			{
				players.push_back(Player());
				Log("added new player");
			}
		}
		Log("Players count = " + std::to_string(players.size()));

		for(int i = 0; i < players.size(); i++)
		{
			Log("Recived client ID = " + std::to_string(i));
			players[i].position = client.recive_vec3();
			players[i].rotation = client.recive_vec3();
			players[i].scale    = client.recive_vec3();
		}
		client.send(player.position);
		client.send(player.rotation);
		client.send(player.scale);
		Log("sended myself data");
	}
	Log("Отключение от сервера...");
	client.disconnect();
}

void f_clientUpdate(Network::Client& client)
{
	int srv_playersCount = client.recive_f();
	if(players.size() < srv_playersCount)
	{
		for(int i = 0; i < srv_playersCount; i++)
		{
			players.push_back(Player());
			Log("added new player");
		}
	}
	Log("Players count = " + std::to_string(players.size()));

	for(int i = 0; i < players.size(); i++)
	{
		Log("Recived client ID = " + std::to_string(i));
		players[i].position = client.recive_vec3();
		/*players[i].rotation = */client.recive_vec3();
		/*players[i].scale = */client.recive_vec3();
	}
	client.send(player.position);
	client.send(player.rotation);
	client.send(player.scale);
	Log("sended myself data");
}

int main(int argc, char** argv)
{
	std::thread clientThread;
	Network::Client* client;

	if(argc > 1)
	{
		std::string input = argv[1];

		if(input == "connect" && argc == 4)
		{
			player.nickname = argv[3];

			client = new Network::Client(argv[2], 6666);
			Log("Подключен к серверу!");

			client->send(player.nickname);
			client->send(player.position);
			client->send(player.rotation);
			client->send(player.scale);

			//clientThread = std::thread(std::bind(client_f, std::string(argv[2]), 6666));

		} else {
			std::cout << "Неизвестный аргумент.\n" <<
				"\t- ./netcube connect [ip] [nickname]\n";
		}
	}

	if(!glfwInit())
	{
	    fmt::print(stderr, "Что то пошло не так с GLFW...\n");
	    
	    return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "EDu", nullptr, nullptr);
	if(window == nullptr)
	{
		fmt::print(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;		
	}
	
	glfwMakeContextCurrent(window);
			
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		fmt::print(stderr, "Failed to init GLEW\n");
		
		return -1;
	}
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VAO_id;
	glGenVertexArrays(1, &VAO_id);
	glBindVertexArray(VAO_id);
	
	GLuint pid = Shaders::load("res/triangle.vs", "res/triangle.fs");
	    
    GLuint matrix_id = glGetUniformLocation(pid, "MVP");
    GLuint text_sampler  = glGetUniformLocation(pid, "texture_sampler");

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(text_sampler, 0);
    
    Texture test("res/t_test.png");
    Texture test1("res/t_test1.png");
    Texture test2("res/t_test2.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, test.get_width(), test.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, test.get_data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	static const GLfloat d_VBO[] = 
	{
	   // 1 face
		 1.0f,  1.0f, -1.0f, // 1 triangle
		 1.0f, -1.0f, -1.0f,
 		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, // 2 triangle
        -1.0f,  1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f, 
	
	   // 2 face
	    -1.0f,  1.0f,  1.0f, // 1 triangle
	    -1.0f, -1.0f,  1.0f,
 		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f, // 2 triangle
         1.0f,  1.0f,  1.0f, 
	    -1.0f, -1.0f,  1.0f,  
		 
	   // 3 face
	    -1.0f,  1.0f, -1.0f, // 2 triangle
        -1.0f, -1.0f, -1.0f, 
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, // 1 triangle
		-1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f, -1.0f,  
		
	   // 4 face
		 1.0f,  1.0f,  1.0f,  // 1 triangle
		 1.0f, -1.0f,  1.0f, 
		 1.0f,  1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f,  // 2 triangle
		 1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,  
		 
	   // 5 face
		-1.0f,  1.0f, -1.0f, // 1 triangle
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,  
		 1.0f,  1.0f,  1.0f, // 2 triangle
         1.0f,  1.0f, -1.0f, 
		-1.0f,  1.0f,  1.0f, 
		
	   // 6 face
		 1.0f, -1.0f, -1.0f,  // 1 triangle
		 1.0f, -1.0f,  1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f,  1.0f,  // 2 triangle
		-1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f
    };

    static const GLfloat d_UV[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0  
    };

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(d_VBO), d_VBO, GL_STATIC_DRAW);
	
	GLuint UV;
	glGenBuffers(1, &UV);
	glBindBuffer(GL_ARRAY_BUFFER, UV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(d_UV), d_UV, GL_STATIC_DRAW);
	
//	std::uniform_real_distribution<float> dist(0.0f, 360.0f);
//	std::mt19937 gen;
	
	bool is_wire = false;
	size_t frame = 0;
	
	glm::vec3 translate(0.0f);
	glm::vec3 translate1(2.0f);
	glm::vec3 rotate(0.0f);
	glm::vec3 scale(1.0f);

	do
	{
	    static double last_time = glfwGetTime();
	
	    double current_time = glfwGetTime();
	    float delta_time = static_cast<float>(current_time - last_time);
	    
        //fmt::print("{}\n", frame);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		glUseProgram(pid);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, UV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		
		Controls::compute_input_mat(window);
		glm::mat4 proj = Controls::get_proj_mat();
        glm::mat4 view = Controls::get_view_mat();

		/*//draw local player
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, player.position);
		model = glm::rotate(model, glm::radians(player.position.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(player.position.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(player.position.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, player.scale);

		glm::mat4 mvp = proj * view * model;

		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, (sizeof(d_VBO) / sizeof(d_VBO[0])) / 3);*/

		player.position = Controls::get_cam_pos();

		f_clientUpdate(*client);

		//draw other players
		for(int i = 0; i < players.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			model = glm::translate(model, players[i].position);
			model = glm::rotate(model, glm::radians(players[i].position.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(players[i].position.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(players[i].position.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, players[i].scale);

			glm::mat4 mvp = proj * view * model;

			glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, (sizeof(d_VBO) / sizeof(d_VBO[0])) / 3);
		}
	    

		
		if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, test.get_width(), test.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, test.get_data());
		}
		
		if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, test1.get_width(), test1.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, test1.get_data());
		}
		
		if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, test2.get_width(), test2.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, test2.get_data());
		}
		
		if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            is_wire = !is_wire;
        }
		
		if(is_wire)
		{
		    glDrawArrays(GL_LINES, 0, (sizeof(d_VBO) / sizeof(d_VBO[0])) / 3);
		}
		else
		{
		    ///glDrawArrays(GL_TRIANGLES, 0, (sizeof(d_VBO) / sizeof(d_VBO[0])) / 3);    
		}
				
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	
		glfwPollEvents();
		glfwSwapBuffers(window);
	
        frame++;
        
        last_time = current_time;
	}
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		  glfwWindowShouldClose(window) == 0);
		  
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UV);
	glDeleteVertexArrays(1, &VAO_id);
	glDeleteTextures(1, &texture_id);
	glDeleteProgram(pid);

	glfwTerminate();

	return 0;
}

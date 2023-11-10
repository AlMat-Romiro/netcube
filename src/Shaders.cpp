#include "../include/Shaders.h"

#include <fmt/core.h>

#include <fstream>
#include <sstream>
#include <vector>

GLuint Shaders::load(std::string vs_path, std::string fs_path)
{
	GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::string vs_code;
	std::ifstream vs_stream(vs_path, std::ios::in);
	if(!vs_stream.is_open())
	{
	    fmt::print(stderr, "File with vertex shader doesn't exist.\n");
	    return -1;
	}
	
	std::stringstream sstr;
	sstr << vs_stream.rdbuf();
	vs_code = sstr.str();
	vs_stream.close();
	
	std::string fs_code;
	std::ifstream fs_stream(fs_path, std::ios::in);
	if(!fs_stream.is_open())
	{
	    fmt::print(stderr, "File with fragment shader doesn't exist.\n");
	    return -1;
	}
	
	sstr.str(std::string());
	sstr.clear();
	sstr << fs_stream.rdbuf();
	fs_code = sstr.str();
	fs_stream.close();
	
	GLint res = GL_FALSE;
	int info_log_len;
	
	fmt::print("Компиляция шейдера: {}\n", vs_path);
	char const* vs_pointer = vs_code.c_str();
	glShaderSource(vs_id, 1, &vs_pointer, nullptr);
	glCompileShader(vs_id);
	
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &res);
    glGetShaderiv(vs_id, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 0)
    {
        std::vector<char> vs_err_msg(info_log_len + 1);
        glGetShaderInfoLog(vs_id, info_log_len, nullptr, &vs_err_msg[0]);
        fmt::print("{}\n", &vs_err_msg[0]);
    }

    fmt::print("Компиляция шейдера: {}\n", fs_path);
	char const* fs_pointer = fs_code.c_str();
	glShaderSource(fs_id, 1, &fs_pointer, nullptr);
	glCompileShader(fs_id);
	
	glGetShaderiv(fs_id, GL_COMPILE_STATUS, &res);
    glGetShaderiv(fs_id, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 0)
    {
        std::vector<char> fs_err_msg(info_log_len + 1);
        glGetShaderInfoLog(fs_id, info_log_len, nullptr, &fs_err_msg[0]);
        fmt::print("{}\n", &fs_err_msg[0]);
    }
    
    fmt::print("Связываем шейдеры...\n");
	GLuint pid = glCreateProgram();
	glAttachShader(pid, vs_id);
	glAttachShader(pid, fs_id);
	glLinkProgram(pid);
	
	glGetProgramiv(pid, GL_LINK_STATUS, &res);
	glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &info_log_len);
	if(info_log_len > 0)
	{
		std::vector<char> p_err_msg(info_log_len + 1);
		glGetProgramInfoLog(pid, info_log_len, nullptr, &p_err_msg[0]);
        fmt::print(stderr, "{}\n", &p_err_msg[0]);
	}
	
	glDetachShader(pid, vs_id);
	glDetachShader(pid, fs_id);
	
	glDeleteShader(vs_id);
	glDeleteShader(fs_id);

	return pid;
}
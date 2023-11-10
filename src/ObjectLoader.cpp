#include "../include/ObjectLoader.h"

#include <cstdio>
#include <fmt/core.h>

#include <fstream>

ObjectLoader::ObjectLoader(std::string path)
{
    std::fstream in(path);
    if(!in)
    {
        fmt::print(stderr, "Ошибка открытия файла модели: {}\n", path);
    }
}
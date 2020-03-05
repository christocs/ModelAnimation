#pragma once

#include <glad/glad.h>
#include <string>

auto compileShader(const std::string &path, GLuint type) -> GLuint;

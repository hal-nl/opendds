/*
 * File:   helper.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef HELPER_INCLUDED
#define HELPER_INCLUDED

#include <string>
#include <vector>
#include <map>

#include <fmt/core.h>
#include <fstream>
#include <iostream>

#include <filesystem>
#include <thread>
#include <mutex>

void        appLogger         (const std::string& typ, const std::string& msg, const std::string& threadId);
std::string GetApplicationName(const char *name);
std::string DecimalToCode64   (size_t decimal);
std::string GetContents       (const std::string& fullFileName);
int         check_directories (const std::vector<std::string>& dirlist, const std::string& threadId);

#endif /* HELPER_INCLUDED */
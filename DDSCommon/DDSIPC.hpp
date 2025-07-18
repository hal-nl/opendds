/*
 * File:   DDSIPC.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef DDSIPC_HPP
#define DDSIPC_HPP

#include <iostream>

#include <vector>
#include <map>
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool InitializePipe(const std::string& pipeName, int buffersize, HANDLE& handle);

//Pipe name format - \\<servername>\pipe\<pipename>
#define WIN_ROLTIJD         ("\\\\.\\pipe\\ROLTIJD")
#define WIN_ROLTIJDBUFSIZE  (1024)

#endif /* DDSIPC_HPP */

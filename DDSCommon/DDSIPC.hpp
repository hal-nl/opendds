/*
 * File:   DDSIPC.hpp
 * Author: haluk ates
 *
* Created on July 2025
 */

#ifndef DDSIPC_HPP
#define DDSIPC_HPP

#include <iostream>

#include <vector>
#include <map>
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;

bool InitializePipe(const std::string& pipeName, int buffersize, HANDLE& handle);

//Pipe name format - \\<servername>\pipe\<pipename>
#define WIN_ROLTIJD         ("\\\\.\\pipe\\ROLTIJD")
#define WIN_ROLTIJDBUFSIZE  (1024)

#endif /* DDSIPC_HPP */

/*
 * File:   DDSIPC.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include <vector>
#include <map>
#include <iostream>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

using namespace std;

bool InitializePipe(const std::string& pipeName, int buffersize, HANDLE& handle)
{
  handle = CreateNamedPipe(
    pipeName.c_str(),
    PIPE_ACCESS_DUPLEX,       // read/write access
    PIPE_TYPE_MESSAGE |       // message type pipe
    PIPE_READMODE_MESSAGE |   // message-read mode
    PIPE_WAIT,                // non-blocking modeParameters
    PIPE_UNLIMITED_INSTANCES, // max. instances
    buffersize,               // output buffer size
    buffersize,               // input buffer size
    NMPWAIT_USE_DEFAULT_WAIT, // client time-out
    NULL);                    // default security attribute

  return (INVALID_HANDLE_VALUE != handle);
}


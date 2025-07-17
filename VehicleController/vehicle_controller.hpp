/*
 * File:   vehicle_controller.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef VEHICLE_CONTROLLER_INCLUDED
#define VEHICLE_CONTROLLER_INCLUDED

#include <unistd.h>
#include <dlfcn.h>
#include <chrono>
#include <thread>
#include <mutex>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <libconfig.h++>
#include <fmt/core.h>
#include <filesystem>
#include <algorithm>
#include <cctype>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;
using namespace libconfig;

void appLogger(const string& typ, const string& msg, const string& threadId);
string GetApplicationName(const char *name);
string DecimalToCode64(size_t decimal);


/**
 * @brief Starts up a multi threaded application to control vehicles by
 * sending them control commands via OpenDDS. The control commands
 * can be activated by file notifications or OpenDDS subscription method
 * to the proper Vehicle topics.
 *
 * @param argc : not used yet
 * @param argv : not used yet
 * @param cfg
 * @return int
 */
int run_controller(int argc, char** argv, const Config& cfg);


#endif /* VEHICLE_CONTROLLER_INCLUDED */

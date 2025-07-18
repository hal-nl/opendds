/*
 * File:   vehicle_controller_notifiers.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef VEHICLE_CONTROLLER_NOTIFIERS_INCLUDED
#define VEHICLE_CONTROLLER_NOTIFIERS_INCLUDED

#include "vehicle_controller_run.hpp"

#define MAX_EVENTS      1024                                         /* Max. number of events to process at one go */
#define LEN_NAME        512                                          /* Assuming that the length of the filename won't exceed 512 bytes */
#define EVENT_SIZE      ( sizeof (struct inotify_event) )            /* size of one event */
#define BUF_LEN         ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))    /* buffer to store the data of events*/


int process_notifiers(int argc, char** argv, const libconfig::Setting& notifiers, std::vector<std::thread>& run_threads);

#endif /* VEHICLE_CONTROLLER_NOTIFIERS_INCLUDED */

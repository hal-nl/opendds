/*
 * File:   control_notifiers.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef CONTROL_NOTIFIERS_INCLUDED
#define CONTROL_NOTIFIERS_INCLUDED

#include "vehicle_controller_run.hpp"

/**
 * @brief Threaded notifier that looks for control files in the input_dir
 * defined in the configuration file and processes this file
 * and puts the result in output_dir with extension .log.
 *
 * @param input_dir     Input directory of files
 * @param extension     Extension of the input files
 * @param output_dir    Output directory of processed files
 * @return int
 */
int run_control_notifiers(int argc, char** argv, const std::string& input_dir, const std::string& extension, const std::string& output_dir);

#endif /* CONTROL_NOTIFIERS_INCLUDED */

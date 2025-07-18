/*
 * File:   upload_notifiers.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef UPLOAD_NOTIFIERS_INCLUDED
#define UPLOAD_NOTIFIERS_INCLUDED

#include "vehicle_controller.hpp"

/**
 * @brief Threaded notifier that looks for upload files in the input_dir
 * defined in the configuration file and processes this file
 * and puts the result in output_dir with extension .log.
 *
 * @param input_dir     Input directory of files
 * @param extension     Extension of the input files
 * @param output_dir    Output directory of processed files
 * @return int
 */
int run_upload_notifiers( int argc, char** argv, const string& input_dir, const string& extension, const string& output_dir);

#endif /* UPLOAD_NOTIFIERS_INCLUDED */

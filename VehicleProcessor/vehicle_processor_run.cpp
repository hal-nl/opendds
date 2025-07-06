/*
 * File:   vehicle_processor_run.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include "vehicle_processor.hpp"
#include "vehicle_processor_notifiers.hpp"

/**
 * @brief Run the processes from the definition in the configuration file
 * with threads.
 *
 * @param argc
 * @param argv
 * @param cfg
 * @return int
 */
int run_processor(int argc, char** argv, const Config& cfg)
{
   vector<std::thread> run_threads_notifiers;
   const Setting& root = cfg.getRoot();

   // Use hash and convert to string
   std::hash<std::thread::id> hasher;
   string mainThreadId = DecimalToCode64(hasher(std::this_thread::get_id()));

   // -------------------------------------------------------------------------
   // Start notifiers threads
   // -------------------------------------------------------------------------
   try
   {
      const Setting &notifiers = root["notifiers"];
      process_notifiers(argc, argv, notifiers, run_threads_notifiers);
   }
   catch (const SettingNotFoundException &nfex)
   {
      appLogger("error",string(nfex.what()) + ": 'notifiers'" +  " in configuration file.", mainThreadId);
   }

   // -------------------------------------------------------------------------
   // Run the threads
   // -------------------------------------------------------------------------
   for (auto &&thr : run_threads_notifiers) thr.join();

   return 0;
}

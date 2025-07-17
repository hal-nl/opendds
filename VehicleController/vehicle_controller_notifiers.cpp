/*
 * File:   vehicle_controller_notifiers.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include <sys/inotify.h>

#include "vehicle_controller.hpp"
#include "control_notifiers.hpp"

using namespace std;

/**
 * @brief Check if directories exist, else create
 *
 * @param dirlist : list of directories to check
 * @return int
 */
int check_directories(const vector<string>& dirlist, const string& threadId)
{
   // -----------------------------------------------------------------
   // Check input dir/ output dir and create if not exists
   // -----------------------------------------------------------------
   for(auto d : dirlist )
   {
      string fullPath = filesystem::path(d);
      if (!filesystem::exists(fullPath))
      {
         // Folder does not exist, create it
         if (filesystem::create_directories(fullPath))
         {
            appLogger("info ","Folder created successfully: " + fullPath, threadId);
         }
         else
         {
            appLogger("error","Failed to create folder: " + fullPath, threadId);
            return -1;
         }
      }
      else
      {
         appLogger("info ","Folder already exists: " + fullPath, threadId);
      }
   }
   return 0;
}


/**
 * @brief Run the notifiers in different threads
 *
 * @param settings
 * @param run_threads
 * @return int
 */
int process_notifiers(int argc, char** argv, const Setting& settings, vector<std::thread>& run_threads)
{
   // Use hash and convert to string
   std::hash<std::thread::id> hasher;
   string mainThreadId = DecimalToCode64(hasher(std::this_thread::get_id()));

   try
   {
      // ---------------------------------------------------------------------
      // Check if all needed directories exist, else create directories
      // before running the threads
      // ---------------------------------------------------------------------
      for (int i(0); i < settings.getLength(); ++i)
      {
         const Setting &setting = settings[i];

         string input_dir, output_dir;

         // ---------------------------------------------------------------------
         // Check service first
         // ---------------------------------------------------------------------
         string service;
         if (!setting.lookupValue("service", service) )
         {
            appLogger("error","Notifier settings 'service' does not exist.", mainThreadId);
            continue;
         }
         else
         {
            if (service == "Control")
            {
               if (
                     !(setting.lookupValue("input_dir",   input_dir)     &&
                       setting.lookupValue("output_dir",  output_dir))
                  )
               {
                  appLogger("error","Notifier settings not complete.", mainThreadId);
                  continue;
               }
               else
               {
                  if (check_directories({input_dir, output_dir}, mainThreadId))
                  {
                     appLogger("error","Some directories could not be created", mainThreadId);
                     continue;
                  }
               }
            }
            else
            {
               appLogger("error","Unknown 'service': " + service, mainThreadId);
               continue;
            }
         }
      }

      for (int i(0); i < settings.getLength(); ++i)
      {
         const Setting &setting = settings[i];

         // ---------------------------------------------------------------------
         // Check service first
         // ---------------------------------------------------------------------
         string service;
         if (!setting.lookupValue("service", service) )
         {
            appLogger("error","Notifier settings 'service' does not exist.", mainThreadId);
            continue;
         }
         else
         {
            if (service == "Control")
            {
               // ---------------------------------------------------------------------
               // Only run thread if all of the expected fields are present.
               // ---------------------------------------------------------------------
               string input_dir, extension, output_dir;
               if (!(
                     setting.lookupValue("input_dir",         input_dir)        &&
                     setting.lookupValue("extension",         extension)        &&
                     setting.lookupValue("output_dir",        output_dir)
                     )
                  )
               {
                  appLogger("error","Notifier settings not complete.", mainThreadId);
                  continue;
               }
               run_threads.emplace_back(std::thread(
                     run_control_notifiers, argc, argv, input_dir, extension, output_dir)
               );
               string threadId = DecimalToCode64( hasher(run_threads.back().get_id()) );
               appLogger("out  ","RunLogFile::Starting Notifier thread hash: " + threadId, mainThreadId);
            }
            else
            {
               appLogger("error","Unknown 'service': " + service, mainThreadId);
               continue;
            }
         }
      }
      appLogger("out  ","Running threads run_notifiers STARTED", mainThreadId);
   }
   catch (const SettingNotFoundException &nfex)
   {
      appLogger("error","Can not find notifier settings in configuration file.", mainThreadId);
   }
   return 0;
}

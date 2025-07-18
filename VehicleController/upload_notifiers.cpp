/*
 * File:   upload_notifiers.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include <sys/inotify.h>

#include "vehicle_controller.hpp"
#include "vehicle_controller_notifiers.hpp"


/**
 * @brief Threaded notifier that looks for new files in the input_dir with a specific
 * extension, defined in the configuration file and processes the events in
 * this file and puts the result in output_dir with extension html.
 *
 * @param input_dir     Input directory of files
 * @param extension     Extension of the input files
 * @param output_dir    Output directory of processed files
 * @return int
 */
int run_upload_notifiers( int argc, char** argv,
   const string& input_dir, const string& extension, const string& output_dir)
{
   int length, fd, wd, i = 0;
   char buffer[BUF_LEN];
   int retValue(0);

   // Use hash and convert to string
   std::hash<std::thread::id> hasher;
   string threadId = DecimalToCode64( hasher( std::this_thread::get_id() ) );

   /* Initialize Inotify*/
   fd = inotify_init();
   if (fd < 0)
   {
      appLogger("error","Couldn't initialize inotify", threadId);
      return -1;
   }
   /* add watch to starting directory */

   wd = inotify_add_watch(fd, input_dir.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);
   if (wd == -1)
   {
      appLogger("error","Couldn't add watch to " + input_dir, threadId);
   }
   else
   {
      appLogger("out  ","Watching files in " + input_dir + "/*" + extension + " ==> result in " + output_dir + "/*.out", threadId);
      while (1)
      {
         i = 0;
         length = read(fd, buffer, BUF_LEN);
         if (length < 0)
         {
            appLogger("error","Read error in notify", threadId);
         }
         while (i < length)
         {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
            {
               // ------------------------------------------------------------------------------
               // Do not act on CREATE: if (event->mask & IN_CREATE)
               // Do not act on DELETE: if (event->mask & IN_DELETE)
               // Act on MODIFY of files
               // ------------------------------------------------------------------------------
               if (event->mask & IN_MODIFY)
               {
                  // is file??
                  if (!(event->mask & IN_ISDIR))
                  {
                     string filename = string(event->name);

                     std::filesystem::path p(filename);
                     string ext = p.extension().string();
                     if (ext == extension)
                     {
                        string base_name = p.stem().string();
                        string infile  = input_dir  + "/" + base_name + extension;
                        string output_name = output_dir + "/" + base_name + ".out";
                        string output_content("");

                        auto start_timer = std::chrono::high_resolution_clock::now();

                        try
                        {
                           string contents = GetContents(infile);

                           appLogger("info ", "Upload data to the vehicle", threadId);
                           // TODO: Not implemented: Upload data to vehicle
                           // Etc..

                           string message = "COMPLETE THE EVENT in: ";
                           message.append(infile);
                           appLogger("info ", message, threadId);

                           if (std::remove(infile.c_str()) == 0)
                           {
                              appLogger("info ", "File deleted successfully.", threadId);
                           }
                           else
                           {
                              appLogger("error", "Deleting file unsuccesful.", threadId);
                           }
                        }
                        catch(std::exception& e)
                        {
                           appLogger("error","Unhandled Exception in 'Run': " + string(e.what()), threadId);
                        }

                        auto end_timer = std::chrono::high_resolution_clock::now();
                        auto duration = end_timer - start_timer;
                        auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

                        string ss;
                        ss.append("duration: ").append(to_string(i_millis.count())).append(" ms");
                        ss.append(", ");
                        ss.append("size: ").append(to_string(output_content.size())).append(" bytes ");
                        appLogger("out  ","Processed file " + base_name + ", " + ss, threadId);
                     }
                  }
               }
               i += EVENT_SIZE + event->len;
            }
         }
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
   }
   /* Clean up*/
   inotify_rm_watch(fd, wd);
   close(fd);

   return 0;
}


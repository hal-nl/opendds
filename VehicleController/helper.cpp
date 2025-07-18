/*
 * File:   helper.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */


#include "helper.hpp"

using namespace std;

const string version_info = "1.0.0";

/**
 * @brief Use a real logger for this. This serves as an example only
 */
void appLogger(const string& typ, const string& msg, const string& threadId)
{
   lock_guard<mutex> lock(mutex);
   string logMessage = fmt::format("[{:s}][{:s}] {:s}", typ, threadId, msg);
   cout <<  logMessage << endl;
}

/**
 * @brief Get the Application Name of the current application
 *
 * @param name
 * @return string
 */
string GetApplicationName(const char *name)
{
   lock_guard<mutex> lock(mutex);
   filesystem::path p(name);
   string application_name = p.stem().string();
   return application_name;
}

/**
 * @brief Decimal to hex code
 *
 * @param decimal
 * @return string
 */
string DecimalToCode64(size_t decimal)
{
   lock_guard<mutex> lock(mutex);
   const char *hexDigits = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$";
   string hexValue;

   do
   {
      hexValue = hexDigits[decimal % 64] + hexValue;
      decimal /= 64;
   } while (decimal > 0);

   return hexValue;
}

/**
 * @brief Get the file contents
 *
 * @param fullFileName
 * @return string
 */
string GetContents(const string &fullFileName)
{
   ifstream f(fullFileName.c_str());
   if (f)
   {
      string data;
      f.seekg(0, ios::end);
      data.resize(f.tellg());
      f.seekg(0);
      f.read(data.data(), data.size());
      return data;
   }
   return "";
}

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

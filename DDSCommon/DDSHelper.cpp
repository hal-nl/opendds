/*
 * File:   DDSHelper.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 */

#include "DDSCommon.hpp"
#include "DDSHelper.hpp"

#ifdef LINUX
#include <uuid/uuid.h> // uuid class
#endif

using namespace std;

using u32 = uint_least32_t;
using engine = std::mt19937;

namespace DDSCOMMON
{

   /**
    * Get the additional application arguments '--param1=value1 --param2=value2' from argc and argv.
    *
    * Argument may also have an empty value (" argument= ")
    *
    * @param argc        The number of application arguments
    * @param argv        The application arguments
    * @param argumentmap The map that contains the additional arguments
    *
    * @return        The number of found arguments
    */
   int GetApplicationArguments(int argc, const char **argv, map<string, string> &argumentmap)
   {
      for (int i(0); i < argc; i++)
      {
         string argstr(argv[i]);

         // argument must start with "--"
         if (argstr.find("--") == 0)
         {
            string arg(argstr.substr(2));
            vector<string> kv;
            boost::split(kv, arg, boost::is_any_of("="));
            switch (kv.size())
            {
            case 1:
               argumentmap[kv[0]] = "";
               break;
            case 2:
               argumentmap[kv[0]] = kv[1];
               break;

            default: /*do nothing*/
               break;
            }
         }
      }
      return argumentmap.size();
   }

   /*
    * Split Message into a (key, value) map: Use URL query like messages:
    * parameter delimited by '=' from value and (key, value) and pairs delimited by '&'.
    *
    */
   map<string, string> GetQueryParameters(const string &msg, const string &recordDelim, const string &keyDelim)
   {
      map<string, string> kvmap;
      if (msg.size())
      {
         vector<string> kvrecords;
         boost::split(kvrecords, msg, boost::is_any_of(recordDelim) /*,boost::token_compress_on*/);

         if (kvrecords.size())
         {
            for (size_t k(0); k < kvrecords.size(); k++)
            {
               vector<string> kv;
               boost::split(kv, kvrecords[k], boost::is_any_of(keyDelim) /*,boost::token_compress_on*/);
               if (kv.size() == 2)
               {
                  kvmap[kv[0]] = kv[1];
               }
            }
         }
      }
      return kvmap;
   }

   /**
    * ------------------------------------------------------------------------------------
    * The Time2* functions use the BOOST LIBRARY
    * ------------------------------------------------------------------------------------
    */
   namespace pt = boost::posix_time;

   string Time2IsoString         (void) { return pt::to_iso_string(pt::microsec_clock::local_time()); }
   string Time2ExtendedIsoString (void) { return pt::to_iso_extended_string(pt::microsec_clock::local_time()); }
   string Time2SimpleString      (void) { return pt::to_simple_string(pt::microsec_clock::local_time()); }

   long long GetMicroseconds(std::tm &timeinfo, bool utc = false)
   {
      std::lock_guard<std::mutex> lock(std::mutex);

      // Get current time and microseconds
      auto now = system_clock::now();
      auto us = duration_cast<microseconds>(now.time_since_epoch()) % 1000000;

      // convert to std::time_t in order to convert to std::tm (broken time)
      auto timer = system_clock::to_time_t(now);

      // Convert to UTC or not
      timeinfo = (utc) ? *std::gmtime(&timer): *std::localtime(&timer);

      return us.count();
   }

   string Time2IsoStringHMS(bool fraction)
   {
      std::lock_guard<std::mutex> lock(std::mutex);
      std::tm t;
      long long micro = GetMicroseconds(t, false); // Get local time

      string hms = fmt::format("{:02d}{:02d}{:02d}", t.tm_hour, t.tm_min, t.tm_sec);
      if (fraction)
      {
         hms.append(fmt::format(".{:06d}", micro));
      }
      return hms;
   }
      /**
    * ------------------------------------------------------------------------------------
    * Create a unique random ID
    *
    * Format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    * ------------------------------------------------------------------------------------
    */
   string CreateUUID(void)
   {
      boost::uuids::random_generator gen;
      boost::uuids::uuid uuid = gen(); // initialize uuid

      const std::string s = boost::uuids::to_string(uuid);
      return s;
   }

   /**
    * Get the domain id from the name (usually from XML name;
    *
    * Domains = [REAL, OBSERVED, TEST]
    */
   int GetDomainId(const string &domainname)
   {
      if (domainname == "REAL")
      {
         return DDSCOMMON::CONSTANTS::DOMAINS::REAL;
      }
      else if (domainname == "OBSERVED")
      {
         return DDSCOMMON::CONSTANTS::DOMAINS::OBSERVED;
      }
      else if (domainname == "TEST")
      {
         return DDSCOMMON::CONSTANTS::DOMAINS::TEST;
      }
      else if (domainname == "CONTROL")
      {
         return DDSCOMMON::CONSTANTS::DOMAINS::CONTROL;
      }
      // If unknown: always in domain real
      return DDSCOMMON::CONSTANTS::DOMAINS::REAL;
   }

   /**
    * Get random value between start and end
    */
   int GetRandomValue(int start_value, int end_value)
   {
      std::random_device os_seed;

      const u32 seed = os_seed();

      engine generator(seed);
      std::uniform_int_distribution<u32> distribute(start_value, end_value);

      return distribute(generator);
   }

   double GetRandomValue(double start_value, double end_value)
   {
      std::random_device os_seed;

      const u32 seed = os_seed();

      engine generator(seed);

      std::uniform_real_distribution<double> distribute(start_value, end_value);

      return distribute(generator);
   }

   std::string GetRandomString(unsigned int length, int start_value, int end_value)
   {
      std::random_device os_seed;
      const u32 seed = os_seed();
      engine generator(seed);

      vector<char> char_vector;
      for (int l(0); l < length; ++l)
      {
         std::uniform_int_distribution<int> distribute(start_value, end_value);
         char val = static_cast<char>(distribute(generator));
         char_vector.push_back(val);
      }
      char_vector.push_back(0);
      string ret( char_vector.data());

      return ret;
   }


   std::string GetRandomString(unsigned int length)
   {
      return GetRandomString(length, 32, 126);
   }

   std::string GetRandomStringUpper(unsigned int length)
   {
      return GetRandomString(length, 65, 90);
   }

   std::string GetRandomStringLower(unsigned int length)
   {
      return GetRandomString(length, 97, 122);
   }

   std::string GetRandomStringNumbers(unsigned int length)
   {
      return GetRandomString(length, 48, 57);
   }

   /**
    * Read data from file
    */
   string FileGetContents(const string &fullFileName)
   {
      string contents;
      ifstream input(fullFileName.c_str(), ios::in | ios::binary);

      if (input)
      {
         input.seekg(0, ios::end);
         contents.resize(input.tellg());
         input.seekg(0, ios::beg);
         input.read(&contents[0], contents.size());
         input.close();
      }
      else
      {
#ifdef _DEBUG
         cout << "Error opening file: " << fullFileName << "\n";
#endif
      }
      return (contents);
   }

   /**
    * Write data to file
    */
   void FilePutContents(const string &fullFileName, const string &data)
   {
      ofstream outfile;

      outfile.open(fullFileName.c_str());
      outfile.write(data.c_str(), data.size());
      outfile.close();
   }

   /**
    * Append data to file
    */
   void FileAddContents(const string &fullFileName, const string &data)
   {
      ofstream outfile;

      outfile.open(fullFileName.c_str(), ios::app);
      outfile << data;
      outfile.close();
   }

};

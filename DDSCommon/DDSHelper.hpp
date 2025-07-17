/*
 * File:   DDSHelper.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 */

#ifndef DDSHELPER_HPP
#define DDSHELPER_HPP

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <boost/date_time/posix_time/posix_time.hpp> // Get current date/time in milliseconds.

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include "ObjectVisibility.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <map>
#include <iostream>

#include <cctype>
#include <random>
#include <chrono>
#include <mutex>
#include <fmt/core.h>

using namespace std;
using namespace std::chrono;

namespace DDSCOMMON
{

   /**
    * Get application arguments '--param1=value1 --param2=value2' from argc and argv.
    *
    * Arguments should start with double hyphen: --
    * Argument may also have an empty value (" --argument= ")
    *
    * @param argc          The number of application arguments
    * @param argv          The application arguments
    * @param argumentmap   The map<string, string> that contains the arguments
    *
    * @return            The number of correct arguments
    */
   int GetApplicationArguments(int argc, const char **argv, map<string, string> &argumentmap);

   /**
    * Split Message into a (key, value) map: Use URL query like messages:
    * parameter delimited by '=' from value and (key, value) and pairs delimited by '&'.
    *
    * @param msg           String to be split
    * @param recordDelim   Delimiter for key value pairs
    * @param keyDelim      Delimiter between key and value
    * @return
    */
   map<string, string> GetQueryParameters(const string &msg, const string &recordDelim = "&", const string &keyDelim = "=");

   /*
    * ------------------------------------------------------------------------------------
    * The Time2* functions use the BOOST LIBRARY
    * ------------------------------------------------------------------------------------
    */

   /**
    * Return the current date time and microseconds as ISO string.
    *
    * Format "YYYYMMDDTHHmmss.tttttt": YearMonthDayTHoursMinutesSeconds.Microseconds
    *
    * i.e Is used in Publication time
    */
   string Time2IsoString(void);

   /**
    * Return the current date time and microseconds as extended ISO string.
    *
    * Format "YYYY-MM-DDTHH:mm:ss.tttttt"
    */
   string Time2ExtendedIsoString(void);

   /**
    * Return the current date time and microseconds as simple string.
    *
    * Format "YYYY-MMM-DD HH:mm:ss.tttttt"
    */
   string Time2SimpleString(void);

   /**
    * Return the current time only: Format "HH:mm:ss<.ffffff>"
    * If fraction then microsecond format appended: .ffffff
    */
   string Time2IsoStringHMS(bool fraction = false);

   /**
    * Create a unique random ID
    *
    * Format "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
    */
   string CreateUUID(void);

   /**
    * Get the domain id from the name (usually from XML name;
    *
    * Domains = [REAL, OBSERVED, TEST]
    */
   int GetDomainId(const string &domainname);

   /**
    * Get random value between start and end
    *
    */
   int         GetRandomValue(int start_value, int end_value);
   double      GetRandomValue(double start_value, double end_value);

   std::string GetRandomString         (unsigned int length);
   std::string GetRandomStringUpper    (unsigned int length);
   std::string GetRandomStringLower    (unsigned int length);
   std::string GetRandomStringNumbers  (unsigned int length);

   string FileGetContents(const string &fullFileName);
   void FilePutContents(const string &fullFileName, const string &data);
   void FileAddContents(const string &fullFileName, const string &data);

};

#endif /* DDSHELPER_HPP */

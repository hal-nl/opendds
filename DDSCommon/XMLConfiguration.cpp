/*
 * File:   XMLConfiguration.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */
#include "ObjectVisibility.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

using namespace std;

// Short alias namespace for the boost property tree.
namespace NSPT = boost::property_tree;

#include "XMLConfiguration.hpp"

#define XMLLOAD_OK (0)
#define XMLLOAD_ERROR (-1)

int DLL_LOCAL XMLLoadFromFile(const std::string &configurationfile, NSPT::ptree &_root)
{
   // If file name exists then try to read from file:
   if (!configurationfile.empty())
   {
      try
      {
         // Load the XML file in this ptree
         NSPT::read_xml(configurationfile, _root);
      }
      catch (const NSPT::ptree_error &e)
      {
         cerr << "DDSCOMMON Configuration FILE initialize error: " << e.what() << endl;
         return XMLLOAD_ERROR;
      }
   }
   else
   {
      cerr << "DDSCOMMON Configuration FILE empty" << endl;
      return XMLLOAD_ERROR;
   }
   return XMLLOAD_OK;
}

int DLL_LOCAL XMLLoadFromString(const std::string &configurationdata, NSPT::ptree &_root)
{
   if (!configurationdata.empty())
   {
      try
      {
         // Load the XML data in this ptree
         NSPT::read_xml(configurationdata, _root);
      }
      catch (const NSPT::ptree_error &e)
      {
         cerr << "DDSCOMMON Configuration STRING initialize error: " << e.what() << endl;
         return XMLLOAD_ERROR;
      }
   }
   else
   {
      cout << "DDSCOMMON Configuration STRING empty" << endl;
      return XMLLOAD_ERROR;
   }
   return XMLLOAD_OK;
}

int DLL_LOCAL readArguments(vector<string> &vArgs, const char *dds_str, const char *ddscommon_str, NSPT::ptree &root)
{
   int argc(0);
   map<string, string> openddsArgs;
   map<string, string> ddscommonArgs;

   try
   {
      // Read OpenDDS arguments
      BOOST_FOREACH (NSPT::ptree::value_type &v, root.get_child(dds_str))
      {
         openddsArgs[v.first] = v.second.data();
      }
      // Read ddscommon arguments
      BOOST_FOREACH (NSPT::ptree::value_type &v, root.get_child(ddscommon_str))
      {
         ddscommonArgs[v.first] = v.second.data();
      }
   }
   catch (...)
   {
      cerr << "Exception reading XML arguments: \n'"
           << dds_str << "' and/or '" << ddscommon_str << "' in xml are not root path element(s).\n";
   }

   argc = static_cast<int>((2 * openddsArgs.size()) + ddscommonArgs.size());
   if (argc > 0)
   {
      // -------------------------------------------------
      // Create OpenDDS arguments
      // -------------------------------------------------
      map<string, string>::const_iterator ddsItr(openddsArgs.begin());
      while (ddsItr != openddsArgs.end())
      {
         vArgs.push_back(string("-") + ddsItr->first);
         vArgs.push_back(ddsItr->second);
         ddsItr++;
      }

      // -------------------------------------------------
      // Create DDSCOMMON arguments
      // -------------------------------------------------
      map<string, string>::const_iterator ddscommonItr(ddscommonArgs.begin());
      while (ddscommonItr != ddscommonArgs.end())
      {
         vArgs.push_back(string("--") + ddscommonItr->first + string("=") + ddscommonItr->second);
         ddscommonItr++;
      }
   }
   return argc;
}

/**
 * Access the value of the configuration key as integer
 * You can also go through nested nodes "some.complex.path" by using dots.
 */
extern "C" int XMLReadInteger(const char *xmlconfname, const char *key, int defaultValue)
{
   NSPT::ptree _root;

   if (XMLLOAD_OK == XMLLoadFromFile(xmlconfname, _root))
   {
      int value = _root.get<int>(key, defaultValue);
      ;
      return value;
   }
   return defaultValue;
}

/**
 * Access the value of the configuration key as integer
 * You can also go through nested nodes "some.complex.path" by using dots.
 */
extern "C" long XMLReadLong(const char *xmlconfname, const char *key, long defaultValue)
{
   NSPT::ptree _root;

   if (XMLLOAD_OK == XMLLoadFromFile(xmlconfname, _root))
   {
      long value = _root.get<long>(key, defaultValue);
      return value;
   }
   return defaultValue;
}

/**
 * Access the value of the configuration key as boolean
 * You can also go through nested nodes "some.complex.path" by using dots.
 */
extern "C" bool XMLReadBool(const char *xmlconfname, const char *key, bool defaultValue)
{
   NSPT::ptree _root;

   if (XMLLOAD_OK == XMLLoadFromFile(xmlconfname, _root))
   {
      bool value = _root.get<bool>(key, defaultValue);
      return value;
   }
   return defaultValue;
}

/**
 * Access the value of the configuration key as std::string.
 * You can also go through nested nodes "some.complex.path" by using dots
 * After the call you need to free memory allocated with _strdup.
 */
extern "C" char *XMLReadString(const char *xmlconfname, const char *key, const char *defaultValue)
{
   NSPT::ptree _root;

   if (XMLLOAD_OK == XMLLoadFromFile(xmlconfname, _root))
   {
      std::string value = _root.get<std::string>(key, defaultValue);
#ifdef LINUX
      return strdup((char *)value.c_str());
#else
      return _strdup((char *)value.c_str());
#endif
   }
   return (char *)defaultValue;
}

extern "C" char *XMLReadElementFromString(const char *xmlstring, const char *key)
{
   NSPT::ptree root;
   string value("");

   if (XMLLOAD_OK == XMLLoadFromString(xmlstring, root))
   {
      value = root.get<std::string>(key, "");
   }
#ifdef LINUX
   return strdup((char *)value.c_str());
#else
   return _strdup((char *)value.c_str());
#endif
}

extern "C" char *XMLReadElementFromFile(const char *xmlconfname, const char *key)
{
   NSPT::ptree root;
   string value("");

   if (XMLLOAD_OK == XMLLoadFromFile(xmlconfname, root))
   {
      value = root.get<std::string>(key, "");
   }
#ifdef LINUX
   return strdup((char *)value.c_str());
#else
   return _strdup((char *)value.c_str());
#endif
}

extern "C" char *XMLReadAttributeFromString(const char *xmlstring, const char *key, const char *attribute)
{
   NSPT::ptree root;
   string fullkey("");
   string value("");

   if (XMLLOAD_OK == XMLLoadFromString(xmlstring, root))
   {
      fullkey = string(key) + string(".<xmlattr>.") + string(attribute);
      value = root.get<std::string>(fullkey, "");
   }
#ifdef LINUX
   return strdup((char *)value.c_str());
#else
   return _strdup((char *)value.c_str());
#endif
}

extern "C" char *XMLReadAttributeFromFile(const char *xmlconfname, const char *key, const char *attribute)
{
   NSPT::ptree root;
   string fullkey("");
   string value("");

   if (XMLLOAD_OK == XMLLoadFromFile(xmlconfname, root))
   {
      fullkey = string(key) + string(".<xmlattr>.") + string(attribute);
      value = root.get<std::string>(fullkey, "");
   }
#ifdef LINUX
   return strdup((char *)value.c_str());
#else
   return _strdup((char *)value.c_str());
#endif
}

extern "C" int XMLReadChildElementsFromString(const string &xmlString,
                                              const string &node, map<string, string> &keyvalue)
{
   NSPT::ptree root;
   if (XMLLOAD_OK == XMLLoadFromString(xmlString, root))
   {
      try
      {
         BOOST_FOREACH (NSPT::ptree::value_type &v, root.get_child(node))
         {
            keyvalue[v.first] = v.second.data();
         }
      }
      catch (...)
      {
         cerr << "Exception reading XML elements from string: \n'"
              << node << " in xml is not a path element.\n";
      }
   }
   return static_cast<int>(keyvalue.size());
}

extern "C" int XMLReadChildElementsFromFile(const string &xmlFileName,
                                            const string &node, map<string, string> &keyvalue)
{
   NSPT::ptree root;
   if (XMLLOAD_OK == XMLLoadFromFile(xmlFileName, root))
   {
      try
      {
         BOOST_FOREACH (NSPT::ptree::value_type &v, root.get_child(node))
         {
            keyvalue[v.first] = v.second.data();
         }
      }
      catch (...)
      {
         cerr << "Exception reading XML elements from file: \n'"
              << node << " in xml is not a path element.\n";
      }
   }
   return static_cast<int>(keyvalue.size());
}

extern "C" int XMLReadChildAttributesFromString(const string &xmlString,
                                                const string &node, const string &child, const string &attribute, vector<string> &attributeValues)
{
   NSPT::ptree root;
   if (XMLLOAD_OK == XMLLoadFromString(xmlString, root))
   {
      try
      {
         BOOST_FOREACH (NSPT::ptree::value_type &v, root.get_child(node))
         {
            if (v.first == child)
            {
               string key = string("<xmlattr>.") + attribute;
               string val = v.second.get<std::string>(key, "");
               attributeValues.push_back(val);
            }
         }
      }
      catch (...)
      {
         cerr << "Exception reading XML child attributes from string: \n'"
              << node << " in xml is not a path element.\n";
      }
   }
   return static_cast<int>(attributeValues.size());
}

extern "C" int XMLReadChildElementListFromString(const string &xmlString,
                                                 const string &node, const string &child, const string &element, vector<string> &elementValues)
{
   NSPT::ptree root;
   if (XMLLOAD_OK == XMLLoadFromString(xmlString, root))
   {
      try
      {
         BOOST_FOREACH (NSPT::ptree::value_type &v, root.get_child(node))
         {
            if (v.first == child)
            {
               string val = v.second.get<std::string>(element, "");
               elementValues.push_back(val);
            }
         }
      }
      catch (...)
      {
         cerr << "Exception reading XML child attributes from string: \n'"
              << node << " in xml is not a path element.\n";
      }
   }
   return static_cast<int>(elementValues.size());
}

int CreateOpenDDSArgumentsFromMap(const map<string, string> &keyvalue, vector<string> &arguments)
{
   int argc = static_cast<int>(2 * keyvalue.size());
   if (argc > 0)
   {
      map<string, string>::const_iterator mItr(keyvalue.begin());
      while (mItr != keyvalue.end())
      {
         arguments.push_back(string("-") + mItr->first);
         arguments.push_back(mItr->second);
         mItr++;
      }
   }
   return argc;
}

int CreateDDSCommonArgumentsFromMap(const map<string, string> &keyvalue, vector<string> &arguments)
{
   int argc = static_cast<int>(2 * keyvalue.size());
   if (argc > 0)
   {
      map<string, string>::const_iterator mItr(keyvalue.begin());
      while (mItr != keyvalue.end())
      {
         arguments.push_back(string("--") + mItr->first + string("=") + mItr->second);
         mItr++;
      }
   }
   return argc;
}

extern "C" int GetParameters(const string &confXml, const string &ddscommon_node, const string &dds_node,
                             vector<string> &allArguments)
{
   map<string, string> ddsKeyValues;
   map<string, string> ddscommonKeyValues;

   int count = XMLReadChildElementsFromFile(confXml, dds_node, ddsKeyValues);
   if (count > 0)
      CreateOpenDDSArgumentsFromMap(ddsKeyValues, allArguments);
   count = XMLReadChildElementsFromFile(confXml, ddscommon_node, ddscommonKeyValues);
   if (count > 0)
      CreateDDSCommonArgumentsFromMap(ddscommonKeyValues, allArguments);

   return static_cast<int>(allArguments.size());
}

extern "C" int GetParametersFromXMLString(const string &xml, const string &ddscommon_node, const string &dds_node,
                                          vector<string> &allArguments)
{
   map<string, string> ddsKeyValues;
   map<string, string> ddscommonKeyValues;

   int count = XMLReadChildElementsFromString(xml, dds_node, ddsKeyValues);
   if (count > 0)
      CreateOpenDDSArgumentsFromMap(ddsKeyValues, allArguments);
   count = XMLReadChildElementsFromString(xml, ddscommon_node, ddscommonKeyValues);
   if (count > 0)
      CreateDDSCommonArgumentsFromMap(ddscommonKeyValues, allArguments);

   return static_cast<int>(allArguments.size());
}

// extern "C" bool DDSCOMMONFreeParameters(char **ARGV, int nrOfParameters)
//{
//   try {
//     ARGC = allArguments.size();
//     ARGV = new char*[allArguments.size()];
//     for (size_t i(0); i < allArguments.size(); i++) {
//       ARGV[i] = new char;
// #ifdef LINUX
//       ARGV[i] = strdup(allArguments[i].c_str());
// #else
//       ARGV[i] = _strdup(allArguments[i].c_str());
// #endif
//     }
//   }
//   catch (...) {
//     /* Something went wrong */
//     return 0;
//   }
//
//
//
//   try {
//     for (int i(0); i < nrOfParameters; i++) delete ARGV[i];
//     delete[] ARGV;
//   }
//   catch (...) {
//     return false;
//   }
//   return true;
// }

/*
 * File:   DDSConfiguration.cpp
 *
* Created on July 2025
 *
 */

#include <cstdlib>
#include <iostream>
#include <charconv>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ace/Log_Msg.h>

#include <dds/DdsDcpsCoreC.h>
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

using namespace std;

#include "DDSConfiguration.hpp"

DDSConfiguration::DDSConfiguration()
  : _valid(false), _configurationfile(""), _configurationdata("")
{
}

#if defined(LINUX)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
DDSConfiguration::DDSConfiguration(const DDSConfiguration& orig)
{
}

DDSConfiguration::~DDSConfiguration()
{
}


int DDSConfiguration::XMLLoadFromFile(const std::string& configurationfile)
{
  // If file name exists then try to read from file:
  if (!configurationfile.empty()) {
    try {
      // Load the XML file in this ptree
      NSPT::read_xml(configurationfile, _root);
      _valid = true;
    } catch(const NSPT::ptree_error& e) {
      _valid= false;
      cerr << "DDSConfiguration FILE initialize error: " << e.what() << endl;
    }
  } else {
    _valid= false;
    cerr << "DDSConfiguration FILE empty: " << configurationfile << endl;
  }
  return 0;
}


int DDSConfiguration::XMLLoadFromString (const std::string& configurationdata)
{
   if (!configurationdata.empty())
   {
      try
      {
         // Load the XML data in this ptree
         string ss;
         ss.append(configurationdata);
         NSPT::read_xml(ss, _root);
         _valid = true;
      }
      catch (const NSPT::ptree_error &e)
      {
         _valid = false;
         cerr << "DDSConfiguration STRING initialize error: " << e.what() << endl;
      }
   }
   else
   {
      _valid = false;
      cout << "DDSConfiguration STRING empty" << endl;
   }
   return 0;
}

/**
 * Access the value of the configuration key as integer
 * You can also go through nested nodes "some.complex.path" by using dots.
 */
int DDSConfiguration::XMLReadInteger (const std::string& key, int defaultValue)
{
  if (_valid) {
    //long longvalue = _root.get<long>(key, defaultValue);
    //int value = static_cast<int>(longvalue);
    int value = _root.get<int>(key, defaultValue);;
    return value;
  }
  return defaultValue;
}

/**
 * Access the value of the configuration key as integer
 * You can also go through nested nodes "some.complex.path" by using dots.
 */
long DDSConfiguration::XMLReadLong (const std::string& key, long defaultValue)
{
  if (_valid) {
    long value = _root.get<long>(key, defaultValue);
    return value;
  }
  return defaultValue;
}


/**
 * Access the value of the configuration key as boolean
 * You can also go through nested nodes "some.complex.path" by using dots.
 */
bool DDSConfiguration::XMLReadBool(const std::string& key, bool defaultValue)
{
  if (_valid) {
    bool value = _root.get<bool>(key, defaultValue);
    return value;
  }
  return defaultValue;
}


/**
 * Access the value of the configuration key as std::string.
 * You can also go through nested nodes "some.complex.path" by using dots
 */
std::string DDSConfiguration::XMLReadString  (const std::string& key, const std::string& defaultValue)
{
  if (_valid) {
    std::string value = _root.get<std::string>(key, defaultValue);
    return value;
  }
  return defaultValue;
}

/**
 * Access the value of the attribute as std::string.
 * You can also go through nested nodes "some.complex.path" by using dots
 */
std::string DDSConfiguration::XMLReadAttribute(const std::string& key, const std::string& attribute)
{
  if (_valid) {
    std::string fullkey(key + ".<xmlattr>."+ attribute);
    std::string value = _root.get<std::string>(fullkey);
    return value;
  }
  return "";
}

std::vector<std::string> DDSConfiguration::XMLReadElements(const std::string& itemlist)
{
  std::vector<std::string> list;
  if (_valid) {
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, _root.get_child(itemlist + "."))
    {
      // v.first is the name of the child.
      // v.second is the child tree.
      boost::property_tree::ptree child = v.second;
      std::string value = child.get<std::string>("");
      //cout << value << endl << endl;
      list.push_back(value);
    }
  }
  return list;
}

/**
 * Setting the octet of the user_data
 *
 * @param octet
 * @param octValue
 */
void DDSConfiguration::fillFromOctetSequence(const std::string& octet, DDS::OctetSeq& octValue)
{
   if ((octet.size() > 0)                                                           // Must not be empty
       && ((octet.size() % 2) == 0)                                                 // Length: Multiple of 2
       && (octet.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos)) // Contains HEX values
   {
      int len(octet.size() / 2);
      octValue.length(len);
      for (int i(0); i < len; i++)
      {
         string hexStr = octet.substr(i * 2, 2);
         int value = 0;
         std::from_chars(hexStr.data(), hexStr.data() + hexStr.size(), value, 16);
         octValue[i * 2] = value;
      }
   }
}

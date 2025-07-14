/*
 * File:   XMLConfiguration.hpp
 *
 * See also <boost/property_tree/xml_parser.hpp> for examples.
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef XMLCONFIGURATION_HPP
#define XMLCONFIGURATION_HPP

#include "ObjectVisibility.hpp"

#ifdef LINUX
#include <string.h>
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * Methods to read individual configuration setings and attributes from an xml file or xml string.
 *
 */
extern "C" int XMLReadInteger(const char *xmlconfname, const char *key, int defaultValue = 0);
extern "C" long XMLReadLong(const char *xmlconfname, const char *key, long defaultValue = 0L);
extern "C" bool XMLReadBool(const char *xmlconfname, const char *key, bool defaultValue = false);
extern "C" char *XMLReadString(const char *xmlconfname, const char *key, const char *defaultValue = "");

extern "C" char *XMLReadElementFromString(const char *xmlstring, const char *key);
extern "C" char *XMLReadElementFromFile(const char *xmlconfname, const char *key);

extern "C" char *XMLReadAttributeFromString(const char *xmlstring, const char *key, const char *attribute);
extern "C" char *XMLReadAttributeFromFile(const char *xmlconfname, const char *key, const char *attribute);

extern "C" int XMLReadChildElementsFromString(const string &xmlString,
                                              const string &node, map<string, string> &keyvalue);
extern "C" int XMLReadChildElementsFromFile(const string &xmlFileName,
                                            const string &node, map<string, string> &keyvalue);

extern "C" int XMLReadChildAttributesFromString(const string &xmlString,
                                                const string &node, const string &child, const string &attribute, vector<string> &attributeValues);
extern "C" int XMLReadChildElementListFromString(const string &xmlString,
                                                 const string &node, const string &child, const string &element, vector<string> &elementValues);

/**
 * Methods to get/free OpenDDS or DDSCOMMON command line arguments from xml configuration.
 */
extern "C" int GetParameters(const string &confXml, const string &ddscommon_node, const string &dds_node, vector<string> &allArguments);

extern "C" int GetParametersFromXMLString(const string &xml, const string &ddscommon_node, const string &dds_node,
                                          vector<string> &allArguments);

#endif /* XMLCONFIGURATION_HPP */

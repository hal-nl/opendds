/*
 * File:   ddsqos.hpp
 *
 * See also <boost/property_tree/xml_parser.hpp> for examples.
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef DDSQOS_HPP
#define DDSQOS_HPP

#include <cstdlib>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <ace/Log_Msg.h>

#include <dds/DdsDcpsCoreC.h>
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

using namespace std;

// Short alias namespace for the boost property tree.
namespace NSPT = boost::property_tree;

/**
 * Class used to read configuration data from XML based configuration files.
 */
class DDSQOS {
public:
  DDSQOS();
  /**
   * Copy constructor
   * @param orig
   */
  DDSQOS(const DDSQOS& orig);
  virtual ~DDSQOS();

  /**
   * Read the XML configuration settings from file or string
   *
   * @param configurationfile
   * @return
   */
  int XMLLoadFromFile(const std::string& configurationfile);

  /**
   * Read the XML configuration settings from string
   *
   * @param configurationdata
   * @return
   */
  int XMLLoadFromString(const std::string& configurationdata);

  /**
   * Methods to read individual configuration setings from an xml files.
   */
  int         XMLReadInteger  (const std::string& key, int defaultValue = 0);
  long        XMLReadLong     (const std::string& key, long defaultValue = 0L);
  bool        XMLReadBool     (const std::string& key, bool defaultValue = false);
  std::string XMLReadString   (const std::string& key, const std::string& defaultValue = "");

  /**
   * OpenDDS Qos methods to read from an xml configuration file and set the QoS settings of the OpenDDS entities.
   */
  int SetParticipantQos  (DDS::DomainParticipantQos& participant_qos);
  int SetTopicQos        (DDS::TopicQos& topic_qos);
  int SetPublisherQos    (DDS::PublisherQos& pub_qos);
  int SetSubscriberQos   (DDS::SubscriberQos& sub_qos);
  int SetDatawriterQos   (DDS::DataWriterQos& dw_qos);
  int SetDatareaderQos   (DDS::DataReaderQos& dr_qos);

private:
  bool          _valid;
  std::string   _configurationfile;
  std::string   _configurationdata;
  NSPT::ptree   _root;

  void fillFromOctetSequence(const std::string& octet, DDS::OctetSeq& octValue);

};

#endif /* DDSQOS_HPP */


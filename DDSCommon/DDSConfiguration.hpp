/*
 * File:   DDSConfiguration.hpp
 *
 * See also <boost/property_tree/xml_parser.hpp> for examples.
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 */

#ifndef DDSCONFIGURATION_HPP
#define DDSCONFIGURATION_HPP

#include <cstdlib>
#include <iostream>
#include <map>

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

// Short alias namespace for the boost property tree.
namespace NSPT = boost::property_tree;

/**
 * Class used in DDS to read configuration data from XML based configuration files.
 */
class DDSConfiguration {
public:
  DDSConfiguration();
  /**
   * Copy constructor
   * @param orig
   */
  DDSConfiguration(const DDSConfiguration& orig);
  virtual ~DDSConfiguration();

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
  std::string XMLReadAttribute(const std::string& key, const std::string& attribute);

  std::vector<std::string> XMLReadElements(const std::string& itemlist);

  /**
   * OpenDDS Qos methods to read from an xml configuration file and set the QoS settings of the OpenDDS entities.
   */
  int SetDatareaderQosFromXML   (DDS::DataReaderQos& dr_qos);
  int SetDatawriterQosFromXML   (DDS::DataWriterQos& dw_qos);
  int SetParticipantQosFromXML  (DDS::DomainParticipantQos& participant_qos);
  int SetPublisherQosFromXML    (DDS::PublisherQos& pub_qos);
  int SetSubscriberQosFromXML   (DDS::SubscriberQos& sub_qos);
  int SetTopicQosFromXML        (DDS::TopicQos& topic_qos);

  std::string ShowValuesDatareaderQos   (const DDS::DataReaderQos& dr_qos);
  std::string ShowValuesDatawriterQos   (const DDS::DataWriterQos& dw_qos);
  std::string ShowValuesParticipantQos  (const DDS::DomainParticipantQos& participant_qos);
  std::string ShowValuesPublisherQos    (const DDS::PublisherQos& pub_qos);
  std::string ShowValuesSubscriberQos   (const DDS::SubscriberQos& sub_qos);
  std::string ShowValuesTopicQos        (const DDS::TopicQos& topic_qos);

private:
  bool          _valid;
  std::string   _configurationfile;
  std::string   _configurationdata;
  NSPT::ptree   _root;

  void fillFromOctetSequence(const std::string& octet, DDS::OctetSeq& octValue);

};

#endif /* DDSCONFIGURATION_HPP */

/*
 * File:   QoSConfigurationParticipant.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include <cstdlib>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <ace/Log_Msg.h>

#include <dds/DdsDcpsCoreC.h>
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>
//#include <dds/DdsDcpsCoreTypeSupportC.h>
//#include <dds/DdsDcpsCoreTypeSupportImpl.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

using namespace std;

#include "DDSConfiguration.hpp"

/**
 * Set the values of participant QoS from XML configuration
 *
 * @param participant_qos
 * @return
 */
int DDSConfiguration::SetParticipantQosFromXML(DDS::DomainParticipantQos& participant_qos)
{
  if (_valid) {
    string element("");

    element = "qos.participant.user_data.value";
    if (!XMLReadString(element, "").empty() ) {
      string octet = _root.get<string>(element);
      fillFromOctetSequence(octet, participant_qos.user_data.value);
    }
    element = "qos.participant.entity_factory.autoenable_created_entities";
    if (!XMLReadString(element, "").empty() ) {
      participant_qos.entity_factory.autoenable_created_entities = _root.get<bool>(element);
    }
  }
  return 0;
}

/**
 * Initially for debug purposes only
 *
 * @param participant_qos
 * @return
 */
std::string DDSConfiguration::ShowValuesParticipantQos(const DDS::DomainParticipantQos& participant_qos)
{
   if (_valid)
   {
      string ss;
      ss.append("DOMAIN PARTICIPANT QOS:\n");

      // Policy Values:
      // ss << "user_data.value: " << participant_qos.user_data.value << "\n";
      ss.append("\tentity_factory.autoenable_created_entities: ").append(to_string(participant_qos.entity_factory.autoenable_created_entities)).append("\n");
      return ss;
   }
   return "";
}

/*
 * File:   ddsqosParticipant.cpp
 * Author: Haluk Ates
 *
* Created on July 2025
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

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

using namespace std;

#include "ddsqos.hpp"

/**
 * Set the values of participant QoS from XML configuration
 *
 * @param participant_qos
 * @return
 */
int DDSQOS::SetParticipantQos(DDS::DomainParticipantQos& participant_qos)
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



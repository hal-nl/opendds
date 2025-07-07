/*
 * File:   QoSConfigurationPublisher.cpp
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
//#include <dds/DdsDcpsCoreTypeSupportC.h>
//#include <dds/DdsDcpsCoreTypeSupportImpl.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

using namespace std;

#include "DDSConfiguration.hpp"

/**
 * Set the values of publisher QoS from XML configuration
 *
 * @param pub_qos
 * @return
 */
int DDSConfiguration::SetPublisherQosFromXML(DDS::PublisherQos& pub_qos)
{
  if (_valid) {
    string element("");

    element = "qos.publisher.presentation.access_scope";
    if (!XMLReadString(element, "").empty() ) {
      string access_scope = _root.get<string>(element);

      if (access_scope == "GROUP_PRESENTATION_QOS")       pub_qos.presentation.access_scope = DDS::GROUP_PRESENTATION_QOS;
      if (access_scope == "INSTANCE_PRESENTATION_QOS")    pub_qos.presentation.access_scope = DDS::INSTANCE_PRESENTATION_QOS;
      if (access_scope == "TOPIC_PRESENTATION_QOS")  pub_qos.presentation.access_scope = DDS::TOPIC_PRESENTATION_QOS;
    }
    element = "qos.publisher.presentation.coherent_access";
    if (!XMLReadString(element, "").empty() ) {
      pub_qos.presentation.coherent_access = _root.get<bool>(element);
    }
    element = "qos.publisher.presentation.ordered_access";
    if (!XMLReadString(element, "").empty() ) {
      pub_qos.presentation.ordered_access = _root.get<bool>(element);
    }

    /*
     * NOT USED YET:
      element = "qos.publisher.partition.name";
    */

    element = "qos.publisher.group_data.value";
    if (!XMLReadString(element, "").empty() ) {
      string octet = _root.get<string>(element);
      fillFromOctetSequence(octet, pub_qos.group_data.value);
    }

    element = "qos.publisher.entity_factory.autoenable_created_entities";
    if (!XMLReadString(element, "").empty() ) {
      pub_qos.entity_factory.autoenable_created_entities = _root.get<bool>(element);
    }
  }
  return 0;
}

/**
 * Initially for debug purposes only
 *
 * @param pub_qos
 * @return
 */
std::string DDSConfiguration::ShowValuesPublisherQos(const DDS::PublisherQos& pub_qos)
{
   if (_valid)
   {
      string ss;
      ss.append("PUBLISHER QOS:\n");

      // Policy kinds:
      ss.append("\tpresentation.access_scope kind: ").append(to_string(pub_qos.presentation.access_scope)).append("\n");

      // Policy Values:
      ss.append("\tpresentation\n");
      ss.append("\t\tcoherent_access: ").append(to_string(pub_qos.presentation.coherent_access)).append("\n");
      ss.append("\t\tordered_access: ").append(to_string(pub_qos.presentation.ordered_access)).append("\n");

      //ss.append("partition.name: ".append(pub_qos.partition.name.append("\n";
      //ss.append("group_data.value: ".append(pub_qos.group_data.value.append("\n";
      ss.append("\tentity_factory.autoenable_created_entities: ").append(to_string(pub_qos.entity_factory.autoenable_created_entities)).append("\n");

      return ss;
   }
   return "";
}

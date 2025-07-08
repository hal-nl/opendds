/*
 * File:   QoSConfigurationSubscriber.cpp
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
 * Set the values of subscriber QoS from XML configuration
 *
 * @param sub_qos
 * @return
 */
int DDSConfiguration::SetSubscriberQosFromXML(DDS::SubscriberQos& sub_qos)
{
  if (_valid) {
    string element("");

    element = "qos.subscriber.presentation.access_scope";
    if (!XMLReadString(element, "").empty() ) {
      string access_scope = _root.get<string>(element);

      if (access_scope == "GROUP_PRESENTATION_QOS")       sub_qos.presentation.access_scope = DDS::GROUP_PRESENTATION_QOS;
      if (access_scope == "INSTANCE_PRESENTATION_QOS")    sub_qos.presentation.access_scope = DDS::INSTANCE_PRESENTATION_QOS;
      if (access_scope == "TOPIC_PRESENTATION_QOS")  sub_qos.presentation.access_scope = DDS::TOPIC_PRESENTATION_QOS;
    }
    element = "qos.subscriber.presentation.coherent_access";
    if (!XMLReadString(element, "").empty() ) {
      sub_qos.presentation.coherent_access = _root.get<bool>(element);
    }
    element = "qos.subscriber.presentation.ordered_access";
    if (!XMLReadString(element, "").empty() ) {
      sub_qos.presentation.ordered_access = _root.get<bool>(element);
    }

    /*
     * NOT USED YET:
      element = "qos.subscriber.partition.name";
    */

    element = "qos.subscriber.group_data.value";
    if (!XMLReadString(element, "").empty() ) {
      string octet = _root.get<string>(element);
      fillFromOctetSequence(octet, sub_qos.group_data.value);
    }

    element = "qos.subscriber.entity_factory.autoenable_created_entities";
    if (!XMLReadString(element, "").empty() ) {
      sub_qos.entity_factory.autoenable_created_entities = _root.get<bool>(element);
    }
  }
  return 0;
}

/**
 * Initially for debug purposes only
 *
 * @param sub_qos
 * @return
 */
std::string DDSConfiguration::ShowValuesSubscriberQos(const DDS::SubscriberQos& sub_qos)
{
   if (_valid)
   {
      string ss;

      ss.append("SUBSCRIBER QOS:\n");

      // Policy kinds:
      ss.append("\tpresentation.access_scope kind: ").append(to_string(sub_qos.presentation.access_scope)).append("\n");

      // Policy Values:
      ss.append("\tpresentation\n");
      ss.append("\t\tcoherent_access: ").append(to_string(sub_qos.presentation.coherent_access)).append("\n");
      ss.append("\t\tordered_access: ").append(to_string(sub_qos.presentation.ordered_access)).append("\n");

      // ss.append("partition.name: "                 ).append(sub_qos.partition.name).append("\n");
      // ss.append("group_data.value: "               ).append(sub_qos.group_data.value).append("\n");
      ss.append("\tentity_factory.autoenable_created_entities: ").append(to_string(sub_qos.entity_factory.autoenable_created_entities)).append("\n");

      return ss;
   }
   return "";
}

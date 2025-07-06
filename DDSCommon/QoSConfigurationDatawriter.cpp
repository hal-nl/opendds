/*
 * File:   QoSConfigurationDatawriter.cpp
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
 * Set the values of datawriter QoS from XML configuration
 *
 * @param dw_qos
 * @return
 */
int DDSConfiguration::SetDatawriterQosFromXML(DDS::DataWriterQos& dw_qos)
{
  if (_valid) {
    string element("");

    element = "qos.datawriter.durability.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "PERSISTENT_DURABILITY_QOS")      dw_qos.durability.kind = DDS::PERSISTENT_DURABILITY_QOS;
      if (kind == "TRANSIENT_DURABILITY_QOS")       dw_qos.durability.kind = DDS::TRANSIENT_DURABILITY_QOS;
      if (kind == "TRANSIENT_LOCAL_DURABILITY_QOS") dw_qos.durability.kind = DDS::TRANSIENT_LOCAL_DURABILITY_QOS;
      if (kind == "VOLATILE_DURABILITY_QOS")        dw_qos.durability.kind = DDS::VOLATILE_DURABILITY_QOS;
    }

    element = "qos.datawriter.durability_service.service_cleanup_delay.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_SEC") {
        dw_qos.durability_service.service_cleanup_delay.sec = DDS::DURATION_ZERO_SEC;
      } else {
        dw_qos.durability_service.service_cleanup_delay.sec = _root.get<long>(element);
      }
    }
    element = "qos.datawriter.durability_service.service_cleanup_delay.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_NSEC") {
        dw_qos.durability_service.service_cleanup_delay.nanosec = DDS::DURATION_ZERO_NSEC;
      } else {
        dw_qos.durability_service.service_cleanup_delay.nanosec = _root.get<unsigned long>(element);
      }
    }
    element = "qos.datawriter.durability_service.history_kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "KEEP_ALL_HISTORY_QOS")  dw_qos.durability_service.history_kind = DDS::KEEP_ALL_HISTORY_QOS;
      if (kind == "KEEP_LAST_HISTORY_QOS") dw_qos.durability_service.history_kind = DDS::KEEP_LAST_HISTORY_QOS;
    }
    element = "qos.datawriter.durability_service.history_depth";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.durability_service.history_depth = _root.get<long>(element);
    }

    element = "qos.datawriter.durability_service.max_samples";
    if (!XMLReadString(element, "").empty() ) {
      string unlimited = _root.get<std::string>(element);
      if (unlimited == "LENGTH_UNLIMITED") {
        dw_qos.durability_service.max_samples = DDS::LENGTH_UNLIMITED;
      } else {
        long value = _root.get<long>(element);
        dw_qos.durability_service.max_samples = value;
      }
    }

    element = "qos.datawriter.durability_service.max_instances";
    if (!XMLReadString(element, "").empty() ) {
      string unlimited = _root.get<std::string>(element);
      if (unlimited == "LENGTH_UNLIMITED") {
        dw_qos.durability_service.max_instances = DDS::LENGTH_UNLIMITED;
      } else {
        long value = _root.get<long>(element);
        dw_qos.durability_service.max_instances = value;
      }
    }
    element = "qos.datawriter.durability_service.max_samples_per_instance";
    if (!XMLReadString(element, "").empty() ) {
      string unlimited = _root.get<std::string>(element);
      if (unlimited == "LENGTH_UNLIMITED") {
        dw_qos.durability_service.max_samples_per_instance = DDS::LENGTH_UNLIMITED;
      } else {
        long value = _root.get<long>(element);
        dw_qos.durability_service.max_samples_per_instance = value;
      }
    }

    element = "qos.datawriter.deadline.period.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        dw_qos.deadline.period.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dw_qos.deadline.period.sec = _root.get<long>(element);
      }
    }
    element = "qos.datawriter.deadline.period.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        dw_qos.deadline.period.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dw_qos.deadline.period.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datawriter.latency_budget.duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_SEC") {
        dw_qos.latency_budget.duration.sec = DDS::DURATION_ZERO_SEC;
      } else {
        dw_qos.latency_budget.duration.sec = _root.get<long>(element);
      }
    }
    element = "qos.datawriter.latency_budget.duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_NSEC") {
        dw_qos.latency_budget.duration.nanosec = DDS::DURATION_ZERO_NSEC;
      } else {
        dw_qos.latency_budget.duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datawriter.liveliness.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "AUTOMATIC_LIVELINESS_QOS")             dw_qos.liveliness.kind = DDS::AUTOMATIC_LIVELINESS_QOS;
      if (kind == "MANUAL_BY_PARTICIPANT_LIVELINESS_QOS") dw_qos.liveliness.kind = DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
      if (kind == "MANUAL_BY_TOPIC_LIVELINESS_QOS")       dw_qos.liveliness.kind = DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS;
    }

    element = "qos.datawriter.liveliness.lease_duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        dw_qos.liveliness.lease_duration.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dw_qos.liveliness.lease_duration.sec = _root.get<long>(element);
      }
    }

    element = "qos.datawriter.liveliness.lease_duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        dw_qos.liveliness.lease_duration.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dw_qos.liveliness.lease_duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datawriter.reliability.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "BEST_EFFORT_RELIABILITY_QOS")  dw_qos.reliability.kind = DDS::BEST_EFFORT_RELIABILITY_QOS;
      if (kind == "RELIABLE_RELIABILITY_QOS")     dw_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    }

    element = "qos.datawriter.reliability.max_blocking_time.sec";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.reliability.max_blocking_time.sec = _root.get<long>(element);
    }

    element = "qos.datawriter.reliability.max_blocking_time.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.reliability.max_blocking_time.nanosec = _root.get<unsigned long>(element);
    }

    element = "qos.datawriter.destination_order.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS") dw_qos.destination_order.kind = DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
      if (kind == "BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS") dw_qos.destination_order.kind = DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    }

    element = "qos.datawriter.history.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "KEEP_ALL_HISTORY_QOS")  dw_qos.history.kind = DDS::KEEP_ALL_HISTORY_QOS;
      if (kind == "KEEP_LAST_HISTORY_QOS")  dw_qos.history.kind = DDS::KEEP_LAST_HISTORY_QOS;
    }
    element = "qos.datawriter.history.depth";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.history.depth = _root.get<long>(element);
    }

    element = "qos.datawriter.resource_limits.max_samples";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        dw_qos.resource_limits.max_samples = DDS::LENGTH_UNLIMITED;
      } else {
        dw_qos.resource_limits.max_samples = _root.get<long>(element);
      }
    }
    element = "qos.datawriter.resource_limits.max_instances";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        dw_qos.resource_limits.max_instances = DDS::LENGTH_UNLIMITED;
      } else {
        dw_qos.resource_limits.max_instances = _root.get<long>(element);
      }
    }
    element = "qos.datawriter.resource_limits.max_samples_per_instance";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        dw_qos.resource_limits.max_samples_per_instance = DDS::LENGTH_UNLIMITED;
      } else {
        dw_qos.resource_limits.max_samples_per_instance = _root.get<long>(element);
      }
    }

    element = "qos.datawriter.transport_priority.value";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.transport_priority.value = _root.get<long>(element);
    }

    element = "qos.datawriter.lifespan.duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        dw_qos.lifespan.duration.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dw_qos.lifespan.duration.sec = _root.get<long>(element);
      }
    }

    element = "qos.datawriter.lifespan.duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        dw_qos.lifespan.duration.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dw_qos.lifespan.duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datawriter.user_data.value";
    if (!XMLReadString(element, "").empty() ) {
      string octet = _root.get<string>(element);
      fillFromOctetSequence(octet, dw_qos.user_data.value);
    }

    element = "qos.datawriter.ownership.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "SHARED_OWNERSHIP_QOS")     dw_qos.ownership.kind = DDS::SHARED_OWNERSHIP_QOS;
      if (kind == "EXCLUSIVE_OWNERSHIP_QOS")  dw_qos.ownership.kind = DDS::EXCLUSIVE_OWNERSHIP_QOS;
    }

    element = "qos.datawriter.ownership_strength.value";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.ownership_strength.value = _root.get<long>(element);
    }

    element = "qos.datawriter.writer_data_lifecycle.autodispose_unregistered_instances";
    if (!XMLReadString(element, "").empty() ) {
      dw_qos.writer_data_lifecycle.autodispose_unregistered_instances = _root.get<bool>(element);
    }
  }
  return 0;
}

/**
 * Initially for debug purposes only
 *
 * @param dw_qos
 * @return
 */
std::string DDSConfiguration::ShowValuesDatawriterQos(const DDS::DataWriterQos& dw_qos)
{
   if (_valid)
   {
      string ss;

      ss.append("DATAWRITER QOS:\n");

      // Policy kinds:
      ss.append("\tdurability kind: ")                .append(to_string(dw_qos.durability.kind)).append("\n");
      ss.append("\tliveliness kind: ")                .append(to_string(dw_qos.liveliness.kind)).append("\n");
      ss.append("\treliability kind: ")               .append(to_string(dw_qos.reliability.kind)).append("\n");
      ss.append("\tdestination_order kind: ")         .append(to_string(dw_qos.destination_order.kind)).append("\n");
      ss.append("\thistory kind: ")                   .append(to_string(dw_qos.history.kind)).append("\n");
      ss.append("\townership kind: ")                 .append(to_string(dw_qos.ownership.kind)).append("\n");
      ss.append("\tdurability_service.history kind: ").append(to_string(dw_qos.durability_service.history_kind)).append("\n");
      ss.append("\tdurability_service\n");
      ss.append("\t\tservice_cleanup_delay.sec: ")    .append(to_string(dw_qos.durability_service.service_cleanup_delay.sec)).append("\n");
      ss.append("\t\tservice_cleanup_delay.nanosec: ").append(to_string(dw_qos.durability_service.service_cleanup_delay.nanosec)).append("\n");
      ss.append("\t\thistory_depth: ")                .append(to_string(dw_qos.durability_service.history_depth)).append("\n");
      ss.append("\t\tmax_samples: ")                  .append(to_string(dw_qos.durability_service.max_samples)).append("\n");
      ss.append("\t\tmax_instances: ")                .append(to_string(dw_qos.durability_service.max_instances)).append("\n");
      ss.append("\t\tmax_samples_per_instance: ")     .append(to_string(dw_qos.durability_service.max_samples_per_instance)).append("\n");
      ss.append("\tdeadline\n");
      ss.append("\t\tperiod.sec: ")                   .append(to_string(dw_qos.deadline.period.sec)).append("\n");
      ss.append("\t\tperiod.nanosec: ")               .append(to_string(dw_qos.deadline.period.nanosec)).append("\n");
      ss.append("\tlatency_budget\n");
      ss.append("\t\tduration.sec: ")                 .append(to_string(dw_qos.latency_budget.duration.sec)).append("\n");
      ss.append("\t\tduration.nanosec: ")             .append(to_string(dw_qos.latency_budget.duration.nanosec)).append("\n");
      ss.append("\tliveliness\n");
      ss.append("\t\tlease_duration.sec: ")           .append(to_string(dw_qos.liveliness.lease_duration.sec)).append("\n");
      ss.append("\t\tlease_duration.nanosec: ")       .append(to_string(dw_qos.liveliness.lease_duration.nanosec)).append("\n");
      ss.append("\treliability\n") ;
      ss.append("\t\tmax_blocking_time.sec: ")        .append(to_string(dw_qos.reliability.max_blocking_time.sec)).append("\n");
      ss.append("\t\tmax_blocking_time.nanosec: ")    .append(to_string(dw_qos.reliability.max_blocking_time.nanosec)).append("\n");
      ss.append("\thistory.depth: ")                  .append(to_string(dw_qos.history.depth)).append("\n");
      ss.append("\tresource_limits\n");
      ss.append("\t\tmax_samples: ")                  .append(to_string(dw_qos.resource_limits.max_samples)).append("\n");
      ss.append("\t\tmax_instances: ")                .append(to_string(dw_qos.resource_limits.max_instances)).append("\n");
      ss.append("\t\tmax_samples_per_instance: ")     .append(to_string(dw_qos.resource_limits.max_samples_per_instance)).append("\n");
      ss.append("\ttransport_priority.value: ")       .append(to_string(dw_qos.transport_priority.value)).append("\n");
      ss.append("\tlifespan.duration\n");
      ss.append("\t\tduration.sec: ")                 .append(to_string(dw_qos.lifespan.duration.sec)).append("\n");
      ss.append("\t\tduration.nanosec: ")             .append(to_string(dw_qos.lifespan.duration.nanosec)).append("\n");
      ss.append("\twriter_data_lifecycle.autodispose_unregistered_instances: ").append(to_string(dw_qos.writer_data_lifecycle.autodispose_unregistered_instances)).append("\n");

      return ss;
   }
   return "";
}

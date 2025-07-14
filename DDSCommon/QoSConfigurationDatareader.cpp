/*
 * File:   QoSConfiguration.datareader.cpp
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
 * Set the values of datareader QoS from XML configuration
 *
 * @param dr_qos
 * @return
 */
int DDSConfiguration::SetDatareaderQosFromXML(DDS::DataReaderQos& dr_qos)
{
  if (_valid) {
    string element("");

    element = "qos.datareader.durability.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "PERSISTENT_DURABILITY_QOS")      dr_qos.durability.kind = DDS::PERSISTENT_DURABILITY_QOS;
      if (kind == "TRANSIENT_DURABILITY_QOS")       dr_qos.durability.kind = DDS::TRANSIENT_DURABILITY_QOS;
      if (kind == "TRANSIENT_LOCAL_DURABILITY_QOS") dr_qos.durability.kind = DDS::TRANSIENT_LOCAL_DURABILITY_QOS;
      if (kind == "VOLATILE_DURABILITY_QOS")        dr_qos.durability.kind = DDS::VOLATILE_DURABILITY_QOS;
    }

    element = "qos.datareader.deadline.period.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        dr_qos.deadline.period.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dr_qos.deadline.period.sec = _root.get<long>(element);
      }
    }
    element = "qos.datareader.deadline.period.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        dr_qos.deadline.period.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dr_qos.deadline.period.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datareader.latency_budget.duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_SEC") {
        dr_qos.latency_budget.duration.sec = DDS::DURATION_ZERO_SEC;
      } else {
        dr_qos.latency_budget.duration.sec = _root.get<long>(element);
      }
    }
    element = "qos.datareader.latency_budget.duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_NSEC") {
        dr_qos.latency_budget.duration.nanosec = DDS::DURATION_ZERO_NSEC;
      } else {
        dr_qos.latency_budget.duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datareader.liveliness.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "AUTOMATIC_LIVELINESS_QOS")             dr_qos.liveliness.kind = DDS::AUTOMATIC_LIVELINESS_QOS;
      if (kind == "MANUAL_BY_PARTICIPANT_LIVELINESS_QOS") dr_qos.liveliness.kind = DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
      if (kind == "MANUAL_BY_TOPIC_LIVELINESS_QOS")       dr_qos.liveliness.kind = DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS;
    }

    element = "qos.datareader.liveliness.lease_duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        dr_qos.liveliness.lease_duration.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dr_qos.liveliness.lease_duration.sec = _root.get<long>(element);
      }
    }

    element = "qos.datareader.liveliness.lease_duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        dr_qos.liveliness.lease_duration.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dr_qos.liveliness.lease_duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datareader.reliability.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "BEST_EFFORT_RELIABILITY_QOS")  dr_qos.reliability.kind = DDS::BEST_EFFORT_RELIABILITY_QOS;
      if (kind == "RELIABLE_RELIABILITY_QOS")     dr_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    }

    element = "qos.datareader.reliability.max_blocking_time.sec";
    if (!XMLReadString(element, "").empty() ) {
      string max_blocking_time = _root.get<std::string>(element);
      if (max_blocking_time == "DURATION_INFINITY_SEC") {
        dr_qos.reliability.max_blocking_time.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dr_qos.reliability.max_blocking_time.sec = _root.get<long>(element);
      }
    }

    element = "qos.datareader.reliability.max_blocking_time.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string max_blocking_time = _root.get<std::string>(element);
      if (max_blocking_time == "DURATION_INFINITY_NSEC") {
        dr_qos.reliability.max_blocking_time.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dr_qos.reliability.max_blocking_time.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datareader.destination_order.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS") dr_qos.destination_order.kind = DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
      if (kind == "BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS") dr_qos.destination_order.kind = DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    }

    element = "qos.datareader.history.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "KEEP_ALL_HISTORY_QOS")  dr_qos.history.kind = DDS::KEEP_ALL_HISTORY_QOS;
      if (kind == "KEEP_LAST_HISTORY_QOS")  dr_qos.history.kind = DDS::KEEP_LAST_HISTORY_QOS;
    }
    element = "qos.datareader.history.depth";
    if (!XMLReadString(element, "").empty() ) {
      dr_qos.history.depth = _root.get<long>(element);
    }

    element = "qos.datareader.resource_limits.max_samples";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        dr_qos.resource_limits.max_samples = DDS::LENGTH_UNLIMITED;
      } else {
        dr_qos.resource_limits.max_samples = _root.get<long>(element);
      }
    }
    element = "qos.datareader.resource_limits.max_instances";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        dr_qos.resource_limits.max_instances = DDS::LENGTH_UNLIMITED;
      } else {
        dr_qos.resource_limits.max_instances = _root.get<long>(element);
      }
    }
    element = "qos.datareader.resource_limits.max_samples_per_instance";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        dr_qos.resource_limits.max_samples_per_instance = DDS::LENGTH_UNLIMITED;
      } else {
        dr_qos.resource_limits.max_samples_per_instance = _root.get<long>(element);
      }
    }

    element = "qos.datareader.user_data.value";
    if (!XMLReadString(element, "").empty() ) {
      string octet = _root.get<string>(element);
      fillFromOctetSequence(octet, dr_qos.user_data.value);
    }

    element = "qos.datareader.ownership.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "SHARED_OWNERSHIP_QOS")     dr_qos.ownership.kind = DDS::SHARED_OWNERSHIP_QOS;
      if (kind == "EXCLUSIVE_OWNERSHIP_QOS")  dr_qos.ownership.kind = DDS::EXCLUSIVE_OWNERSHIP_QOS;
    }

    element = "qos.datareader.time_based_filter.minimum_separation.sec";
    if (!XMLReadString(element, "").empty() ) {
      string minimum_separation = _root.get<std::string>(element);
      if (minimum_separation == "DURATION_ZERO_SEC") {
        dr_qos.time_based_filter.minimum_separation.sec = DDS::DURATION_ZERO_SEC;
      } else {
        dr_qos.time_based_filter.minimum_separation.sec = _root.get<long>(element);
      }
    }
    element = "qos.datareader.time_based_filter.minimum_separation.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string minimum_separation = _root.get<std::string>(element);
      if (minimum_separation == "DURATION_ZERO_NSEC") {
        dr_qos.time_based_filter.minimum_separation.nanosec = DDS::DURATION_ZERO_NSEC;
      } else {
        dr_qos.time_based_filter.minimum_separation.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datareader.reader_data_lifecycle.autopurge_nowriter_samples_delay.sec";
    if (!XMLReadString(element, "").empty() ) {
      string autopurge_nowriter_samples_delay = _root.get<std::string>(element);
      if (autopurge_nowriter_samples_delay == "DURATION_INFINITY_SEC") {
        dr_qos.reader_data_lifecycle.autopurge_nowriter_samples_delay.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dr_qos.reader_data_lifecycle.autopurge_nowriter_samples_delay.sec = _root.get<long>(element);
      }
    }
    element = "qos.datareader.reader_data_lifecycle.autopurge_nowriter_samples_delay.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string autopurge_nowriter_samples_delay = _root.get<std::string>(element);
      if (autopurge_nowriter_samples_delay == "DURATION_INFINITY_NSEC") {
        dr_qos.reader_data_lifecycle.autopurge_nowriter_samples_delay.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dr_qos.reader_data_lifecycle.autopurge_nowriter_samples_delay.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.datareader.reader_data_lifecycle.autopurge_disposed_samples_delay.sec";
    if (!XMLReadString(element, "").empty() ) {
      string autopurge_disposed_samples_delay = _root.get<std::string>(element);
      if (autopurge_disposed_samples_delay == "DURATION_INFINITY_SEC") {
        dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay.sec = _root.get<long>(element);
      }
    }
    element = "qos.datareader.reader_data_lifecycle.autopurge_disposed_samples_delay.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string autopurge_disposed_samples_delay = _root.get<std::string>(element);
      if (autopurge_disposed_samples_delay == "DURATION_INFINITY_NSEC") {
        dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay.nanosec = _root.get<unsigned long>(element);
      }
    }
  }
  return 0;
}


/**
 * Initially for debug purposes only
 *
 * @param dr_qos
 * @return
 */
std::string DDSConfiguration::ShowValuesDatareaderQos(const DDS::DataReaderQos& dr_qos)
{
   if (_valid)
   {
      string ss;

      ss.append("DATAREADER QOS:\n");

      // Policy kinds:
      ss.append("\tdurability kind: "       )       .append(to_string(dr_qos.durability.kind        )).append( "\n");
      ss.append("\tliveliness kind: "       )       .append(to_string(dr_qos.liveliness.kind        )).append( "\n");
      ss.append("\treliability kind: "      )       .append(to_string(dr_qos.reliability.kind       )).append( "\n");
      ss.append("\tdestination_order kind: ")       .append(to_string(dr_qos.destination_order.kind )).append( "\n");
      ss.append("\thistory kind: "          )       .append(to_string(dr_qos.history.kind           )).append( "\n");
      ss.append("\townership kind: "        )       .append(to_string(dr_qos.ownership.kind         )).append( "\n");
      ss.append("\tdeadline\n");
      ss.append("\t\tperiod.sec: ")                 .append(to_string(dr_qos.deadline.period.sec)) .append( "\n");
      ss.append("\t\tperiod.nanosec: ")             .append(to_string(dr_qos.deadline.period.nanosec)) .append( "\n");
      ss.append("\tlatency_budget\n");
      ss.append("\t\tduration.sec: ")               .append(to_string(dr_qos.latency_budget.duration.sec)) .append( "\n");
      ss.append("\t\tduration.nanosec: ")           .append(to_string(dr_qos.latency_budget.duration.nanosec)) .append( "\n");
      ss.append("\tliveliness\n");
      ss.append("\t\tlease_duration.sec: ")         .append(to_string(dr_qos.liveliness.lease_duration.sec)) .append( "\n");
      ss.append("\t\tlease_duration.nanosec: ")     .append(to_string(dr_qos.liveliness.lease_duration.nanosec)) .append( "\n");
      ss.append("\treliability\n");
      ss.append("\t\tmax_blocking_time.sec: ")      .append(to_string(dr_qos.reliability.max_blocking_time.sec)) .append( "\n");
      ss.append("\t\tmax_blocking_time.nanosec: ")  .append(to_string(dr_qos.reliability.max_blocking_time.nanosec)) .append( "\n");
      ss.append("\thistory.depth: ")                .append(to_string(dr_qos.history.depth)) .append( "\n");
      ss.append("\tresource_limits\n");
      ss.append("\t\tmax_samples: ")                .append(to_string(dr_qos.resource_limits.max_samples)) .append( "\n");
      ss.append("\t\tmax_instances: ")              .append(to_string(dr_qos.resource_limits.max_instances)) .append( "\n");
      ss.append("\t\tmax_samples_per_instance: " )  .append(to_string(dr_qos.resource_limits.max_samples_per_instance)) .append( "\n");
      ss.append("\ttime_based_filter\n");
      ss.append("\t\tminimum_separation.sec: ")     .append(to_string(dr_qos.time_based_filter.minimum_separation.sec)) .append( "\n");
      ss.append("\t\tminimum_separation.nanosec: " ).append(to_string(dr_qos.time_based_filter.minimum_separation.nanosec)) .append( "\n");
      ss.append("\treader_data_lifecycle\n");
      ss.append("\t\tautopurge_nowriter_samples_delay.sec: "     ) .append(to_string(dr_qos.reader_data_lifecycle.autopurge_nowriter_samples_delay.sec)) .append( "\n");
      ss.append("\t\tautopurge_nowriter_samples_delay.nanosec: " ) .append(to_string(dr_qos.reader_data_lifecycle.autopurge_nowriter_samples_delay.nanosec)) .append( "\n");
      ss.append("\t\tautopurge_disposed_samples_delay.sec: "     ) .append(to_string(dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay.sec)) .append( "\n");
      ss.append("\t\tautopurge_disposed_samples_delay.nanosec: " ) .append(to_string(dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay.nanosec)) .append( "\n");

      return ss;
   }
   return "";
}

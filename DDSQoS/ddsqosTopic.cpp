/*
 * File:   ddsqosTopic.cpp
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

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

using namespace std;

#include "ddsqos.hpp"

/**
 * Set the values of topic QoS from XML configuration
 *
 * @param topic_qos
 * @return
 */
int DDSQOS::SetTopicQos(DDS::TopicQos& topic_qos)
{
  if (_valid) {
    string element("");

    element = "qos.topic.topic_data.value";
    if (!XMLReadString(element, "").empty() ) {
      string octet = _root.get<string>(element);
      fillFromOctetSequence(octet, topic_qos.topic_data.value);
    }

    element = "qos.topic.durability.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "PERSISTENT_DURABILITY_QOS")      topic_qos.durability.kind = DDS::PERSISTENT_DURABILITY_QOS;
      if (kind == "TRANSIENT_DURABILITY_QOS")       topic_qos.durability.kind = DDS::TRANSIENT_DURABILITY_QOS;
      if (kind == "TRANSIENT_LOCAL_DURABILITY_QOS") topic_qos.durability.kind = DDS::TRANSIENT_LOCAL_DURABILITY_QOS;
      if (kind == "VOLATILE_DURABILITY_QOS")        topic_qos.durability.kind = DDS::VOLATILE_DURABILITY_QOS;
    }

    element = "qos.topic.durability_service.service_cleanup_delay.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_SEC") {
        topic_qos.durability_service.service_cleanup_delay.sec = DDS::DURATION_ZERO_SEC;
      } else {
        topic_qos.durability_service.service_cleanup_delay.sec = _root.get<long>(element);
      }
    }
    element = "qos.topic.durability_service.service_cleanup_delay.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_NSEC") {
        topic_qos.durability_service.service_cleanup_delay.nanosec = DDS::DURATION_ZERO_NSEC;
      } else {
        topic_qos.durability_service.service_cleanup_delay.nanosec = _root.get<unsigned long>(element);
      }
    }
    element = "qos.topic.durability_service.history_kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "KEEP_ALL_HISTORY_QOS")  topic_qos.durability_service.history_kind = DDS::KEEP_ALL_HISTORY_QOS;
      if (kind == "KEEP_LAST_HISTORY_QOS") topic_qos.durability_service.history_kind = DDS::KEEP_LAST_HISTORY_QOS;
    }
    element = "qos.topic.durability_service.history_depth";
    if (!XMLReadString(element, "").empty() ) {
      topic_qos.durability_service.history_depth = _root.get<long>(element);
    }

    element = "qos.topic.durability_service.max_samples";
    if (!XMLReadString(element, "").empty() ) {
      string unlimited = _root.get<std::string>(element);
      if (unlimited == "LENGTH_UNLIMITED") {
        topic_qos.durability_service.max_samples = DDS::LENGTH_UNLIMITED;
      } else {
        long value = _root.get<long>(element);
        topic_qos.durability_service.max_samples = value;
      }
    }

    element = "qos.topic.durability_service.max_instances";
    if (!XMLReadString(element, "").empty() ) {
      string unlimited = _root.get<std::string>(element);
      if (unlimited == "LENGTH_UNLIMITED") {
        topic_qos.durability_service.max_instances = DDS::LENGTH_UNLIMITED;
      } else {
        long value = _root.get<long>(element);
        topic_qos.durability_service.max_instances = value;
      }
    }
    element = "qos.topic.durability_service.max_samples_per_instance";
    if (!XMLReadString(element, "").empty() ) {
      string unlimited = _root.get<std::string>(element);
      if (unlimited == "LENGTH_UNLIMITED") {
        topic_qos.durability_service.max_samples_per_instance = DDS::LENGTH_UNLIMITED;
      } else {
        long value = _root.get<long>(element);
        topic_qos.durability_service.max_samples_per_instance = value;
      }
    }

    element = "qos.topic.deadline.period.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        topic_qos.deadline.period.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        topic_qos.deadline.period.sec = _root.get<long>(element);
      }
    }
    element = "qos.topic.deadline.period.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        topic_qos.deadline.period.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        topic_qos.deadline.period.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.topic.latency_budget.duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_SEC") {
        topic_qos.latency_budget.duration.sec = DDS::DURATION_ZERO_SEC;
      } else {
        topic_qos.latency_budget.duration.sec = _root.get<long>(element);
      }
    }
    element = "qos.topic.latency_budget.duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_ZERO_NSEC") {
        topic_qos.latency_budget.duration.nanosec = DDS::DURATION_ZERO_NSEC;
      } else {
        topic_qos.latency_budget.duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.topic.liveliness.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "AUTOMATIC_LIVELINESS_QOS")             topic_qos.liveliness.kind = DDS::AUTOMATIC_LIVELINESS_QOS;
      if (kind == "MANUAL_BY_PARTICIPANT_LIVELINESS_QOS") topic_qos.liveliness.kind = DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
      if (kind == "MANUAL_BY_TOPIC_LIVELINESS_QOS")       topic_qos.liveliness.kind = DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS;
    }

    element = "qos.topic.liveliness.lease_duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        topic_qos.liveliness.lease_duration.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        topic_qos.liveliness.lease_duration.sec = _root.get<long>(element);
      }
    }

    element = "qos.topic.liveliness.lease_duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<std::string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        topic_qos.liveliness.lease_duration.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        topic_qos.liveliness.lease_duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.topic.reliability.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "BEST_EFFORT_RELIABILITY_QOS")  topic_qos.reliability.kind = DDS::BEST_EFFORT_RELIABILITY_QOS;
      if (kind == "RELIABLE_RELIABILITY_QOS")     topic_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    }

    element = "qos.topic.reliability.max_blocking_time.sec";
    if (!XMLReadString(element, "").empty() ) {
      string maxblockingtime = _root.get<string>(element);
      if (maxblockingtime == "DURATION_INFINITY_SEC") {
        topic_qos.reliability.max_blocking_time.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        topic_qos.reliability.max_blocking_time.sec = _root.get<long>(element);
      }
    }

    element = "qos.topic.reliability.max_blocking_time.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string maxblockingtime = _root.get<string>(element);
      if (maxblockingtime == "DURATION_INFINITY_NSEC") {
        topic_qos.reliability.max_blocking_time.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        topic_qos.reliability.max_blocking_time.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.topic.destination_order.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS") topic_qos.destination_order.kind = DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
      if (kind == "BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS") topic_qos.destination_order.kind = DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    }

    element = "qos.topic.history.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);

      if (kind == "KEEP_ALL_HISTORY_QOS")  topic_qos.history.kind = DDS::KEEP_ALL_HISTORY_QOS;
      if (kind == "KEEP_LAST_HISTORY_QOS")  topic_qos.history.kind = DDS::KEEP_LAST_HISTORY_QOS;
    }
    element = "qos.topic.history.depth";
    if (!XMLReadString(element, "").empty() ) {
      topic_qos.history.depth = _root.get<long>(element);
    }

    element = "qos.topic.resource_limits.max_samples";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        topic_qos.resource_limits.max_samples = DDS::LENGTH_UNLIMITED;
      } else {
        topic_qos.resource_limits.max_samples = _root.get<long>(element);
      }
    }
    element = "qos.topic.resource_limits.max_instances";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        topic_qos.resource_limits.max_instances = DDS::LENGTH_UNLIMITED;
      } else {
        topic_qos.resource_limits.max_instances = _root.get<long>(element);
      }
    }
    element = "qos.topic.resource_limits.max_samples_per_instance";
    if (!XMLReadString(element, "").empty() ) {
      string lengthunlimited = _root.get<string>(element);
      if (lengthunlimited == "LENGTH_UNLIMITED") {
        topic_qos.resource_limits.max_samples_per_instance = DDS::LENGTH_UNLIMITED;
      } else {
        topic_qos.resource_limits.max_samples_per_instance = _root.get<long>(element);
      }
    }

    element = "qos.topic.transport_priority.value";
    if (!XMLReadString(element, "").empty() ) {
      topic_qos.transport_priority.value = _root.get<long>(element);
    }

    element = "qos.topic.lifespan.duration.sec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<string>(element);
      if (duration == "DURATION_INFINITY_SEC") {
        topic_qos.lifespan.duration.sec = DDS::DURATION_INFINITE_SEC;
      } else {
        topic_qos.lifespan.duration.sec = _root.get<long>(element);
      }
    }

    element = "qos.topic.lifespan.duration.nanosec";
    if (!XMLReadString(element, "").empty() ) {
      string duration = _root.get<string>(element);
      if (duration == "DURATION_INFINITY_NSEC") {
        topic_qos.lifespan.duration.nanosec = DDS::DURATION_INFINITE_NSEC;
      } else {
        topic_qos.lifespan.duration.nanosec = _root.get<unsigned long>(element);
      }
    }

    element = "qos.topic.ownership.kind";
    if (!XMLReadString(element, "").empty() ) {
      string kind = _root.get<string>(element);
      if (kind == "SHARED_OWNERSHIP_QOS")     topic_qos.ownership.kind = DDS::SHARED_OWNERSHIP_QOS;
      if (kind == "EXCLUSIVE_OWNERSHIP_QOS")  topic_qos.ownership.kind = DDS::EXCLUSIVE_OWNERSHIP_QOS;
    }
  }
  return 0;
}



/*
 * File:   Vehicle_local.hpp
 *
 * Author: Haluk Ates
 *
 * Created 2021-04-01
 */

#ifndef VEHICLE_LOCAL_H
#define VEHICLE_LOCAL_H

// ----------------------------------------------------------------------------------------
// Default Macros
// ----------------------------------------------------------------------------------------
#include "ObjectVisibility.hpp"
#include "../_idls/Vehicle/VehicleIdl.hpp"


#include <ace/Log_Msg.h>

#include "../DDSQoS/ddsqos.hpp"

#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include "../_idls/Vehicle/generated/VehicleTypeSupportImpl.h"

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <dds/DCPS/StaticIncludes.h>

#include "Vehicle_global.hpp"

// ----------------------------------------------------------------------------------------
// Internal structs for internal data
// ----------------------------------------------------------------------------------------
struct LOCALINFO
{
   int   infoLevelPub;
   int   infoLevelSub;
   bool  monitor;
};

// ----------------------------------------------------------------------------------------
// Internal structs for handling the DDS variables:
// ----------------------------------------------------------------------------------------
struct LOCALDDS
{
   int                                 domain;
   DDS::InstanceHandle_t               handle;

   DDS::DomainParticipant_var          participant;
   DDS::DomainParticipantFactory_var   dpf;

   DDS::Publisher_var                  pub;
   DDS::Subscriber_var                 sub;
   DDS::DataWriter_var                 dataWriter;
   DDS::DataReader_var                 dataReader;
   DDS::Topic_var                      topic;
   DDS::ContentFilteredTopic_var       topicFiltered;
   CORBA::String_var                   typeName;
};

// ----------------------------------------------------------------------------------------
// Internal structs for handling the DDS QoS :
// ----------------------------------------------------------------------------------------
struct LOCALQOS
{
   string                              ddsQos;
   string                              ConfFile;
   DDSQOS*                             qosConf;
   DDSQOS*                             ddsQosConfiguration;

   DDS::DomainParticipantQos           qosParticipant;
   DDS::PublisherQos                   qosPublisher;
   DDS::DataWriterQos                  qosWriter;
   DDS::SubscriberQos                  qosSubscriber;
   DDS::DataReaderQos                  qosReader;
   DDS::TopicQos                       qosTopic;
};

// ----------------------------------------------------------------------------------------
// Topic related structs:
// ----------------------------------------------------------------------------------------
struct VEHICLEDATA : LOCALDDS, LOCALINFO, LOCALQOS
{
   Vehicle::DataDataWriter_var      Writer;
   Vehicle::DataTypeSupport_var     typeSupport;
   Vehicle::Data                    Vehicle;
};


DLL_LOCAL int vehicleDataCreate      (DDSCOMMON::OUTPUTFUNC debugFunc, int argc, ACE_TCHAR *argv[]);
DLL_LOCAL int vehicleDataCreateArgv  (DDSCOMMON::OUTPUTFUNC debugFunc, ACE_TCHAR *arguments);
DLL_LOCAL int vehicleDataDestroy     (void);

DLL_LOCAL int vehicleDataSend        (const struct VEHICLEIDL::Data& dataStruct);

DLL_LOCAL int vehicleDataReaderListener(CBF_VEHICLEDATA readerFunc, DDSCOMMON::OUTPUTFUNC debugFunc);
DLL_LOCAL int vehicleDataWriterListener(CBF_VEHICLEDATA writerFunc, DDSCOMMON::OUTPUTFUNC debugFunc);

DLL_LOCAL void vehicleWriteOutput  (const char *output);

#endif /* VEHICLE_LOCAL_H */


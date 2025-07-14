/*
 * File:   Vehicle_local.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */


#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "../DDSCommon/DDSCommon.hpp"
#include "../DDSCommon/DDSHelper.hpp"

#include "../_idls/Vehicle/generated/VehicleTypeSupportImpl.h"

#include "Vehicle_local.hpp"
#include "Vehicle_global.hpp"
#include "VehicleDataReaderListenerImpl.hpp"
#include "VehicleDataWriterListenerImpl.hpp"

using namespace std;

// ----------------------------------------------------------------------------------------
// Generated UUID string for Vehicle.
// This will be the key to filter out self publishers
// ----------------------------------------------------------------------------------------
DLL_LOCAL string UUID;

// ----------------------------------------------------------------------------------------
// Filtering vehicles based on UUID
// ----------------------------------------------------------------------------------------
DLL_LOCAL string uuidFiltered = string(VEHICLEIDL::TOPIC_DATA) + string( "_UUID_FILTERED");
DLL_LOCAL string uuidQuery;

// ----------------------------------------------------------------------------------------
// Additional Program arguments
// ----------------------------------------------------------------------------------------
DLL_LOCAL string    argumentName;
DLL_LOCAL string    argumentDomain;

DLL_LOCAL VEHICLEDATA   vd;

// ----------------------------------------------------------------------------------------
// Internal function to write info and debug messages via callback
// ----------------------------------------------------------------------------------------
DLL_LOCAL DDSCOMMON::OUTPUTFUNC writeOutput = nullptr;

DLL_LOCAL void WriteOutput(const string& m)
{
  if (writeOutput) writeOutput(m.c_str());
}
DLL_LOCAL void WriteOutput(int value)
{
  if (writeOutput) writeOutput(to_string(value).c_str());
}

int vehicleDataCreateArgv(DDSCOMMON::OUTPUTFUNC debugFunc, char* arguments)
{
   vector<string> arg;
   boost::split(arg, arguments, boost::is_any_of(" \t"));

   int argcc = arg.size();

   // ----------------------------------------------------
   // Allocate char**
   // ----------------------------------------------------
   char **argvv = new char*[argcc] ;

   for (size_t i(0); i < arg.size(); i++)
   {
      string s = arg[i];
      // Allocate char*
      argvv[i] = new char[s.size() + 1];
      strncpy(argvv[i], s.data(), s.size());
      argvv[i][s.size()] = '\0';
   }

   // ----------------------------------------------------
   // call the other internal create function
   // ----------------------------------------------------
   int retValue = vehicleDataCreate(debugFunc, argcc, argvv);

   // ----------------------------------------------------
   // De-allocate pointers
   // ----------------------------------------------------
   for (size_t i(0); i < arg.size(); i++) delete argvv[i];
   delete [] argvv;

   return retValue;
}


/* DLL_LOCAL */
int vehicleDataCreate(DDSCOMMON::OUTPUTFUNC debugFunc, int argc, ACE_TCHAR *argv[])
{
   /* *********************************************************************************************
   * Initialize local variables
   * *********************************************************************************************/
   writeOutput = debugFunc;

   UUID = DDSCOMMON::CreateUUID();
   argumentName   = "";
   argumentDomain = "";
   uuidQuery      = "uuid <> '" + UUID + "'";

   vd.domain        = VEHICLEIDL::DOMAIN;
   vd.infoLevelPub  = DDSCOMMON::DEBUG::NOTHING;
   vd.infoLevelSub  = DDSCOMMON::DEBUG::NOTHING;
   vd.monitor       = false;
   vd.ddsQos        = "";
   vd.ConfFile      = "";

   /* *********************************************************************************************
   * Get the application arguments and set vehicle parameters
   * *********************************************************************************************/
   map<string, string> parameters;
   /* int count = */ DDSCOMMON::GetApplicationArguments(argc, const_cast<const char**>(argv), parameters);

   if (parameters.find(DDSCOMMON::PARAMETERS::NAME) != parameters.end()) {
      if (!parameters[DDSCOMMON::PARAMETERS::NAME].empty()) {
         argumentName = parameters[DDSCOMMON::PARAMETERS::NAME];
      }
   }

   if (parameters.find(DDSCOMMON::PARAMETERS::DOMAIN) != parameters.end()) {
      if (!parameters[DDSCOMMON::PARAMETERS::DOMAIN].empty()) {
         argumentDomain = parameters[DDSCOMMON::PARAMETERS::DOMAIN];
         try
         {
            int value = std::stoi(argumentDomain);
            if ((value != VEHICLEIDL::DOMAIN) && (value < 200)) {
                vd.domain = value;
            }
         }
         catch (...) { /* do nothing*/ }
      }
   }

   if (parameters.find(DDSCOMMON::PARAMETERS::MONITOR) != parameters.end()) {
      if (!parameters[DDSCOMMON::PARAMETERS::MONITOR].empty()) {
         string monitor = parameters[DDSCOMMON::PARAMETERS::MONITOR];
         vd.monitor = (monitor == string("true"));
      }
   }

   if (parameters.find(DDSCOMMON::PARAMETERS::DDSCONFIG) != parameters.end()) {
      if (!parameters[DDSCOMMON::PARAMETERS::DDSCONFIG].empty()) {
         vd.ConfFile = parameters[DDSCOMMON::PARAMETERS::DDSCONFIG];
         if (!boost::filesystem::exists(vd.ConfFile)) {
            WriteOutput("The Configuration file '" + vd.ConfFile + "' does not exist.\n");
            vd.ConfFile = "";
         }
      }
   }

   /* *********************************************************************************************
   * Get Global Configuration settings from XML file
   * *********************************************************************************************/
   string xmlitem_pub_infolevel = "dds.modules.vehicle.publisher.data.infolevel";
   string xmlitem_sub_infolevel = "dds.modules.vehicle.subscriber.data.infolevel";
   string xmlitem_ddsQos        = "dds.modules.vehicle.ddsqos";

   vd.ddsQosConfiguration = new DDSQOS();
   vd.ddsQosConfiguration->XMLLoadFromFile(vd.ConfFile);
   vd.infoLevelPub = vd.ddsQosConfiguration->XMLReadInteger(xmlitem_pub_infolevel);
   vd.infoLevelSub = vd.ddsQosConfiguration->XMLReadInteger(xmlitem_sub_infolevel);
   vd.ddsQos       = vd.ddsQosConfiguration->XMLReadString(xmlitem_ddsQos);

   /* *********************************************************************************************
   * Show info depending on info level
   * *********************************************************************************************/
   if (vd.infoLevelPub & DDSCOMMON::DEBUG::INFO) {
      string ss;
      ss.append("infoLeveldataPublisher: '").append(to_string(vd.infoLevelPub)).append("'\n");
      WriteOutput(ss);
   }
   if (vd.infoLevelSub & DDSCOMMON::DEBUG::INFO) {
      string ss;
      ss.append("infoLeveldataSubscriber: '").append(to_string(vd.infoLevelSub)).append("'\n");
      WriteOutput(ss);
   }
   if ((vd.infoLevelPub || vd.infoLevelSub) & DDSCOMMON::DEBUG::INFO) {
      string dstr;
      dstr.append("Working domain: '").append(to_string(vd.domain)).append("'\n");
      WriteOutput(dstr);
      if (!argumentName.empty()) {
         WriteOutput("Vehicle Name: '" + argumentName + "'\n");
         WriteOutput("Vehicle UUID: '" + UUID + "'\n");
      }
         if (!vd.ddsQos.empty()) {
         WriteOutput("QoS xmlfile: '" + vd.ddsQos + "'\n");
      }
   }

   try {
         DDS::ReturnCode_t qosret;

#ifndef LINUX
      ACE::init();
#endif

      /* *********************************************************************************************
      * Initialize the Participant
      * *********************************************************************************************/
      vd.dpf = TheParticipantFactoryWithArgs(argc, argv);

      /* *********************************************************************************************
      * Get default DomainParticipant QoS from a DomainParticipantFactory:
      * *********************************************************************************************/
      qosret = vd.dpf->get_default_participant_qos(vd.qosParticipant);
      if (DDS::RETCODE_OK != qosret) {
      WriteOutput("Could not get default participant QoS!\n");
         return -1;
      }

      /* *********************************************************************************************
      * Get QoS settings from XML: If the file is not found or parameterName is empty(""),
      * then the QoS values will not be set.
      * *********************************************************************************************/
      vd.qosConf = new DDSQOS();
      vd.qosConf->XMLLoadFromFile(vd.ddsQos);

      /**
      * -------------------------------------------------------------------------------------------
      * Create the publisher, subscriber, topic, filter etc. for DATA and POSITION
      * -------------------------------------------------------------------------------------------
      */
      /* *********************************************************************************************
      * Overwrite defaults with QoS settings from dataQosFile:
      * *********************************************************************************************/
      vd.qosConf->SetParticipantQos(vd.qosParticipant);
      vd.participant = vd.dpf->create_participant(
         vd.domain, vd.qosParticipant,
         0,  /* NO domain participant listener*/
         OpenDDS::DCPS::DEFAULT_STATUS_MASK);
      if (!vd.participant)
      {
         WriteOutput("Create_participant failed for " + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }

      /* ---------------------------------------------------------------------------------
      * Register Data TypeSupport
      * --------------------------------------------------------------------------------- */
      vd.typeSupport = new Vehicle::DataTypeSupportImpl;
      if (vd.typeSupport->register_type(vd.participant, "") != DDS::RETCODE_OK)
      {
         WriteOutput("Register_type failed for" + VEHICLEIDL::TOPIC_DATA + "\n");
      }

      /* *********************************************************************************************
      * Get default Topic QoS from a DomainParticipant:
      * *********************************************************************************************/
      qosret = vd.participant->get_default_topic_qos(vd.qosTopic);
      if (DDS::RETCODE_OK != qosret)
      {
         WriteOutput("Could not get default topic QoS for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }
      /* *********************************************************************************************
      * Overwrite defaults with QoS settings from dataQosFile:
      * *********************************************************************************************/
      vd.qosConf->SetTopicQos(vd.qosTopic);

      /* ---------------------------------------------------------------------------------
      * Create Data Topic
      * --------------------------------------------------------------------------------- */
      vd.typeName = vd.typeSupport->get_type_name();
      vd.topic = vd.participant->create_topic(
         VEHICLEIDL::TOPIC_DATA.c_str(),
         vd.typeName,
         vd.qosTopic,
         0,  /* NO topic listener */
         OpenDDS::DCPS::DEFAULT_STATUS_MASK);
      if (!vd.topic)
      {
         WriteOutput("Create_topic failed for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }

      /* *********************************************************************************************
      * Get default dataPublisher QoS from a DomainParticipant:
      * *********************************************************************************************/
      qosret = vd.participant->get_default_publisher_qos(vd.qosPublisher);
      if (DDS::RETCODE_OK != qosret)
      {
         WriteOutput("Could not get default qosPublisher for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }
      /* *********************************************************************************************
      * Overwrite defaults with QoS settings from dataQosFile:
      * *********************************************************************************************/
      vd.qosConf->SetPublisherQos(vd.qosPublisher);

      /* *********************************************************************************************
      * Create A Publisher
      * *********************************************************************************************/
      vd.pub = vd.participant->create_publisher(
         vd.qosPublisher,
         0, /* NO publisher listener */
         OpenDDS::DCPS::DEFAULT_STATUS_MASK);
      if (!vd.pub)
      {
         WriteOutput("Create_Publisher failed for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }

      /* *********************************************************************************************
      * Get default DataWriter QoS from a Publisher:
      * *********************************************************************************************/
      qosret = vd.pub->get_default_datawriter_qos(vd.qosWriter);
      if (DDS::RETCODE_OK != qosret)
      {
         WriteOutput("Could not get default data writer QoS for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }
      /* *********************************************************************************************
      * Overwrite defaults with QoS settings from dataQosFile:
      * *********************************************************************************************/
      vd.qosConf->SetDatawriterQos(vd.qosWriter);

      /* *********************************************************************************************
      * Does dataSubscriber topic need filtering? Yes: Filter self!
      * *********************************************************************************************/
      vd.topicFiltered = vd.participant->create_contentfilteredtopic(
          uuidFiltered.c_str(), vd.topic, uuidQuery.c_str(), DDS::StringSeq());

      /* *********************************************************************************************
      * Get default Subscriber QoS from a DomainParticipant:
      * *********************************************************************************************/
      qosret = vd.participant->get_default_subscriber_qos(vd.qosSubscriber);
      if (DDS::RETCODE_OK != qosret)
      {
         WriteOutput("Could not get default Subscriber QoS for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }
      /* *********************************************************************************************
      * Overwrite defaults with QoS settings from dataQosFile:
      * *********************************************************************************************/
      vd.qosConf->SetSubscriberQos(vd.qosSubscriber);

      /* *********************************************************************************************
      * Create A Subscriber
      * *********************************************************************************************/
      vd.sub = vd.participant->create_subscriber(
         vd.qosSubscriber,
         0, /* NO subscriber listener */
         OpenDDS::DCPS::DEFAULT_STATUS_MASK);
      if (!vd.sub)
      {
         WriteOutput("create_Subscriber failed for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }

      /* *********************************************************************************************
      * Get default DataReader QoS from a dataSubscriber:
      * *********************************************************************************************/
      qosret = vd.sub->get_default_datareader_qos(vd.qosReader);
      if (DDS::RETCODE_OK != qosret)
      {
         WriteOutput("Could not get default data reader QoS for" + VEHICLEIDL::TOPIC_DATA + "\n");
         return -1;
      }
      /* *********************************************************************************************
      * Overwrite defaults with QoS settings from dataQosFile:
      * *********************************************************************************************/
      vd.qosConf->SetDatareaderQos(vd.qosReader);

   } catch (const CORBA::Exception& e)
   {
      WriteOutput("Exception caught in Vehicle Create()");
      return -1;
   }
   return 0;
}

/* *********************************************************************************************
* Create A DataReader Listener
* *********************************************************************************************/
DLL_LOCAL int vehicleDataReaderListener(CBF_VEHICLEDATA readerFunc, DDSCOMMON::OUTPUTFUNC debugFunc)
{
   DDS::DataReaderListener_var readerListener(
      new VehicleDataReaderListenerImpl(readerFunc, debugFunc, vd.infoLevelSub, vd.monitor));

   // Does Reader need filtering? Yes, self!
   vd.dataReader = vd.sub->create_datareader(
      vd.topicFiltered,
      vd.qosReader,
      readerListener,
      OpenDDS::DCPS::DEFAULT_STATUS_MASK);
   if (!vd.dataReader)
   {
      WriteOutput("create_datareader data failed!\n");
      return -1;
   }
   Vehicle::DataDataReader_var dataReader_i = Vehicle::DataDataReader::_narrow(vd.dataReader);
   if (!dataReader_i)
   {
      WriteOutput("_narrow dataReader failed!\n");
      return -1;
   }
   return 0;
}


/* *********************************************************************************************
* Create A Data Writer Listener for Data
* *********************************************************************************************/
DLL_LOCAL int vehicleDataWriterListener(CBF_VEHICLEDATA writerFunc, DDSCOMMON::OUTPUTFUNC debugFunc)
{
   DDS::DataWriterListener_var writerListener(
      new VehicleDataWriterListenerImpl(writerFunc, debugFunc, vd.infoLevelSub, vd.monitor));

   vd.dataWriter = vd.pub->create_datawriter(
      vd.topic,
      vd.qosWriter,
      writerListener,
      OpenDDS::DCPS::DEFAULT_STATUS_MASK);
   if (!vd.dataWriter)
   {
      WriteOutput("Create_datawriter data failed!\n");
      return -1;
   }
   vd.Writer = Vehicle::DataDataWriter::_narrow(vd.dataWriter);
   if (!vd.Writer)
   {
      WriteOutput("_narrow data failed!\n");
      return -1;
   }

   /* --------------------------------------------------------------------------------------------
   * Register the data writer's instances for efficiency
   * ------------------------------------------------------------------------------------------- */
   vd.Vehicle.index = 0;
   vd.handle = vd.Writer->register_instance(vd.Vehicle);

   return 0;
}

DLL_LOCAL int vehicleDataSend(const struct VEHICLEIDL::Data& dataStruct)
{
  try
  {
    // -----------------------------------------------------------
    // FILL DATA
    // -----------------------------------------------------------
    vd.Vehicle.index++;

    vd.Vehicle.type    = static_cast<Vehicle::VehicleEnum>(dataStruct.type);
    vd.Vehicle.uuid    = CORBA::string_dup(UUID.c_str());
    vd.Vehicle.ip      = dataStruct.ip;
    vd.Vehicle.ip_port = dataStruct.ip_port;

    vd.Vehicle.velocity      = dataStruct.velocity;
    vd.Vehicle.acceleration  = dataStruct.acceleration;
    vd.Vehicle.latitude      = dataStruct.latitude;
    vd.Vehicle.longitude     = dataStruct.longitude;
    vd.Vehicle.altitude      = dataStruct.altitude;

    strncpy(vd.Vehicle.name, CORBA::string_dup(argumentName.c_str()), Vehicle::VEHICLE_NAME_LEN - 1);
    vd.Vehicle.name[Vehicle::VEHICLE_NAME_LEN - 1] = '\0';

    if (vd.infoLevelPub & DDSCOMMON::DEBUG::INFO)
    {
      WriteOutput("SENDING DATA at " + DDSCOMMON::Time2IsoString() + " ");
    }
    if (vd.infoLevelPub & DDSCOMMON::DEBUG::VALUES)
    {
      WriteOutput("Name ==> " + string(dataStruct.name) + "\n");
    }

    // -----------------------------------------------------------
    // SEND DATA
    // -----------------------------------------------------------
    DDS::ReturnCode_t error(DDS::RETCODE_OK);
    try
    {
      error = vd.Writer->write(vd.Vehicle, vd.handle);
      return error;
    }
    catch (exception& ex)
    {
      string ss;
      ss.append("Exception in Data Writer: ").append(ex.what()).append("!\n");
      WriteOutput(ss);
    }
    if (error != DDS::RETCODE_OK)
    {
      string ss;
      ss.append("Write data returned ").append(to_string(error)).append("!\n");
      WriteOutput(ss);

      return error;
    }
    if (vd.infoLevelPub & DDSCOMMON::DEBUG::INFO) {
      WriteOutput("SENT DATA \n");
    }
  }
  catch (...) {
    WriteOutput("Unknown error occurred in send data\n");;
    return -1;
  }
  return 0;
}

/* DLL_LOCAL */
int vehicleDataDestroy(void)
{
  try {
    vd.participant->delete_contained_entities();
    vd.dpf->delete_participant(vd.participant);

    TheServiceParticipant->shutdown();
  #ifndef LINUX
    ACE::fini();
  #endif

    if (vd.qosConf) delete vd.qosConf;
    if (vd.ddsQosConfiguration) delete vd.ddsQosConfiguration;

  } catch(...) {
    WriteOutput("Unknown error occurred in Vehicle Destroy.\n");;
    return -1;
  }
  return 0;
}


/* DLL_LOCAL */
void vehicleWriteOutput(const char *output)
{
  WriteOutput(output);
}

/*
 * File:   VehicleDataReaderListenerImpl.cpp
 *
 * Author: Haluk Ates
 *
 * Created 2021-04-01
 */

#include <iostream>


#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include "VehicleDataReaderListenerImpl.hpp"

#include "../DDSCommon/DDSHelper.hpp"

#include "../_idls/Vehicle/generated/VehicleTypeSupportC.h"
#include "../_idls/Vehicle/generated/VehicleTypeSupportImpl.h"

VehicleDataReaderListenerImpl::VehicleDataReaderListenerImpl(
    CBF_VEHICLEDATA dataFunc, DDSCOMMON::OUTPUTFUNC debugFunc, int infolevel, bool monitor)
  : _dataFunc(dataFunc), _debugFunc(debugFunc),
    _infolevel(infolevel), _monitor(monitor)
{
}

VehicleDataReaderListenerImpl::~VehicleDataReaderListenerImpl(){}

void VehicleDataReaderListenerImpl::WriteOutput(const string& m)
{
  if (_debugFunc) _debugFunc(m.c_str());
}
void VehicleDataReaderListenerImpl::WriteOutput(int value)
{
  if (_debugFunc) _debugFunc(to_string(value).c_str());
}


void VehicleDataReaderListenerImpl::on_requested_deadline_missed(
  DDS::DataReader_ptr /*reader*/, const DDS::RequestedDeadlineMissedStatus& status)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("on_requested_deadline_missed()\n");
  }
  if (_monitor) {
    cout  << "RequestedDeadlineMissedStatus\n"
          << "Total count           : " << status.total_count             << "\n"
          << "Total count change    : " << status.total_count_change      << "\n";
  }
}

void VehicleDataReaderListenerImpl::on_requested_incompatible_qos(
  DDS::DataReader_ptr /*reader*/, const DDS::RequestedIncompatibleQosStatus& status)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("on_requested_incompatible_qos()\n");
  }
  if (_monitor) {
    cout  << "RequestedIncompatibleQosStatus\n"
          << "Total count           : " << status.total_count             << "\n"
          << "Total count change    : " << status.total_count_change      << "\n"
          << "Last Policy id        : " << status.last_policy_id          << "\n";
  }
}

void VehicleDataReaderListenerImpl::on_sample_rejected(
  DDS::DataReader_ptr /*reader*/, const DDS::SampleRejectedStatus& status)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("on_sample_rejected()\n");
  }
  if (_monitor) {
    cout  << "SampleRejectedStatus\n"
          << "Total count           : " << status.total_count             << "\n"
          << "Total count change    : " << status.total_count_change      << "\n"
          << "Last Reason           : " << status.last_reason             << "\n";
  }
}

void VehicleDataReaderListenerImpl::on_liveliness_changed(
  DDS::DataReader_ptr /*reader*/, const DDS::LivelinessChangedStatus& status)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("on_liveliness_changed()\n");
  }
  if (_monitor) {
    cout  << "LivelinessChangedStatus\n"
          << "Alive count           : " << status.alive_count             << "\n"
          << "Alive count change    : " << status.alive_count_change      << "\n"
          << "Not Alive count       : " << status.not_alive_count         << "\n"
          << "Not Alivecount change : " << status.not_alive_count_change  << "\n";
  }
}

void VehicleDataReaderListenerImpl::on_subscription_matched(
  DDS::DataReader_ptr /*reader*/, const DDS::SubscriptionMatchedStatus& status)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("on_subscription_matched()\n");
  }
  if (_monitor) {
    cout  << "SubscriptionMatchedStatus\n"
          << "Total count         : " << status.total_count           << "\n"
          << "Total count change  : " << status.total_count_change    << "\n"
          << "Current count       : " << status.current_count         << "\n"
          << "Current count change: " << status.current_count_change  << "\n";
  }
}

void VehicleDataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr /*reader*/, const DDS::SampleLostStatus& status)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("on_sample_lost()\n");
  }
  if (_monitor) {
    cout  << "SampleLostStatus\n"
          << "Total count       : " << status.total_count           << "\n"
          << "Total count change: " << status.total_count_change    << "\n";
  }
}

void VehicleDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
  try {
    Vehicle::DataDataReader_var reader_i = Vehicle::DataDataReader::_narrow(reader);

    if (!reader_i) {
      WriteOutput("on_data_available() - _narrow data failed!\n");
      //ACE_OS::exit(-1);
      return;
    }

    /**
     * @brief Use the following code to Monitor via vehicleDataSeq
     */

    if (_monitor)
    {
      Vehicle::DataSeq    vehicleDataSeq;
      DDS::SampleInfoSeq  infoSeq;
      DDS::ReturnCode_t   errSeq(DDS::RETCODE_OK);

      errSeq = reader_i->take(vehicleDataSeq, infoSeq, DDS::LENGTH_UNLIMITED,
                              DDS::ANY_SAMPLE_STATE, DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);
      if (errSeq == DDS::RETCODE_OK)
      {
        for (size_t i(0); i < vehicleDataSeq.length(); ++i)
        {
          if (infoSeq[i].valid_data == 1)
          {
            Vehicle::Data d(vehicleDataSeq[i]);
            cout << "Data[" << i << "]::" << &d << "; D:==> " << d.name << " index:" << d.index << "\n";
          } else {
            cout << "Data[" << i << "] not valid\n";
          }
        }
      }

      errSeq = reader_i->return_loan(vehicleDataSeq, infoSeq);
      if (errSeq != DDS::RETCODE_OK)
      {
        cout << "return loan error " << errSeq << "\n";
      }
//      return;
    }

    // ---------------------------------------------------------
    // NO MONITORING: Normal processing
    // ---------------------------------------------------------
    Vehicle::Data    vehicleData;
    DDS::SampleInfo  info;

    DDS::ReturnCode_t error = reader_i->take_next_sample(vehicleData, info);

    // -------------------------------------------------
    // Received DATA
    // -------------------------------------------------
    if (error == DDS::RETCODE_OK)
    {
      string time_received = DDSCOMMON::Time2IsoString();
      if (_infolevel & DDSCOMMON::DEBUG::INFO)
      {
        string ss;
        ss.append("RECEIVED DATA at ").append(time_received).append("\n");
        WriteOutput(ss);
      }

      if (info.valid_data == 1)
      {
        if (_infolevel & DDSCOMMON::DEBUG::VALUES) {
          string ss;
          ss.append("Name = ").append(vehicleData.name).append("\n");
          WriteOutput(ss);
        }

        if (_dataFunc) {
          struct VEHICLEIDL::Data dataStruct;

          dataStruct.index       = vehicleData.index;
          dataStruct.type        = static_cast<VEHICLEIDL::VehicleEnum>(vehicleData.type);

          dataStruct.ip          = vehicleData.ip;
          dataStruct.ip_port     = vehicleData.ip_port;
          dataStruct.uuid        = vehicleData.uuid;

          dataStruct.velocity    = vehicleData.velocity;
          dataStruct.acceleration= vehicleData.acceleration;
          dataStruct.latitude    = vehicleData.latitude;
          dataStruct.longitude   = vehicleData.longitude;
          dataStruct.altitude    = vehicleData.altitude;

          strncpy(dataStruct.name, vehicleData.name, Vehicle::VEHICLE_NAME_LEN - 1);
          dataStruct.name[Vehicle::VEHICLE_NAME_LEN - 1] = '\0';

          _dataFunc(dataStruct);
        }
      }
      else if (info.instance_state == DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
      {
         WriteOutput("Instance is disposed\n");
      }
      else if (info.instance_state == DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
      {
         string ss;
         ss.append("ERROR: received unknown instance state ")
            .append(to_string(info.instance_state)).append("\n");
         WriteOutput(ss);
      }
      else
      {
         string ss;
         ss.append("ERROR: received unknown instance state ")
            .append(to_string(info.instance_state)).append("\n");
         WriteOutput(ss);
      }

    } else {
      if (_infolevel & DDSCOMMON::DEBUG::INFO)
      {
        WriteOutput("ERROR: on_data_available() - take_next_sample message failed!\n");
      }
    }
  }
  catch(...)
  {
    WriteOutput("EXCEPTION: on_data_available() - failed!\n");
  }
}

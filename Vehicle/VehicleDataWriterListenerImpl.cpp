/*
 * File:   VehicleDataWriterListenerImpl.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include <iostream>


#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

#include "VehicleDataWriterListenerImpl.hpp"

#include "../DDSCommon/DDSHelper.hpp"

#include "../_idls/Vehicle/generated/VehicleTypeSupportC.h"
#include "../_idls/Vehicle/generated/VehicleTypeSupportImpl.h"

VehicleDataWriterListenerImpl::VehicleDataWriterListenerImpl(
    CBF_VEHICLEDATA dataFunc,
    DDSCOMMON::OUTPUTFUNC debugFunc, int infolevel, bool monitor)
  : _dataFunc(dataFunc),
    _debugFunc(debugFunc),
    _infolevel(infolevel), _monitor(monitor)
{
}

VehicleDataWriterListenerImpl::~VehicleDataWriterListenerImpl(){}

void VehicleDataWriterListenerImpl::WriteOutput(const string& m)
{
  if (_debugFunc) _debugFunc(m.c_str());
}
void VehicleDataWriterListenerImpl::WriteOutput(int value)
{
  if (_debugFunc) _debugFunc(to_string(value).c_str());
}


void VehicleDataWriterListenerImpl::on_offered_deadline_missed(DDS::DataWriter_ptr /* writer */, const DDS::OfferedDeadlineMissedStatus& /* status */ )
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("DATA WRITER_LISTENER: on_offered_deadline_missed()\n");
  }
}

void VehicleDataWriterListenerImpl::on_offered_incompatible_qos(DDS::DataWriter_ptr /* writer */, const DDS::OfferedIncompatibleQosStatus& /* status */)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("DATA WRITER_LISTENER: on_offered_incompatible_qos()\n");
  }
}

void VehicleDataWriterListenerImpl::on_liveliness_lost(DDS::DataWriter_ptr /* writer */, const DDS::LivelinessLostStatus& /* status */)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("DATA WRITER_LISTENER: on_liveliness_lost()\n");
  }
}

void VehicleDataWriterListenerImpl::on_publication_matched(DDS::DataWriter_ptr /* writer */, const DDS::PublicationMatchedStatus& /* status */)
{
  if (_infolevel & DDSCOMMON::DEBUG::INFO)
  {
    WriteOutput("DATA WRITER_LISTENER: on_publication_matched()\n");
  }
}

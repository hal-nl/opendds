/*
 * File:   VehicleDataWriterListenerImpl.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */


#ifndef VEHICLEDATAWRITERLISTENERIMPL_HPP
#define VEHICLEDATAWRITERLISTENERIMPL_HPP


#include <ace/Global_Macros.h>

#include <dds/DdsDcpsPublicationC.h>
#include <dds/DCPS/LocalObject.h>
#include <dds/DCPS/Definitions.h>

#include "Vehicle_global.hpp"

class VehicleDataWriterListenerImpl : public virtual OpenDDS::DCPS::LocalObject<DDS::DataWriterListener>
{
public:
  VehicleDataWriterListenerImpl(CBF_VEHICLEDATA dataFunc, DDSCOMMON::OUTPUTFUNC debugFunc, int infolevel, bool monitor);
  virtual ~VehicleDataWriterListenerImpl();

  virtual void on_offered_deadline_missed (DDS::DataWriter_ptr writer, const DDS::OfferedDeadlineMissedStatus& status);
  virtual void on_offered_incompatible_qos(DDS::DataWriter_ptr writer, const DDS::OfferedIncompatibleQosStatus& status);
  virtual void on_liveliness_lost         (DDS::DataWriter_ptr writer, const DDS::LivelinessLostStatus& status);
  virtual void on_publication_matched     (DDS::DataWriter_ptr writer, const DDS::PublicationMatchedStatus& status);

private:
  CBF_VEHICLEDATA                    _dataFunc;
  DDSCOMMON::OUTPUTFUNC  _debugFunc;
  int                                _infolevel;
  bool                               _monitor;

  void WriteOutput(const string& m);
  void WriteOutput(int value);
};

#endif /* VEHICLEDATAWRITERLISTENERIMPL_HPP */

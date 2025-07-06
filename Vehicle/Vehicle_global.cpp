/*
 * File:   Vehicle_global.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#include <stdlib.h>
#include <iostream>

#include "Vehicle_global.hpp"
#include "Vehicle_local.hpp"

#include "../DDSCommon/DDSCommon.hpp"
#include "../DDSCommon/DDSHelper.hpp"

#include "../_idls/Vehicle/VehicleIdl.hpp"

#ifdef LINUX
  void OnDLLInit(void) {/*<TODO: Do something on DLL initialize>*/ }
  void OnDLLTerm(void) {/*<TODO: Do something on DLL terminate>*/ }
#else
  // dllmain.cpp : Defines the entry point for the DLL application.
  BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
  {
    switch (ul_reason_for_call)
    {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
  }
#endif

// ----------------------------------------------------------------------------------------
// External Wrapper functions
// ----------------------------------------------------------------------------------------

int VehicleDataCreate(DDSCOMMON::OUTPUTFUNC debugFunc, int argc, char* argv[])
{
   try {
      int retvalue = vehicleDataCreate(debugFunc, argc, argv);
      return retvalue;
   } catch (...) {
      vehicleWriteOutput("Exception in VehicleDataCreate.\n");
      return -1;
   }
   return 0;
}

int VehicleDataCreateArgv(DDSCOMMON::OUTPUTFUNC debugFunc, char* arguments)
{
  try {
    int retvalue = vehicleDataCreateArgv(debugFunc, arguments);
    return retvalue;
  } catch (...) {
    vehicleWriteOutput("Exception in VehicleDataCreateArgv.\n");
    return -1;
  }
  return 0;
}


int VehicleDataSend(const struct VEHICLEIDL::Data& dataStruct)
{
  int retValue(0);
  try {
    retValue = vehicleDataSend(dataStruct);
  } catch (...) {
    vehicleWriteOutput("Exception in VehicleSendData.\n");
    return -1;
  }
  return retValue;
}


int VehicleDataReaderListener(CBF_VEHICLEDATA readerFunc, DDSCOMMON::OUTPUTFUNC debugFunc)
{
  int retValue(0);
  try {
    retValue = vehicleDataReaderListener(readerFunc, debugFunc);
  } catch (...) {
    vehicleWriteOutput("Exception in VehicleDataReaderListener.\n");
    return -1;
  }
  return retValue;
}

int VehicleDataWriterListener(CBF_VEHICLEDATA writerFunc, DDSCOMMON::OUTPUTFUNC debugFunc)
{
  int retValue(0);
  try {
    retValue = vehicleDataWriterListener(writerFunc, debugFunc);
  } catch (...) {
    vehicleWriteOutput("Exception in VehicleDataWriterListener.\n");
    return -1;
  }
  return retValue;
}


int VehicleDataDestroy(void)
{
  try {
    int retValue = vehicleDataDestroy();
    return retValue;
  } catch (...) {
    vehicleWriteOutput("Exception in VehicleDataDestroy.\n");
    return -1;
  }
  return 0;
}

/*
 * File:   Vehicle_global.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */

#ifndef VEHICLE_GLOBAL_HPP
#define VEHICLE_GLOBAL_HPP


#include "ObjectVisibility.hpp"
#include "../DDSCommon/DDSCommon.hpp"
#include "../_idls/Vehicle/VehicleIdl.hpp"

#ifdef LINUX
  DLL_LOCAL void OnDLLInit(void) __attribute__((constructor));
  DLL_LOCAL void OnDLLTerm(void) __attribute__((destructor));
#else
  // Windows
#endif


// ----------------------------------------------------------------------------------------
// External function NAMES For the Publisher/Subscriber Part
// ----------------------------------------------------------------------------------------
static string NAME_VEHICLEDATACREATE         = ("VehicleDataCreate");
static string NAME_VEHICLEDATACREATEARGV     = ("VehicleDataCreateArgv");
static string NAME_VEHICLEDATASEND           = ("VehicleDataSend");
static string NAME_VEHICLEDATAREADERLISTENER = ("VehicleDataReaderListener");
static string NAME_VEHICLEDATAWRITERLISTENER = ("VehicleDataWriterListener");
static string NAME_VEHICLEDATADESTROY        = ("VehicleDataDestroy");

// ----------------------------------------------------------------------------------------
// Callback types
// ----------------------------------------------------------------------------------------
using CBF_VEHICLEDATA = void (*)(const struct VEHICLEIDL::Data& dataStruct);

EXTERN_DLL_FUNC int VehicleDataCreate      (DDSCOMMON::OUTPUTFUNC debugFunc, int argc, char* argv[]);
EXTERN_DLL_FUNC int VehicleDataCreateArgv  (DDSCOMMON::OUTPUTFUNC debugFunc, char* arguments);
EXTERN_DLL_FUNC int VehicleDataSend        (const struct VEHICLEIDL::Data& dataStruct);
EXTERN_DLL_FUNC int VehicleDataReaderListener(CBF_VEHICLEDATA readerFunc, DDSCOMMON::OUTPUTFUNC debugFunc);
EXTERN_DLL_FUNC int VehicleDataWriterListener(CBF_VEHICLEDATA writerFunc, DDSCOMMON::OUTPUTFUNC debugFunc);
EXTERN_DLL_FUNC int VehicleDataDestroy     (void);

// ----------------------------------------------------------------------------------------
// External function TYPES in the Publisher Part
// ----------------------------------------------------------------------------------------
using VEHICLEDATASENDFUNC           = int(*)(const struct VEHICLEIDL::Data& dataStruct);
using VEHICLEDATAREADERLISTENERFUNC = int(*)(CBF_VEHICLEDATA readerFunc, DDSCOMMON::OUTPUTFUNC debugFunc);
using VEHICLEDATAWRITERLISTENERFUNC = int(*)(CBF_VEHICLEDATA writerFunc, DDSCOMMON::OUTPUTFUNC debugFunc);

#endif /* VEHICLE_GLOBAL_HPP */


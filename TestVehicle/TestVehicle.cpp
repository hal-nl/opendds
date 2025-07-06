/*
 * File:   TestVehicle.cpp
 *
 * Author: Haluk Ates
 *
 * Created 2021-04-01
 */

#include <unistd.h>
#include <dlfcn.h>

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
#include <map>
#include <cctype>
#include <random>

#include <boost/program_options.hpp>

#include "../../DDSCommon/DDSCommon.hpp"
#include "../../DDSCommon/DDSHelper.hpp"
#include "../../_idls/Vehicle/VehicleIdl.hpp"
#include "../Vehicle/Vehicle_global.hpp"

using namespace std;

using u32     = uint_least32_t;
using engine  = std::mt19937;

const float POS_MIN = 0.0;
const float POS_MAX = 100.0;

void infoCallBack(const char* data)
{
   cout << data;
}

void readerCallback(const struct VEHICLEIDL::Data& data)
{
   cout  << "READER RECEIVED From: " << data.name << " index[" << data.index << "]; Speed: " << data.velocity << "\n";
}

void writerCallback(const struct VEHICLEIDL::Data& data)
{
   cout  << "WRITER RECEIVED From: " << data.name << " index[" << data.index << "]; Speed: " << data.velocity << "\n";
}

static struct VEHICLEIDL::Data      vehicleData;

static void * handle = nullptr;

static DDSCOMMON::CREATEFUNC           VehicleDataCreateFunc          = nullptr;
static DDSCOMMON::CREATEARGVFUNC       VehicleDataCreateArgvFunc      = nullptr;
static DDSCOMMON::DESTROYFUNC          VehicleDataDestroyFunc         = nullptr;

static VEHICLEDATAREADERLISTENERFUNC   VehicleDataReaderListenerFunc  = nullptr;
static VEHICLEDATAWRITERLISTENERFUNC   VehicleDataWriterListenerFunc  = nullptr;

static VEHICLEDATASENDFUNC             VehicleDataSendFunc        = nullptr;

int LoadFunctions(void)
{
  handle = dlopen ("./libVehicle.so", /*RTLD_NOW | RTLD_GLOBAL*/   RTLD_LAZY );
  if (!handle) {
    cout << dlerror() << "\n";
    return 1;
  }

  VehicleDataCreateFunc          = reinterpret_cast<DDSCOMMON::CREATEFUNC>       (dlsym(handle, NAME_VEHICLEDATACREATE.c_str()));
  VehicleDataCreateArgvFunc      = reinterpret_cast<DDSCOMMON::CREATEARGVFUNC>   (dlsym(handle, NAME_VEHICLEDATACREATEARGV.c_str()));
  VehicleDataDestroyFunc         = reinterpret_cast<DDSCOMMON::DESTROYFUNC>      (dlsym(handle, NAME_VEHICLEDATADESTROY.c_str()));

  VehicleDataSendFunc            = reinterpret_cast<VEHICLEDATASENDFUNC>            (dlsym(handle, NAME_VEHICLEDATASEND.c_str()));
  VehicleDataReaderListenerFunc  = reinterpret_cast<VEHICLEDATAREADERLISTENERFUNC>  (dlsym(handle, NAME_VEHICLEDATAREADERLISTENER.c_str()));
  VehicleDataWriterListenerFunc  = reinterpret_cast<VEHICLEDATAWRITERLISTENERFUNC>  (dlsym(handle, NAME_VEHICLEDATAWRITERLISTENER.c_str()));

  if (!VehicleDataCreateFunc || !VehicleDataCreateArgvFunc || !VehicleDataDestroyFunc || !VehicleDataSendFunc ||
      !VehicleDataReaderListenerFunc || !VehicleDataWriterListenerFunc)
  {
    if (!VehicleDataCreateFunc) {
      cout << "Could not find function VehicleDataCreate\n";
      return -1;
    }
    if (!VehicleDataCreateArgvFunc) {
      cout << "Could not find function VehicleDataCreateArgv\n";
      return -1;
    }
    if (!VehicleDataDestroyFunc)  {
      cout << "Could not find function VehicleDataDestroy\n";
      return -1;
    }
    if (!VehicleDataSendFunc)  {
      cout << "Could not find function VehicleDataSend\n";
      return -1;
    }
    if (!VehicleDataReaderListenerFunc)
    {
      cout << "Could not find function VehicleDataReaderListenerFunc\n";
      return -1;
    }
    if (!VehicleDataWriterListenerFunc)
    {
      cout << "Could not find function VehicleDataWriterListenerFunc\n";
      return -1;
    }
  }

  return 0;
}

/**
 * Update value: between [minimal..maximal] by random [0..delta] plus/min
 */
float UpdateValue (float initialValue, float delta, float minimal, float maximal)
{
   float value = 0.0;
   if (initialValue <= minimal)
   {
      value = DDSCOMMON::GetRandomValue(1.0, 1.0 + delta);
   } else if (initialValue >= maximal){
      value = DDSCOMMON::GetRandomValue(-delta - 1.0, -1.0);
   } else {
      value = DDSCOMMON::GetRandomValue(-delta, delta);
   }
   initialValue += value;
   return initialValue;
}

/**
 * Update the position/velocity of the vehicle by random values
 */
void UpdateRandomData(void)
{
   float deltaPosition = 5.0;
   float deltaVelocity = 1.0;

   // Update velocity
   vehicleData.velocity = UpdateValue (vehicleData.velocity, deltaVelocity, 0.0, 130.0);

   // Update latitude
   vehicleData.latitude = UpdateValue (vehicleData.latitude, deltaPosition, POS_MIN, POS_MAX);

   // Update longitude
   vehicleData.longitude= UpdateValue (vehicleData.longitude, deltaPosition, POS_MIN, POS_MAX);
}

/**
 * @brief GetFrequency
 * Get frequency parameter from argv if exists, else set to 1.0
 * meaning once a second an update
 * @param argc
 * @param argv
 * @return
 */
double GetFrequency(int argc, char *argv[])
{
   double frequency = 1.0; // once per second
   map<string, string> parameters;
   DDSCOMMON::GetApplicationArguments(argc, const_cast<const char**>(argv), parameters);
   if (parameters.find(DDSCOMMON::PARAMETERS::FREQUENCY) != parameters.end()) {
     if (!parameters[DDSCOMMON::PARAMETERS::FREQUENCY].empty()) {
       try {
         frequency = std::stod(parameters[DDSCOMMON::PARAMETERS::FREQUENCY]);
         frequency = fabs(frequency);
       }
       catch (...) { /* do nothing*/ }
     }
   }
   return frequency;
}

int GetArgumentValues(string& argumentstr, int argc, char *argv[])
{
    // Set the default arguments:
    string helpText("This help message.");

    string  DCPSInfoRepo   = "127.0.0.1:12345";
    string  ORBLogFile     = string(argv[0]) + ".ORBLogFile.log" ;
    int     ORBDebugLevel  = 0;
    string  ddsconfig      = "";
    bool    monitor        = false;
    string  name           = "";
    int     domain         = VEHICLEIDL::DOMAIN;
    float   frequency      = 1.0;

    /* Define and parse the program's command line options via boost */
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()
      ("help,h", helpText.c_str())

      ("DCPSInfoRepo,I",
            po::value< string >(&DCPSInfoRepo)->default_value("127.0.0.1:12345"),
            "DCPS Info Repo")
      ("ORBLogFile,O",
            po::value< string >(&ORBLogFile),
            "ORB Log File name. Default: ending on '*<timestamp>.ORBLogFile.log'")
      ("ORBDebugLevel,D",
            po::value< int >(&ORBDebugLevel)->default_value(0),
            "ORB debug level (max 10)")
      ("ddsconfig,c",
            po::value< string >(&ddsconfig)->default_value("./VehicleDDS.xml"),
            "DDS Configuration file specific for the current Vehicle.")
      ("monitor,m",
            po::value< bool >(&monitor)->default_value(false),
            "If monitoring is enabled then extra debug info is shown in terminal.")
      ("name,n",
            po::value< string >(&name)->default_value(""),
            "Name of the Vehicle. Defaults to empty name")
      ("domain,d",
            po::value< int >(&domain)->default_value(VEHICLEIDL::DOMAIN),
            "Domain number: Number between 100 and 200")
      ("frequency,f",
            po::value< float >(&frequency)->default_value(1.0f),
            "Frequency of publishing: Positive non zero float value. defaults to 1.0 ")
        ;

    po::variables_map vm;
    try {
      po::store(po::parse_command_line(argc, argv, desc), vm); // can throw

      if ( vm.count("help")  ) {
        std::cout << "TestVehicle:" << std::endl << desc << std::endl;
        return 1;
      }
      po::notify(vm); // throws on error, so do after help in case there are any problems
    } catch(po::error& e) {
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
      std::cerr << desc << std::endl;
      return -1;
    }

   // ---------------------------------------------- //
   // --- Absolutely necessary parameters check: --- //
   // ---------------------------------------------- //
   if (frequency <=0 ) {
      frequency = 1.0;
   }

   if ((domain < 100) || (domain > 200) ) {
      domain = VEHICLEIDL::DOMAIN;
   }

   cout << "Testing Vehicle (Pub/Sub)\n";

   cout << "Info Repo:\t" << DCPSInfoRepo << "\n";
   cout << "ORB Log file:\t" << ORBLogFile << "\n";
   cout << "ORB Debug level:" << ORBDebugLevel << "\n";
   cout << "ddsconfig file:\t" << ddsconfig << "\n";
   cout << "monitor:\t" << monitor << "\n";
   cout << "name:\t\t" << name << "\n";
   cout << "domain:\t\t" << domain << "\n";
   cout << "frequency:\t" << frequency << "\n";

   string arguments;
   arguments.append("-DCPSInfoRepo ").append(DCPSInfoRepo).append(" ");
   arguments.append("-ORBDebugLevel ").append(to_string(ORBDebugLevel)).append(" ");
   arguments.append("-ORBLogFile ").append(ORBLogFile).append(" ");
   arguments.append("--ddsconfig=").append(ddsconfig).append(" ");
   arguments.append("--monitor=").append(to_string(monitor)).append(" ");
   arguments.append("--name=").append(name).append(" ");
   arguments.append("--domain=").append(to_string(domain)).append(" ");
   arguments.append("--frequency=").append(to_string(frequency)).append(" ");

   argumentstr = arguments;
   return 0;
}

int main(int argc, char *argv[])
{
   string argumentString;
   int retValue = GetArgumentValues(argumentString, argc, argv);
   if (retValue) return retValue;

   // ---------------------------------------------- //
   // -------- Application code comes here --------- //
   // ---------------------------------------------- //
   /**
    * Try to load the external library with Vehicle publisher/subscriber
    */
   if (LoadFunctions()) return -1;


  /**
   * Try to initialize and create the Vehicle publisher/subscriber
   * The DDS arguments start with one '-' and the application arguments are
   * starting with two '--', so need to use 'VehicleCreateArgvFunc'
   * instead of 'VehicleCreateFunc(infoCallBack, argc, argv)'
   */
   retValue = VehicleDataCreateArgvFunc(infoCallBack, const_cast<char*>(argumentString.c_str()));
   if (!retValue)
   {
      retValue = VehicleDataReaderListenerFunc(readerCallback, infoCallBack);
      retValue = VehicleDataWriterListenerFunc(writerCallback, infoCallBack);

      // Initialize location with random value
      vehicleData.latitude  = DDSCOMMON::GetRandomValue(POS_MIN, POS_MAX);
      vehicleData.longitude = DDSCOMMON::GetRandomValue(POS_MIN+10.0, POS_MAX-10.0);

      // Set frequency
      double frequency = GetFrequency(argc, argv);

      // Simulate Running Vehicle. Update each frequency.
      double sleepSeconds = (frequency != 0.0) ? 1/fabs(frequency) : 1.0;

      // Listening and sending data while alive (running)
      while (true)
      {
         UpdateRandomData();
         VehicleDataSendFunc(vehicleData);
         sleep(sleepSeconds);
      }
      VehicleDataDestroyFunc();
  }

  dlclose(handle);
  return 0;
}

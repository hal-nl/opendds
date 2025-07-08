/*
 * File:   DDSCommon.hpp
 *
* Created on July 2025
 *
 */

#ifndef DDSCOMMON_HPP
#define DDSCOMMON_HPP

using namespace std;

#define EXTERN_DLL_FUNC extern "C" DLL_PUBLIC

// ------------------------------------------------------------------------------------------
// DDSCOMMON Namespace
// ------------------------------------------------------------------------------------------

/**
 * The DDSCOMMON OpenDDS namespace
 */
namespace DDSCOMMON
{
   /** Callback function for debug ouotput */
   using OUTPUTFUNC = void (*)(const char* data);

   using CREATEFUNC     = int (*)(DDSCOMMON::OUTPUTFUNC debugFunc, int argc, char *argv[]);
   using CREATEARGVFUNC = int (*)(DDSCOMMON::OUTPUTFUNC debugFunc, char *arguments);
   using DESTROYFUNC    = int (*)(void);

  /**
   * The DEBUG namespace is used for informational purposes
   */
  namespace DEBUG
  {
    extern const int NOTHING;    // 0
    extern const int INFO;       // 1
    extern const int VALUES;     // 2
    extern const int QOSINFO;    // 4

    extern const int ALL;        // 7
  }
  /**
   * The constants used in OpenDDS Publishers/Subscribers
   */
  namespace CONSTANTS
  {
    /**
     * OpenDDS DOMAIN ID Constants.
     * IDs should be between 0..231
     */
    namespace DOMAINS
    {
      extern const int   REAL;      // 16
      extern const int   OBSERVED;  // 32
      extern const int   TEST;      // 48
      extern const int   CONTROL;   // 64

      /** Domain id for static/dynamic data, */
      extern const int   STATIC;    // 80
      extern const int   DYNAMIC;   // 96
      extern const int   MIN_DOMAIN;// 112
      extern const int   MAX_DOMAIN;// 231
    };
  };

  /**
   * Parameter names etc
   */
  namespace PARAMETERS
  {
    /** Configuration */
    extern const char* DDSCONFIG;

    /** Name */
    extern const char* NAME;

    /** Infolevel */
    extern const char* INFOLEVEL;

    /** Monitor */
    extern const char* MONITOR;

    /** Public key file */
    extern const char* PUBKEYFILE;

    /** Domain parameter */
    extern const char* DOMAIN;

    /** Publisher Domain parameter */
    extern const char* PUBDOMAIN;
    /** Subscriber Domain parameter */
    extern const char* SUBDOMAIN;

    /** Relay domain parameter */
    extern const char* RELAY;

    /** Frequency parameter */
    extern const char* FREQUENCY;

    /** Topic Name parameter */
    extern const char* TOPICNAME;

    /** Input file */
    extern const char* INPUTFILE;

    /** Receiver (=yes) (no=Sender) */
    extern const char* RECEIVER;
  }
};

#endif /* DDSCOMMON_HPP */


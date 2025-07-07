/*
 * File:   DDSCommon.cpp
 *
* Created on July 2025
 *
 */

#include "DDSCommon.hpp"

namespace DDSCOMMON
{
  /**
   * The DEBUG namespace is used for informational purposes
   */
  namespace DEBUG
  {
    const int  NOTHING(0);
    const int  INFO   (1);
    const int  VALUES (2);
    const int  QOSINFO(4);

    const int  ALL    (7);
  }

  namespace CONSTANTS
  {
    /**
     * OpenDDS DOMAIN ID Constants.
     * IDs should be between 0..231
     */
    namespace DOMAINS
    {
      const int   REAL        (16);
      const int   OBSERVED    (32);
      const int   TEST        (48);
      const int   CONTROL     (64);

      /** Domain id for static/dynamic data, */
      const int   STATIC      (80);
      const int   DYNAMIC     (96);
      const int   MIN_DOMAIN  (112);
      const int   MAX_DOMAIN  (231);
    };
  };

  namespace PARAMETERS
  {
    /** Configuration */
    const char* DDSCONFIG   ("ddsconfig");

    /** Name */
    const char* NAME        ("name");

    /** Infolevel */
    const char* INFOLEVEL   ("infolevel");

    /** Monitor */
    const char* MONITOR     ("monitor");

    /** Public key file */
    const char* PUBKEYFILE  ("pubkeyfile");

    /** Domain parameter */
    const char* DOMAIN      ("domain");

    /** Publisher Domain parameter */
    const char* PUBDOMAIN   ("pubdomain");
    /** Subscriber Domain parameter */
    const char* SUBDOMAIN   ("subdomain");

    /** Relay domain parameter */
    const char* RELAY       ("relay");

    /** Frequency parameter */
    const char* FREQUENCY   ("frequency");

    /** Topic Name parameter */
    const char* TOPICNAME   ("topicname");

    /** Input file */
    const char* INPUTFILE   ("inputfile");

    /** Receiver (=yes) (no=Sender) */
    const char* RECEIVER    ("receiver");

  };

};


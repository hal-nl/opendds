cmake_minimum_required (VERSION 3.22)
project(DDS)

# #################################################################################################
# Global output directories
# #################################################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libStatic/ )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/ )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ )


# #################################################################################################
# SET C++ VERSION
# #################################################################################################
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VSC_PROJECT_ROOT_DIR              .            )
set(PROJECT_INCLUDE_VISIBILITY        ./include    )


# #################################################################################################
# SET C++ VERSION
# #################################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# #################################################################################################
# ADD MACROS
# #################################################################################################
add_definitions(
  -DLINUX
)


# #################################################################################################
# Environment settings for DDS
# #################################################################################################
set (DDS_VERSION $ENV{DDS_VERSION})
set (DDS_ROOT $ENV{DDS_ROOT})
set (ACE_ROOT $ENV{ACE_ROOT})
set (TAO_ROOT $ENV{TAO_ROOT})
set (MPC_ROOT $ENV{MPC_ROOT})


# #################################################################################################
# INCLUDE HEADER DIRECTORIES
# #################################################################################################
include_directories(
   ${DDS_ROOT}
   ${ACE_ROOT}
   ${TAO_ROOT}
   ${PROJECT_INCLUDE_VISIBILITY}
)

# #################################################################################################
# INCLUDE LINK DIRECTORIES
# #################################################################################################
link_directories(
    ${DDS_ROOT}/lib/
    ${ACE_ROOT}/lib/
)

# #################################################################################################
# DISTRIBUTED FILES
# #################################################################################################
file(COPY ./scripts/DCPSInfoRepo.bash                    DESTINATION ${CMAKE_BINARY_DIR}/bin/)
file(COPY ./scripts/start_all.bash                       DESTINATION ${CMAKE_BINARY_DIR}/bin/)
file(COPY ./scripts/stop_all.bash                        DESTINATION ${CMAKE_BINARY_DIR}/bin/)
file(COPY ./python/VehicleMonitor/VehicleMonitor.png     DESTINATION ${CMAKE_BINARY_DIR}/bin/)
file(COPY ./python/VehicleMonitor/VehicleMonitor.py      DESTINATION ${CMAKE_BINARY_DIR}/bin/)
file(COPY ./python/VehicleMonitor/VehicleMonitor.bash    DESTINATION ${CMAKE_BINARY_DIR}/bin/)

set(DDS_LIBRARIES
    OpenDDS_Dcps
    OpenDDS_FACE
    OpenDDS_Federator
    OpenDDS_InfoRepoDiscovery
    OpenDDS_InfoRepoLib
    OpenDDS_InfoRepoServ
    OpenDDS_Model
    OpenDDS_Multicast
    OpenDDS_Rtps
    OpenDDS_Rtps_Udp
    OpenDDS_Shmem
    OpenDDS_Tcp
    OpenDDS_Udp
    OpenDDS_monitor
    ACE
    TAO
    TAO_AnyTypeCode
    TAO_Async_IORTable
    TAO_CSD_Framework
    TAO_CSD_ThreadPool
    TAO_CodecFactory
    TAO_Codeset
    TAO_DynamicInterface
    TAO_IDL_BE
    TAO_IDL_FE
    TAO_IORManip
    TAO_IORTable
    TAO_ImR_Client
    TAO_Messaging
    TAO_PI
    TAO_PortableServer
    TAO_Svc_Utils
    TAO_Valuetype
)


# #############################################################################
# DDS Libraries
# #############################################################################
add_subdirectory(DDSCommon)
add_subdirectory(DDSQoS)
add_subdirectory(Vehicle)
add_subdirectory(TestVehicle)

cmake_minimum_required (VERSION 3.22)

project(Vehicle)

# #################################################################################################
# ADD MACROS
# #################################################################################################
add_definitions(
  -DLINUX
)

# #################################################################################################
# SET C++ FLAGS
# #################################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated ")


# #################################################################################################
# CXX Standard
# #################################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")


# #################################################################################################
# Find fmt
# #################################################################################################
find_package(fmt REQUIRED)


# #################################################################################################
# Find Boost
# #################################################################################################
find_package(Boost COMPONENTS
  regex system filesystem date_time REQUIRED
)
include_directories(
   ${Boost_INCLUDE_DIRS}
   ${PROJECT_INCLUDE_VISIBILITY}
)


# #################################################################################################
# DECLARE ALL THE USED SOURCES
# #################################################################################################
set(SOURCES
    Vehicle_global.cpp
    Vehicle_local.cpp
    VehicleDataReaderListenerImpl.cpp
    VehicleDataWriterListenerImpl.cpp
    ../DDSCommon/DDSHelper.cpp
    ../DDSCommon/DDSCommon.cpp
    ../DDSQoS/ddsqos.cpp
    ../DDSQoS/ddsqosDatareader.cpp
    ../DDSQoS/ddsqosDatawriter.cpp
    ../DDSQoS/ddsqosParticipant.cpp
    ../DDSQoS/ddsqosPublisher.cpp
    ../DDSQoS/ddsqosSubscriber.cpp
    ../DDSQoS/ddsqosTopic.cpp
    ../_idls/Vehicle/generated/VehicleC.cpp
    ../_idls/Vehicle/generated/VehicleC.inl
    ../_idls/Vehicle/generated/VehicleS.cpp
    ../_idls/Vehicle/generated/VehicleTypeSupportC.cpp
    ../_idls/Vehicle/generated/VehicleTypeSupportC.inl
    ../_idls/Vehicle/generated/VehicleTypeSupportImpl.cpp
    ../_idls/Vehicle/generated/VehicleTypeSupportS.cpp
)

# #################################################################################################
# CREATE STATIC / SHARED LIBRARIES
# #################################################################################################
add_library( ${PROJECT_NAME}Static STATIC ${SOURCES})
add_library( ${PROJECT_NAME} SHARED ${SOURCES})


# #################################################################################################
# DISTRIBUTED FILES
# #################################################################################################
file(COPY ../_idls/Vehicle/Vehicle.idl    DESTINATION  ${CMAKE_BINARY_DIR}/bin/)
file(COPY ../_idls/Vehicle/VehicleQoS.xml DESTINATION  ${CMAKE_BINARY_DIR}/bin/)
file(COPY ../_idls/Vehicle/VehicleDDS.xml DESTINATION  ${CMAKE_BINARY_DIR}/bin/)

# #################################################################################################
# LINK LIBRARIES TO TARGET
# #################################################################################################
target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  dl
  rt
  fmt
  ${DDS_LIBRARIES})

# #################################################################################################
# COPY TARGET TO BIN
# #################################################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/../CustomCommands4Libs.cmake)
CopyLibToBin()

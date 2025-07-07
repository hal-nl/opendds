cmake_minimum_required (VERSION 3.22)

project(DDSCommon)

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
    DDSCommon.cpp
    DDSInit.cpp
    DDSConfiguration.cpp
    DDSHelper.cpp
    DDSUDPClient.cpp
    QoSConfigurationDatareader.cpp
    QoSConfigurationDatawriter.cpp
    QoSConfigurationParticipant.cpp
    QoSConfigurationPublisher.cpp
    QoSConfigurationSubscriber.cpp
    QoSConfigurationTopic.cpp
    XMLConfiguration.cpp
)

# #################################################################################################
# CREATE STATIC / SHARED LIBRARIES
# #################################################################################################
add_library( ${PROJECT_NAME}Static STATIC ${SOURCES})
add_library( ${PROJECT_NAME} SHARED ${SOURCES})


# #################################################################################################
# DISTRIBUTED FILES
# #################################################################################################
file(COPY ./configuration/DDSConfiguration.xml DESTINATION  ${CMAKE_BINARY_DIR}/bin/)

# #################################################################################################
# LINK LIBRARIES TO TARGET
# #################################################################################################
target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  dl
  rt
  fmt
  ${DDS_LIBRARIES}
)

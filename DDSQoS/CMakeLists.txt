cmake_minimum_required (VERSION 3.22)

project(DDSQoS)

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
# CXX Standard
# #################################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")


# #################################################################################################
# Find fmt
# #################################################################################################
find_package(fmt REQUIRED)


# #################################################################################################
# DECLARE ALL THE USED SOURCES
# #################################################################################################
set(SOURCES
    ddsqos.cpp
    ddsqosDatareader.cpp
    ddsqosDatawriter.cpp
    ddsqosParticipant.cpp
    ddsqosPublisher.cpp
    ddsqosSubscriber.cpp
    ddsqosTopic.cpp
)

# #################################################################################################
# CREATE STATIC / SHARED LIBRARIES
# #################################################################################################
add_library( ${PROJECT_NAME}Static STATIC ${SOURCES})
add_library( ${PROJECT_NAME} SHARED ${SOURCES})


# #################################################################################################
# DISTRIBUTED FILES
# #################################################################################################
file(COPY configuration/ddsqos_default_configuration.xml  DESTINATION ${CMAKE_BINARY_DIR}/bin/)
file(COPY configuration/ddsqos_default_configuration.xsd  DESTINATION ${CMAKE_BINARY_DIR}/bin/)

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

# #################################################################################################
# COPY TARGET TO BIN
# #################################################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/../CustomCommands4Libs.cmake)
CopyLibToBin()

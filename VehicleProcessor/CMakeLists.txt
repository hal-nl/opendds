cmake_minimum_required (VERSION 3.22)

project(VehicleProcessor)

# #################################################################################################
# ADD MACROS
# #################################################################################################
add_definitions(
   -DEIGEN_MALLOC_ALREADY_ALIGNED=0
   -DLINUX
)

# #################################################################################################
# SET C++ FLAGS
# #################################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")


# #################################################################################################
# Find Boost
# #################################################################################################
find_package(Boost COMPONENTS
  program_options regex system filesystem date_time REQUIRED
)
include_directories(
   ${Boost_INCLUDE_DIRS}
)

# #################################################################################################
# Find OpenMP
# #################################################################################################
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


# #################################################################################################
# DISTRIBUTED FILES
# #################################################################################################
file(COPY resources/VehicleProcessor.pause.json DESTINATION ${CMAKE_BINARY_DIR}/bin/ )
file(COPY resources/VehicleProcessor.cfg        DESTINATION ${CMAKE_BINARY_DIR}/bin/ )

# #################################################################################################
# CREATE EXECUTABLE
# #################################################################################################
set (SOURCES
   vehicle_processor.cpp
   vehicle_processor_run.cpp
   vehicle_processor_notifiers.cpp
   control_notifiers.cpp
)
add_executable(${PROJECT_NAME}
   ${SOURCES}
)

# #################################################################################################
# LINK LIBRARIES TO TARGET
# #################################################################################################
target_link_libraries(${PROJECT_NAME}
   dl
   rt
   pthread
   fmt
   config++
   ${Boost_LIBRARIES}
   ${GCOV_LIB}
)

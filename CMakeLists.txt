#----------------------------------------------------------------------------
# Setup the project

cmake_minimum_required(VERSION 3.16...3.21)
project(OREOfs)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions

include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/processes_OC_modCS_old/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh
                  ${PROJECT_SOURCE_DIR}/processes_OC_modCS_old/include/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(OREOfs OREOfs.cc ${sources} ${headers})
target_include_directories(OREOfs PRIVATE include processes_OC_modCS_old/include)
target_link_libraries(OREOfs ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all macro/scripts to the build directory

foreach(_file ${macros})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
  )
endforeach()

set(my_SCRIPTS
  execute.sh
)
foreach(_script ${my_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX

install(TARGETS OREOfs DESTINATION bin)

#----------------------------------------------------------------------------
# Create directories in the binary path

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/output)

#----------------------------------------------------------------------------
# Copy directories in the binary path

file(COPY potentialPath DESTINATION ${PROJECT_BINARY_DIR})
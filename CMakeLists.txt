# Create an auto-start plugin. Auto start plugins provide callbacks that get
# called when the plugin is loaded and when the application shutsdown.

cmake_minimum_required(VERSION 3.3)

IF(NOT ParaView_BINARY_DIR)
  FIND_PACKAGE(ParaView REQUIRED)
  INCLUDE(${PARAVIEW_USE_FILE})
ENDIF()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

set (UIS jobListDialog.ui)

QT4_WRAP_UI(UI_HEADERS ${UIS})
#QT5_WRAP_CPP(MOC_SRCS pqJobListStarter.h)
QT4_WRAP_CPP(MOC_SRCS pqJobListStarter.h jobListDialog.h)


ADD_PARAVIEW_AUTO_START(IFACES IFACE_SRCS CLASS_NAME pqJobListStarter
                        STARTUP onStartup
                        SHUTDOWN onShutdown)

# create a plugin for this starter
ADD_PARAVIEW_PLUGIN(jobListStarter "1.0"
                    GUI_INTERFACES ${IFACES}
                    SOURCES pqJobListStarter.cxx jobListDialog.cxx ${MOC_SRCS} ${RCS_SRCS} ${IFACE_SRCS} ${UI_HEADERS})

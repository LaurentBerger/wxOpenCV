cmake_minimum_required(VERSION 3.2)

#list(APPEND CMAKE_MODULE_PATH "/c/install/openscenegraph/cmake")
PROJECT (wxopencv)
set(CMAKE_MODULE_PATH "${wxopencv_SOURCE_DIR}/CMake")
set(CMAKE_SUPPRESS_REGENERATION true)
#set(plplot $ENV{PLPLOT_HOME})



#find_package(glog QUIET OPTIONAL_COMPONENTS)
#find_package(ceres QUIET OPTIONAL_COMPONENTS)
#find_package(VTK  OPTIONAL_COMPONENTS)


find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenCV  REQUIRED)
FIND_PACKAGE(OpenThreads)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgText)
FIND_PACKAGE(osgSim)

find_package(wxWidgets REQUIRED COMPONENTS aui gl xml html core base )
find_package(PLplot  REQUIRED)
set(LOSDK $ENV{LibreOfficeSDK})

FIND_PATH(
  LOSDK_INCLUDE_DIR
  NAMES main.h 
  PATHS /usr/local/include /usr/include  ${LOSDK}/include/sal
)
option(BUILD_SHARED_LIBS "Build shared library" ON)


if (WIN32)
	if (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	else (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ")
	endif (BUILD_SHARED_LIBS)
else (WIN32)
	if (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	else (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	endif (BUILD_SHARED_LIBS)
endif (WIN32)

FOREACH( mesdossiers 
        wxOpenCVMain
#       RapportAOO
#		ScilabOpenCV
    )

    ADD_SUBDIRECTORY(${mesdossiers})

ENDFOREACH()


#message ("update config.txt in ${PROJECT_SOURCE_DIR}/bin")
#install (FILES "${PROJECT_SOURCE_DIR}/config.txt" DESTINATION "${PROJECT_SOURCE_DIR}/bin")

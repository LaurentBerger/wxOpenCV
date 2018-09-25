cmake_minimum_required(VERSION 2.8)


PROJECT (wxopencv)
set(CMAKE_MODULE_PATH "${wxopencv_SOURCE_DIR}/CMake")
set(CMAKE_SUPPRESS_REGENERATION true)
#set(plplot $ENV{PLPLOT_HOME})
set(LOSDK $ENV{LibreOfficeSDK})

FIND_PATH(
  LOSDK_INCLUDE_DIR
  NAMES main.h 
  PATHS /usr/local/include /usr/include  ${LOSDK}/include/sal
)



#find_package(glog QUIET OPTIONAL_COMPONENTS)
#find_package(ceres QUIET OPTIONAL_COMPONENTS)
#find_package(VTK  OPTIONAL_COMPONENTS)


find_package(OpenSceneGraph  REQUIRED osgDB osgUtil osgGA osgViewer osgText osgSim)
find_package(OpenCV  REQUIRED)
find_package(wxWidgets COMPONENTS core base net adv aui html qa richtext stc ribbon xml gl REQUIRED)
find_package(PLplot  REQUIRED)
option(BUILD_SHARED_LIBS "Build shared library" ON)
if (WIN32)
	if (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	else (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
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

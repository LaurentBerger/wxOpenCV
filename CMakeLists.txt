cmake_minimum_required(VERSION 2.8)


PROJECT (wxopencv)
set(CMAKE_MODULE_PATH "${wxopencv_SOURCE_DIR}/CMake")
set(plplot $ENV{PLPLOT_HOME})
set(LOSDK $ENV{LibreOfficeSDK})

FIND_PATH(
  LOSDK_INCLUDE_DIR
  NAMES main.h 
  PATHS /usr/local/include /usr/include  ${LOSDK}/include/sal
)



find_package(OpenSceneGraph  3.2.1 REQUIRED osgDB osgUtil osgGA osgViewer osgText osgSim)
find_package(OpenCV  REQUIRED)
find_package(wxWidgets COMPONENTS core base net adv aui html  qa richtext stc ribbon xml gl REQUIRED)
find_package(PLplot  REQUIRED)
option(BUILD_SHARED_LIBS "Build shared library" ON)


FOREACH( mesdossiers 
        wxOpenCVMain
        RapportAOO
    )

    ADD_SUBDIRECTORY(${mesdossiers})

ENDFOREACH()


#message ("update config.txt in ${PROJECT_SOURCE_DIR}/bin")
#install (FILES "${PROJECT_SOURCE_DIR}/config.txt" DESTINATION "${PROJECT_SOURCE_DIR}/bin")

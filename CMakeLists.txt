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



find_package(OpenCV  REQUIRED)
find_package(wxWidgets COMPONENTS core base net adv aui html  qa richtext stc ribbon xml REQUIRED)
find_package(PLplot  REQUIRED)



add_subdirectory(wxOpenCVMain)
add_subdirectory(RapportAOO)

#message ("update config.txt in ${PROJECT_SOURCE_DIR}/bin")
#install (FILES "${PROJECT_SOURCE_DIR}/config.txt" DESTINATION "${PROJECT_SOURCE_DIR}/bin")

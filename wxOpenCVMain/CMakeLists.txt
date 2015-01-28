cmake_minimum_required(VERSION 2.8)

PROJECT (wxOpenCVMain)

set (wxOpenCV_VERSION_MAJOR 0)
set (wxOpenCV_VERSION_MINOR 0)

file(GLOB wxOpenCVMain_SRCS
    "*.h"
    "*.cpp"
    toolbar.rc)
ADD_EXECUTABLE (wxOpenCVMain ${wxOpenCVMain_SRCS})

configure_file (
	"${PROJECT_SOURCE_DIR}/wxOpenCVConfig.h.in"
	"${PROJECT_SOURCE_DIR}/wxOpenCVConfig.h"
	)

if (wxWidgets_FOUND)
	include_directories(${wxWidgets_INCLUDE_DIRS})
	target_link_libraries (wxOpenCVMain ${wxWidgets_LIBRARIES} )
endif (wxWidgets_FOUND)
message("OPENCV = ${OpenCV_INCLUDE_DIRS}")
if (OpenCV_FOUND)
	include_directories( ${OpenCV_INCLUDE_DIRS} )
#	include_directories("${OPENCV_CONFIG_PATH}/include")
	target_link_libraries( wxOpenCVMain ${OpenCV_LIBS} )
else (OpenCV_FOUND)
message("PB->OPENCV = ${OpenCV_INCLUDE_DIRS}")
endif (OpenCV_FOUND)
if (PLplot_FOUND)
	include_directories(${PLplot_INCLUDE_DIR})
	target_link_libraries (wxOpenCVMain ${PLplot_wxwidgets_LIBRARY} ${PLplot_cxx_LIBRARY} )
endif (PLplot_FOUND)

IF(WIN32) # Check if we are on Windows
	message( STATUS         "WINDOWS")
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(wxOpenCVMain PROPERTIES  COMPILE_DEFINITIONS "")
  
		if  (CMAKE_CL_64)
			set_target_properties(wxOpenCVMain PROPERTIES  COMPILE_DEFINITIONS "__WXMSW__;_CRT_SECURE_NO_WARNINGS")
		else()
			set_target_properties(wxOpenCVMain PROPERTIES  COMPILE_DEFINITIONS "__WXMSW__;_CRT_SECURE_NO_WARNINGS")
		endif()
		set_target_properties(wxOpenCVMain PROPERTIES  LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
		set_target_properties(wxOpenCVMain PROPERTIES LINK_FLAGS_RELEASE  "/SUBSYSTEM:WINDOWS")		
	else()
		if (NOT CMAKE_COMPILER_IS_GNUCXX)
			message (SEND_ERROR  "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
		endif()
	endif()
else()
	if(UNIX)
		set_target_properties(wxOpenCVMain PROPERTIES  COMPILE_DEFINITIONS "__WXGTK__")
	else()
		message ("You are on an unsupported platform! (Not Win32 or Unix)")
	endif()
endif()

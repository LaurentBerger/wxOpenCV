cmake_minimum_required(VERSION 2.8)

PROJECT (wxRapportAOO)

set (wxRapportAOO_VERSION_MAJOR 0)
set (wxRapportAOO_VERSION_MINOR 0)

file(GLOB wxRapportAOO_SRCS
    "*.h"
    "*.cpp")
ADD_EXECUTABLE (wxRapportAOO ${wxRapportAOO_SRCS})

if (wxWidgets_FOUND)
	include_directories(${wxWidgets_INCLUDE_DIRS})
	target_link_libraries (wxRapportAOO ${wxWidgets_LIBRARIES} )
endif (wxWidgets_FOUND)
if (LOSDK_INCLUDE_DIR)
	message ("LOSDK=  ${LOSDK}  ${LOSDK_INCLUDE_DIR}")
	include_directories(${LOSDK_INCLUDE_DIR})
	include_directories(${LOSDK_})
	include_directories("${LOSDK}/includecpp")
	include_directories("${LOSDK}/include")
	target_link_libraries (wxRapportAOO "${LOSDK}/lib/icppu.lib;${LOSDK}/lib/icppuhelper.lib;${LOSDK}/lib/ipurpenvhelper.lib")
	target_link_libraries (wxRapportAOO "${LOSDK}/lib/isal.lib;${LOSDK}/lib/isalhelper.lib")
endif (LOSDK_INCLUDE_DIR)

IF(WIN32) # Check if we are on Windows
	message( STATUS         "WINDOWS")
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(wxRapportAOO PROPERTIES  COMPILE_DEFINITIONS "")
  
		if  (CMAKE_CL_64)
			set_target_properties(wxRapportAOO PROPERTIES  COMPILE_DEFINITIONS "__WXMSW__;WXUSINGDLL;CPPU_ENV=msci;_CRT_SECURE_NO_WARNINGS")
		else()
			set_target_properties(wxRapportAOO PROPERTIES  COMPILE_DEFINITIONS "__WXMSW__;WXUSINGDLL;CPPU_ENV=msci;_CRT_SECURE_NO_WARNINGS")
		endif()
		set_target_properties(wxRapportAOO PROPERTIES  LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
		set_target_properties(wxRapportAOO PROPERTIES LINK_FLAGS_RELEASE  "/SUBSYSTEM:CONSOLE")		
	else()
		if (NOT CMAKE_COMPILER_IS_GNUCXX)
			message (SEND_ERROR  "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
		endif()
	endif()
else()
	if(UNIX)
		set_target_properties(wxRapportAOO PROPERTIES  COMPILE_DEFINITIONS "__WXGTK__")
	else()
		message ("You are on an unsupported platform! (Not Win32 or Unix)")
	endif()
endif()

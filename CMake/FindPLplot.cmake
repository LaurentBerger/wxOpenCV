# Find PLplot header and library.

# This module defines the following uncached variables:
#  PLplot_FOUND, if false, do not try to use PLplot.
#  PLplot_INCLUDE_DIRS, where to find plplot.h.
#  PLplot_LIBRARIES, the libraries to link against to use PLplot
#  PLplot_LIBRARY_DIRS, the directory where the PLplot library is found.
message ("plplot suffixe ${plplot}")
find_path (PLPLOTPATH "plplot")
set(plplotdir ${PLPLOTPATH}/plplot)
message("PLplot_DRIVER_DIR2 ${plplotdir}")

FIND_PATH(
  PLplot_DRIVER_DIR
  NAMES svg.dll libsvg.so
  PATHS /usr/local/include /usr/include  "${plplot}/../../include/plplot" ${plplot}/lib/plplot5.10.0/driversd
  PATH_SUFFIXES "${plplot}/../../lib/plplot5.10.0/driversd" ${plplot} "${plplotdir}/bin"
)

FIND_PATH(
  PLplot_DYNLIB
  NAMES plplotwxwidgetsd.dll libplplotwxwidgets.so 
  PATHS /usr/local/lib /usr/include  "${plplot}/../../include/plplot" ${plplot}/bin
  PATH_SUFFIXES "${plplot}/../../lib/plplot5.10.0/driversd" ${plplot} ${plplotdir}
)
FIND_PATH(
  PLplot_STATLIB
  NAMES plplotwxwidgets.lib libplplotwxwidgets.so 
  PATHS /usr/local/lib /usr/include  "${plplot}/../../include/plplot" ${plplot}/lib ${plplotdir}/lib
  PATH_SUFFIXES "${plplot}/../../lib/plplot5.10.0/driversd" ${plplot} 
)
FIND_PATH(
  PLplot_INCLUDE_DIR
  NAMES plplot.h 
  PATHS /usr/local/include /usr/local/include/plplot /usr/include  ${plplotdir}/include/plplot "${plplot}/../../include/plplot" ${plplot}/include/plplot
  PATH_SUFFIXES "${plplot}/../../include/plplot" ${plplot} 
)
message ("PLPLOT DYN LIB ${PLplot_DYNLIB}")
message ("PLplot_INCLUDE_DIR ${PLplot_INCLUDE_DIR}")

IF (PLplot_STATLIB)
	set(PLplot_FOUND ON)
	set(PLplot_INCLUDE_DIRS ${PLplot_INCLUDE_DIR})
	set(PLplot_cxx_LIBRARY ${PLplot_STATLIB}/plplotcxx.lib ${PLplot_STATLIB}/csirocsa.lib)
	set (PLplot_wxwidgets_LIBRARY ${PLplot_STATLIB}/plplotwxwidgets.lib ${PLplot_STATLIB}/plplot.lib ${PLplot_STATLIB}/qsastime.lib)
	set( PLplot_LIBRARIES ${PLplot_LIBRARY} ${PLplot_wxwidgets_LIBRARY} ${PLplot_cxx_LIBRARY} ${PLplot_cxx_LIBRARY})
else(PLplot_STATLIB)
	IF( PLplot_INCLUDE_DIR )
	  find_library( PLplot_LIBRARY
	    NAMES plplotd plplot
	    PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib
		PATH_SUFFIXES ${plplot}
	  )
	  if(PLplot_LIBRARY)
	    set( PLplot_LIBRARY_DIR "" )
	    get_filename_component(PLplot_LIBRARY_DIRS ${PLplot_LIBRARY} PATH)
	    # Set uncached variables as per standard.
	    set(PLplot_FOUND ON)
	    set(PLplot_INCLUDE_DIRS ${PLplot_INCLUDE_DIR})
	    set(PLplot_LIBRARIES ${PLplot_LIBRARY})
	  endif(PLplot_LIBRARY)
	  
	  # find cxx bindings
	  find_library( PLplot_cxx_LIBRARY
	    NAMES plplotcxxd plplotcxx
	    PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib
		PATH_SUFFIXES ${plplot}
	  )
	  find_library( PLplot_svg_LIBRARY
	    NAMES plplotcxxd plplotcxx
	    PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib
		PATH_SUFFIXES ${plplot}
	  )
	  if( PLplot_cxx_LIBRARY )
	    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_cxx_LIBRARY} )
	  endif( PLplot_cxx_LIBRARY )
	  

	  # find wxwidgets bindings
	  find_library( PLplot_wxwidgets_LIBRARY
	    NAMES plplotwxwidgetsd plplotwxwidgets
	    PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib
		PATH_SUFFIXES ${plplot}
	  )
	  if( PLplot_wxwidgets_LIBRARY )
	    set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_wxwidgets_LIBRARY} )
	  endif( PLplot_wxwidgets_LIBRARY )
	endif(PLplot_INCLUDE_DIR)
endif(PLplot_INCLUDE_DIR)	    
if(PLplot_FOUND)
  if(NOT PLplot_FIND_QUIETLY)
    message(STATUS "FindPLplot: Found both PLplot headers and library")
  endif(NOT PLplot_FIND_QUIETLY)
else(PLplot_FOUND)
  if(PLplot_FIND_REQUIRED)
    message(FATAL_ERROR "FindPLplot: Could not find PLplot headers or library")
  endif(PLplot_FIND_REQUIRED)
endif(PLplot_FOUND)

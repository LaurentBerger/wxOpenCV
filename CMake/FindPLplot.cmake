# Find PLplot header and library.

# This module defines the following uncached variables:
#  PLplot_FOUND, if false, do not try to use PLplot.
#  PLplot_INCLUDE_DIRS, where to find plplot.h.
#  PLplot_LIBRARIES, the libraries to link against to use PLplot
#  PLplot_LIBRARY_DIRS, the directory where the PLplot library is found.
find_path (PLPLOTPATH "plplot")
set(plplotdir "c:/Program Files/plplot")
set(plplot "c:/Program Files/plplot")
message("PLplot_DRIVER_DIR2 ${plplotdir}")


FIND_PATH(
  PLplot_INCLUDE_DIR
  NAMES plplot.h 
  PATHS /usr/local/include /usr/local/include/plplot /usr/include  ${plplotdir}/include/plplot "${plplot}/../../include/plplot" ${plplot}/include/plplot
  PATH_SUFFIXES "${plplot}" ${plplot} 
)
message ("PLPLOT  ${plplot}")
message ("PLplot_INCLUDE_DIR ${PLplot_INCLUDE_DIR}")

IF( PLplot_INCLUDE_DIR )
	if (PLplot_STATLIB)
		set(PLplot_FOUND ON)
		set(PLplot_INCLUDE_DIRS ${PLplot_INCLUDE_DIR})
		set(PLplot_CXX_LIBSTAT_DEBUG1 ${PLplot_STATLIB}/debug/plplotcxx.lib )
		set(PLplot_CXX_LIBSTAT_DEBUG2 ${PLplot_STATLIB}/debug/csirocsa.lib )
		set(PLplot_CXX_LIBSTAT_DEBUG3 ${PLplot_STATLIB}/debug/plplot.lib )
		set(PLplot_CXX_LIBSTAT_DEBUG4 ${PLplot_STATLIB}/debug/plplotwxwidgets.lib )
		set(PLplot_CXX_LIBSTAT_DEBUG5 ${PLplot_STATLIB}/debug/qsastime.lib )
		set(PLplot_CXX_LIBSTAT_RELEASE1 ${PLplot_STATLIB}/release/plplotcxx.lib )
		set(PLplot_CXX_LIBSTAT_RELEASE2 ${PLplot_STATLIB}/release/csirocsa.lib )
		set(PLplot_CXX_LIBSTAT_RELEASE3 ${PLplot_STATLIB}/release/plplot.lib )
		set(PLplot_CXX_LIBSTAT_RELEASE4 ${PLplot_STATLIB}/release/plplotwxwidgets.lib )
		set(PLplot_CXX_LIBSTAT_RELEASE5 ${PLplot_STATLIB}/release/qsastime.lib )
	endif()
	if (PLplot_DYNLIB)
		set(PLplot_FOUND ON)
		set(PLplot_INCLUDE_DIRS ${PLplot_INCLUDE_DIR})
		set(PLplot_CXX_LIBDYN_DEBUG1 ${PLplot_DYNLIB}/debug/plplotcxx.lib )
		set(PLplot_CXX_LIBDYN_DEBUG2 ${PLplot_DYNLIB}/debug/csirocsa.lib )
		set(PLplot_CXX_LIBDYN_DEBUG3 ${PLplot_DYNLIB}/debug/plplot.lib )
		set(PLplot_CXX_LIBDYN_DEBUG4 ${PLplot_DYNLIB}/debug/plplotwxwidgets.lib )
		set(PLplot_CXX_LIBDYN_DEBUG5 ${PLplot_DYNLIB}/debug/qsastime.lib )
		set(PLplot_CXX_LIBDYN_RELEASE1 ${PLplot_DYNLIB}/release/plplotcxx.lib )
		set(PLplot_CXX_LIBDYN_RELEASE2 ${PLplot_DYNLIB}/release/csirocsa.lib )
		set(PLplot_CXX_LIBDYN_RELEASE3 ${PLplot_DYNLIB}/release/plplot.lib )
		set(PLplot_CXX_LIBDYN_RELEASE4 ${PLplot_DYNLIB}/release/plplotwxwidgets.lib )
		set(PLplot_CXX_LIBDYN_RELEASE5 ${PLplot_DYNLIB}/release/qsastime.lib )
	endif()
	find_library( PLplot_LIBRARY_RELEASE
	NAMES plplot
	PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/release 
	PATH_SUFFIXES ${plplot}
	)
	find_library( PLplot_LIBRARY_DEBUG
	NAMES plplot
	PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/debug 
	PATH_SUFFIXES ${plplot}
	)
	# find cxx bindings
	find_library( PLplot_CXX_LIBRARY_RELEASE
	NAMES plplotcxx
	PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/release 
	PATH_SUFFIXES ${plplot}
	)
	find_library( PLplot_CXX_LIBRARY_DEBUG
	NAMES plplotcxx
	PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/debug 
	PATH_SUFFIXES ${plplot}
	)
	if( PLplot_cxx_LIBRARY )
		set(PLplot_FOUND ON)
		set( PLplot_LIBRARIES ${PLplot_LIBRARIES} ${PLplot_cxx_LIBRARY} )
	endif( PLplot_cxx_LIBRARY )


	# find wxwidgets bindings
	find_library( PLplot_WXWIDGETS_LIBRARY_RELEASE
		NAMES plplotwxwidgetsd plplotwxwidgets
		PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/release
		PATH_SUFFIXES ${plplot}
		)
	find_library( PLplot_WXWIDGETS_LIBRARY_DEBUG
		NAMES plplotwxwidgetsd plplotwxwidgets
		PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/debug
		PATH_SUFFIXES ${plplot}
		)
	find_library( PLplot_csirocsa_LIBRARY_RELEASE
		NAMES csirocsa 
		PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/release
		PATH_SUFFIXES ${plplot}
		)
	find_library( PLplot_csirocsa_LIBRARY_DEBUG
		NAMES csirocsa csirocsa
		PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/debug
		PATH_SUFFIXES ${plplot}
		)
	find_library( PLplot_qsastime_LIBRARY_RELEASE
		NAMES qsastime 
		PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/release
		PATH_SUFFIXES ${plplot}
		)
	find_library( PLplot_qsastime_LIBRARY_DEBUG
		NAMES qsastime 
		PATHS /usr/local/lib /usr/lib ${plplot}/../../lib ${plplot}/lib/debug
		PATH_SUFFIXES ${plplot}
		)
		
	if( PLplot_WXWIDGETS_LIBRARY_DEBUG )
		set(PLplot_FOUND ON)
	endif( PLplot_wxwidgets_LIBRARY )

	if(PLplot_FOUND)
		set( PLplot_LIBRARIES_DEBUG ${PLplot_CXX_LIBRARY_DEBUG} ${PLplot_WXWIDGETS_LIBRARY_DEBUG} ${PLplot_LIBRARY_DEBUG})
		set( PLplot_LIBRARIES_RELEASE ${PLplot_CXX_LIBRARY_RELEASE} ${PLplot_WXWIDGETS_LIBRARY_RELEASE} ${PLplot_LIBRARY_RELEASE})
		set( PLplot_LIBRARY_DIR "" )
		# Set uncached variables as per standard.
		set(PLplot_FOUND ON)
		set(PLplot_INCLUDE_DIRS ${PLplot_INCLUDE_DIR})
		set(PLplot_LIBRARIES ${PLplot_LIBRARY})
	endif(PLplot_FOUND)

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

cmake_minimum_required(VERSION 2.8)

PROJECT (ScilabOpenCV)



file(GLOB ScilabOpenCV_SRCS
    ../wxOpenCVMain/CalcMoment.cpp
	../wxOpenCVMain/DericheShen.cpp
	../wxOpenCVMain/ImInCV.cpp
	../wxOpenCVMain/ImCvMinMAx.cpp
	../wxOpenCVMain/ImCvMinMAx.cpp
	../wxOpenCVMain/ImageInfoYML.cpp
	../wxOpenCVMain/ImInCVInitOp.cpp
	../wxOpenCVMain/ImInCVOperation.cpp
	../wxOpenCVMain/LigneMediane.cpp
	main.cpp
	../wxOpenCVMain/ParametreOperation.cpp
	../wxOpenCVMain/Reconstuction.cpp
	../wxOpenCVMain/ImageConstante.h
	../wxOpenCVMain/ImageInfo.h
	../wxOpenCVMain/Panoramique.h
	../wxOpenCVMain/ParametreOperation.h
    )
ADD_LIBRARY (ScilabOpenCV SHARED ${ScilabOpenCV_SRCS})
message("OPENCV = ${OpenCV_INCLUDE_DIRS}")
if (OpenCV_FOUND)
	include_directories( ${OpenCV_INCLUDE_DIRS} )
	target_link_libraries( ScilabOpenCV ${OpenCV_LIBS} )
else (OpenCV_FOUND)
	include_directories( ${OpenCV_INCLUDE_DIRS} )
#	include_directories("${OPENCV_CONFIG_PATH}/include")
	target_link_libraries( ScilabOpenCV ${OpenCV_LIBS} )
message("SCILABOPENCV PB->OPENCV = ${OpenCV_INCLUDE_DIRS}")
endif (OpenCV_FOUND)
			set_target_properties(ScilabOpenCV PROPERTIES  COMPILE_DEFINITIONS "__SCILABOPENCV__;_USRDLL")


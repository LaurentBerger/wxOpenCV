#!/bin/bash -e
source ./libmodel.sh
RepoSource=opencv
cd Build/$RepoSource
CMAKE_OPTIONS='-DBUILD_opencv_world:BOOL=OFF -DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=ON -DBUILD_DOCS:BOOL=OFF -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DWITH_LAPACK:BOOL=ON -DCV_TRACE:BOOL=OFF -DBUILD_PNG:BOOL=OFF -DBUILD_ZLIB:BOOL=OFF '
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DZLIB_INCLUDE_DIR:PATH=${installRepo}/zlib/include -DZLIB_LIBRARY_DEBUG:FILEPATH=${installRepo}/zlib/lib/${zlibname}d.lib -DZLIB_LIBRARY_RELEASE:FILEPATH=${installRepo}/zlib/lib/${zlibname}.lib \
-DPNG_PNG_INCLUDE_DIR:PATH=${installRepo}/libpng/include -DPNG_LIBRARY_RELEASE:FILEPATH=${installRepo}/libpng/lib/${pnglibname}.lib -DPNG_LIBRARY_DEBUG:FILEPATH=${installRepo}/libpng/lib/${pnglibname}d.lib \
-DEIGEN_DIR:PATH="$myRepo"/eigen -DEIGEN_INCLUDE_DIR="$myRepo"/eigen -DEIGEN_INCLUDE_PATH="$myRepo"/eigen \
-DCeres_DIR=${installRepo}/ceres-solver/cmake -Dglog_DIR:PATH=${installRepo}/glog/lib/cmake/glog -Dgflags_DIR:PATH=${installRepo}/gflags/lib/cmake/gflags \
-DLAPACK_IMPL=MKL -DMKL_ROOT_DIR:PATH="C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl" \
-DMKL_INCLUDE_DIRS:PATH="C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/include" \
-DBLAS_mkl_intel_lp64_LIBRARY:FILEPATH="C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64_win/mkl_intel_lp64.lib" \
-DBLAS_mkl_sequential_LIBRARY:FILEPATH="C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64_win/mkl_rt.lib" \
-DBLAS_mkl_intel_c_LIBRARY:FILEPATH="C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64_win/mkl_core.lib" \
-DPYTHON3_EXECUTABLE="C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python36_64/python.exe" \
-DPYTHON3_INCLUDE_DIR="C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python36_64/include" \
-DPYTHON3_LIBRARY="C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python36_64/libs/python36.lib" \
-DPYTHON3_LIBRARY_DEBUG="C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python36_64/libs/python36_d.lib" \
-DPYTHON3_NUMPY_INCLUDE_DIRS="C:/Users/Laurent/AppData/Roaming/Python/Python36/site-packages/numpy/core/include" \
-DPYTHON3_PACKAGES_PATH="C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python36_64/Lib/site-packages" \
$CMAKE_OPTIONS -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -DOPENCV_ENABLE_NONFREE:BOOL=ON \
-DVTK_DIR:PATH="$myRepo"/install/vtk/lib/cmake/vtk-8.1 -DINSTALL_CREATE_DISTRIB=ON -DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource"  ../../"$RepoSource"

cd ..

echo "************************* $Source_DIR -->devenv debug"
cmake --build $RepoSource  --config release 
echo "************************* $Source_DIR -->devenv release"
cmake --build $RepoSource  --target install --config release 
echo "************************* $Source_DIR -->devenv debug"
cmake --build $RepoSource  --config debug 
echo "************************* $Source_DIR -->devenv release"
cmake --build $RepoSource  --target install --config debug 
cd ..




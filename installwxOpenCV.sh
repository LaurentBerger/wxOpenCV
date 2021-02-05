source ./libmodel.sh
RepoSource=wxopencv
pushd ${buildRepo}/$RepoSource 
cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DBUILD_SHARED_LIBS:BOOL=${SharedLibs} \
-DwxWidgets_ROOT_DIR=${installRepo}/wxwidgets -DwxWidgets_LIB_DIR=${installRepo}/wxwidgets/lib/${wxlibs} \
-Dopencv_DIR=${installRepo}/opencv/${ocvlibs} \
-DPLplot_DIR:PATH="g:/lib/install/plplot/lib/cmake/plplot" \
-DCMAKE_INSTALL_PREFIX=${installRepo}/"$RepoSource"  ${sourceRepo}/"$RepoSource"


cmake --build .  --config release
cmake --build .  --target install --config release 
cmake --build .  --config debug 
cmake --build .  --target install --config debug 
popd

#-DOpenCV_DIR:PATH=G:/Lib/install/Static/opencv -Dceres_DIR:PATH=G:/Lib/install/ceres-solver/CMake -Dglog_DIR:PATH=G:/Lib/install/glog/lib/cmake/glog \
#-DwxWidgets_LIB_DIR:PATH=G:/Lib/wxWidgets-3.1.0/lib/vc_x64_lib -DwxWidgets_ROOT_DIR=G:/Lib/wxWidgets-3.1.0 "C:\Users\Laurent.PC-LAURENT-VISI\Documents\Visual Studio 2013\wxopencv"
#-DPLplot_DIR:PATH="C:/Program Files/plplot/share/cmake/plplot5.13.0/examples/cmake" \

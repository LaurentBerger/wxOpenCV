#!/bin/bash 
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
updateRepo=ON
myRepo=$(pwd)
installRepo=${myRepo}/install
buildRepo=${myRepo}/build
sourceRepo=${myRepo}
SharedLibs=ON
zlibname=zlib
pnglibname=libpng16
wxlibs=vc_x64_dll
ocvlibs=
wxhighguilib="wxHighguiLib"
if [[ $SharedLibs = OFF ]]
then
zlibname=${zlibname}static
pnglibname=${pnglibname}_static
wxlibs=vc_x64_lib
ocvlibs=x64/vc15/staticlib
wxhighguilib="wxHighguiLibStatic"
fi
export wxhighguilib


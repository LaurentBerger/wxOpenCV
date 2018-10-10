#!/bin/bash -e 
source ./libmodel.sh
librepo=zlib
if [[ $updateRepo = ON ]]
then
	if [ ! -d "$myRepo/$librepo" ]; 
	then 
		echo "cloning $librepo" 
		git clone https://github.com/madler/zlib.git 
		mkdir ${buildRepo}/$librepo 
		mkdir Install/$librepo 
	else 
		cd $librepo 
		git pull --rebase 
		git submodule update --init 
		cd .. 
	fi
fi
RepoSource=$librepo 
pushd ${buildRepo}/$RepoSource 
CMAKE_OPTIONS= cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DCMAKE_INSTALL_PREFIX=${installRepo}/"$RepoSource" ${sourceRepo}/"$RepoSource" 
cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd


#!/bin/bash -e 
librepo=libpng
if [[ $updateRepo = ON ]]
then
	if [ ! -d "$myRepo/$librepo" ]; 
	then 
		echo "cloning $librepo" 
		git clone https://github.com/glennrp/libpng.git 
		mkdir ${buildRepo}/$librepo 
		mkdir ${installRepo}/$librepo 
	else 
		cd $librepo 
		git pull --rebase 
		git submodule update --init 
		cd .. 
	fi
fi
RepoSource=$librepo 
pushd Build/$RepoSource 
CMAKE_OPTIONS= cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DPNG_TESTS:BOOL=OFF -DAWK= \
-DZLIB_INCLUDE_DIR:PATH=${installRepo}/zlib/include -DZLIB_LIBRARY_DEBUG:FILEPATH=${installRepo}/zlib/lib/zlibd.lib -DZLIB_LIBRARY_RELEASE:FILEPATH=${installRepo}/zlib/lib/zlib.lib \
-DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource" ${sourceRepo}/"$RepoSource" 
cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd



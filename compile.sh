#!/bin/bash

echo by Paulo Muniz de Ávila
echo Instalador Recommender TV
echo Seja paciente !!!
echo


        

echo
echo Recompilando Recommender/Scheduler Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/Scheduler/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-contextmanager/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-contextmanager/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-dataprocessing/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-dataprocessing/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo Recompilando Gingancl-cpp Rev. 23
cd /usr/src/gingaUFSCAR/gingancl-cpp/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando gingalssm-cpp Rev. 23
cd /usr/src/gingaUFSCAR/gingalssm-cpp/
chmod 755 ./autogen.sh
./autogen.sh --enable-tuner --enable-tsparser --enable-dataprocessing --enable-recommender

make
make install

echo
echo Recompilando ginga-cpp Rev. 23
cd /usr/src/gingaUFSCAR/ginga-cpp/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install



echo
echo
echo Que a força esteja com vocês !!!!!!
echo






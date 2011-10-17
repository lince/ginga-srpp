#!/bin/bash

echo by Paulo Muniz de Ávila
echo Instalador Recommender TV
echo Seja paciente !!!
echo



echo
echo Recompilando Recommender/Utils Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/Utils/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/SystemResource Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/SystemResource/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/Database Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/Database/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando Recommender/EIT Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/EIT/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/SDT Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/SDT/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/AgentListener Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/AgentListener/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/MiningAlgorithm Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/MiningAlgorithm/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/DataMining Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/DataMining/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/SchedulerItem Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/SchedulerItem/
chmod 755 ./autogen.sh
./autogen.sh
make
make install





echo
echo Recompilando ncl30-cpp/ncl30 Rev. 23
cd /usr/src/gingaFUCAPI/ncl30-cpp/ncl30/
chmod 755 ./autogen.sh
./autogen.sh
make
make install


echo
echo Recompilando ncl30-cpp/ncl30-converter/ Rev. 23
cd /usr/src/gingaFUCAPI/ncl30-cpp/ncl30-converter/
chmod 755 ./autogen.sh
./autogen.sh
make
make install



echo
echo Recompilando gingacc-cpp/gingacc-cm/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-cm/
chmod 755 ./autogen.sh
./autogen.sh
make
make install


echo
echo Recompilando gingacc-cpp/gingacc-dataprocessing/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-dataprocessing/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-player/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-player/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando wac-state/ Rev. 23
cd /usr/src/gingaFUCAPI/wac-state/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando LinceUtil/ Rev. 23
cd /usr/src/gingaFUCAPI/LinceUtil/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/LocalAgent Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/LocalAgent/
chmod 755 ./autogen.sh
./autogen.sh
make
make install


echo
echo Recompilando gingacc-cpp/gingacc-contextmanager/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-contextmanager/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando Recommender/Scheduler Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/Scheduler/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/EXportXML Rev. 23
cd /usr/src/gingaFUCAPI/Recommender/ExportXML/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando ncl30-cpp/ncl30-generator Rev. 23
cd /usr/src/gingaFUCAPI/ncl30-cpp/ncl30-generator/
chmod 755 ./autogen.sh
./autogen.sh
make
make install



echo
echo Recompilando gingacc-cpp/gingacc-ic/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-ic/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-multidevice/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-multidevice/
chmod 755 ./autogen.sh
./autogen.sh
make
make install
        
echo
echo Recompilando gingacc-cpp/gingacc-system/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-system/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-tsparser/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-tsparser/
chmod 755 ./autogen.sh
./autogen.sh
make
make install


echo
echo Recompilando gingacc-cpp/gingacc-cgc/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-cgc/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-elua/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-elua/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-tuner/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-tuner/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-um/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-um/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo Recompilando Gingancl-cpp Rev. 23
cd /usr/src/gingaFUCAPI/gingancl-cpp/
chmod 755 ./autogen.sh 
./autogen.sh --enable-recommender
make
make install

echo
echo Recompilando gingalssm-cpp Rev. 23
cd /usr/src/gingaFUCAPI/gingalssm-cpp/
chmod 755 ./autogen.sh
./autogen.sh --enable-tuner --enable-tsparser --enable-dataprocessing --enable-recommender

make
make install

echo
echo Recompilando ginga-cpp Rev. 23
cd /usr/src/gingaFUCAPI/ginga-cpp/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando telemidia-links-cpp/ Rev. 23
cd /usr/src/gingaFUCAPI/telemidia-links-cpp/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando telemidia-util-cpp/ Rev. 23
cd /usr/src/gingaFUCAPI/telemidia-util-cpp/
chmod 755 ./autogen.sh
./autogen.sh --enable-graphics --with-directfb --enable-javascript --without-x --without-sdl
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-player/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-player/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-contextmanager/ Rev. 23
cd /usr/src/gingaFUCAPI/gingacc-cpp/gingacc-contextmanager/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo
echo Que a força esteja com vocês !!!!!!
echo






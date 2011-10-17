#!/bin/bash

echo by Paulo Muniz de Ávila
echo Instalador Recommender TV
echo Seja paciente !!!
echo


        
echo
echo Recompilando Recommender Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/Utils Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/Utils/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/SystemResource Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/SystemResource/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/Database Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/Database/
chmod 755 ./autogen.sh --enable-recommender
./autogen.sh
make
make install

echo
echo Recompilando Recommender/EIT Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/EIT/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/SDT Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/SDT/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/AgentListener Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/AgentListener/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/LocalAgent Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/LocalAgent/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/MiningAlgorithm Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/MiningAlgorithm/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/DataMining Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/DataMining/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender/SchedulerItem Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/SchedulerItem/
chmod 755 ./autogen.sh
./autogen.sh
make
make install
echo
echo Recompilando Recommender/Scheduler Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/Scheduler/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando Recommender Rev. 23
cd /usr/src/gingaUFSCAR/Recommender/ExportXML/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

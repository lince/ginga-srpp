#!/bin/bash

echo
echo Recompilando ncl30-cpp/ncl30 Rev. 23
cd /usr/src/gingaUFSCAR/ncl30-cpp/ncl30/
chmod 755 ./autogen.sh
./autogen.sh
make
make install


echo
echo Recompilando ncl30-cpp/ncl30-converter/ Rev. 23
cd /usr/src/gingaUFSCAR/ncl30-cpp/ncl30-converter/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando ncl30-cpp/ncl30-generator Rev. 23
cd /usr/src/gingaUFSCAR/ncl30-cpp/ncl30-generator/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando wac-state/ Rev. 23
cd /usr/src/gingaUFSCAR/wac-state/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-cgc/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-cgc/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-cm/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-cm/
chmod 755 ./autogen.sh
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

echo
echo Recompilando gingacc-cpp/gingacc-elua/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-elua/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-ic/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-ic/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-multidevice/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-multidevice/
chmod 755 ./autogen.sh
./autogen.sh
make
make install
        
echo
echo Recompilando gingacc-cpp/gingacc-player/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-player/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-system/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-system/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-tsparser/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-tsparser/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-tuner/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-tuner/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingacc-cpp/gingacc-um/ Rev. 23
cd /usr/src/gingaUFSCAR/gingacc-cpp/gingacc-um/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo Recompilando Gingancl-cpp Rev. 23
cd /usr/src/gingaUFSCAR/gingancl-cpp/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando gingalssm-cpp Rev. 23
cd /usr/src/gingaUFSCAR/gingalssm-cpp/
chmod 755 ./autogen.sh
./autogen.sh --enable-tuner --enable-tsparser --enable-dataprocessing

make
make install

echo
echo Recompilando ginga-cpp Rev. 23
cd /usr/src/gingaUFSCAR/ginga-cpp/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando telemidia-links-cpp/ Rev. 23
cd /usr/src/gingaUFSCAR/telemidia-links-cpp/
chmod 755 ./autogen.sh
./autogen.sh
make
make install

echo
echo Recompilando telemidia-util-cpp/ Rev. 23
cd /usr/src/gingaUFSCAR/telemidia-util-cpp/
chmod 755 ./autogen.sh
./autogen.sh --enable-graphics --with-directfb --enable-javascript --without-x --without-sdl

make
make install


echo
echo
echo Que a força esteja com vocês !!!!!!
echo


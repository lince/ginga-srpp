#/bin/bash

curDir=`pwd`
echo $curDir

rm *.lua
cd .. &&
make uninstall
make distclean
./autogen.sh &&
make install &&
cd test &&
./test

cd $curDir


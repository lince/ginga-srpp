cd Utils
./autogen.sh
make
make install

cd ../SystemResource
./autogen.sh
make
make install

cd ../Database
./autogen.sh
make
make install

cd ../EIT
./autogen.sh
make
make install

cd ../SDT
./autogen.sh
make
make install

cd ../AgentListener
./autogen.sh
make
make install

cd ../LocalAgent
./autogen.sh
make
make install

cd ../MiningAlgorithm
./autogen.sh
make
make install

cd ../DataMining
./autogen.sh
make
make install

cd ../SchedulerItem
./autogen.sh
make
make install

cd ../Scheduler
./autogen.sh
make
make install

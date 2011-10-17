#ifndef _ITERACTION_H_
#define _ITERACTION_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "IteractionBase.h"
#include "Program.h"
#include "Volume.h"
#include "Key.h"
#include "Device.h"
#include "User.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Iteraction: public IteractionBase {
					//class Iteraction {
					
					private:
						static int idIteraction;
						Program* program;
						Volume* volume;
						Key* key;
						Device* device;
						string user;
						string time;
						string type;
						
					public:
						//Iteraction(Program* program, Volume* volume, Key* key, Device* device, User* user, string time, string type);
						Iteraction(Program* program, Volume* volume, Key* key, string user, string time, string type);						

						string generateSql(string tecla, string volum, string program);
						
						int getIdIteraction();
						static void setIdIteraction(int id);
						static void incIdIteraction();
						
						Program* getProgram();
						void setProgram(Program* program);
						
						Volume* getVolume();
						void setVolume(Volume* volume);
						
						Key* getKey();
						void setKey(Key* key);

						Device* getDevice();
						void setDevice(Device* device);

						string getUser();
						void setUser(string user);		

						string getTime();
						void setTime(string time);
						
						string getType();
						void setType(string type);
					};
				}
			}
		}
	}
}

#endif /*_ITERACTION_H_*/

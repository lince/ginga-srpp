#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <string>
#include "IteractionBase.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Channel: public IteractionBase {
					
					private:
						static int idChannel;
						int code;
						string name;
					
					public:
						Channel(int code, string name);
						
						string generateSql();
						
						int getIdChannel();
						static void setIdChannel(int id);
						static void incIdChannel();
						
						int getCode();
						void setCode(int code);
						
						string getName();
						void setName(string name);
						
					};
				}
			}
		}
	}
}

#endif /*_CHANNEL_H_*/

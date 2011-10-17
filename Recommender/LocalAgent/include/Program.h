#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "IteractionBase.h"
#include "Channel.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Program: public IteractionBase {
					
					private:
						static int idProgram;
						int age;
						int code;
						Channel* channel;
						string name;
						string genre;
						string subGenre;

					public:
						Program(int age, int code, Channel* channel, string name, string genre, string subGenre);
						
						string generateSql();
						
						int getIdProgram();
						static void setIdProgram(int id);
						static void incIdProgram();
						
						int getAge();
						void setAge(int age);
						
						int getCode();
						void setCode(int code);
						
						Channel* getChannel();
						void setChannel(Channel* channel);
						
						string getName();
						void setName(string name);
						
						string getGenre();
						void setGenre(string genre);
						
						string getSubGenre();
						void setSubGenre(string subGenre);

					};
				}
			}
		}
	}
}

#endif /*_PROGRAM_H_*/

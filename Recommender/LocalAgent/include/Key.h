#ifndef _KEY_H_
#define _KEY_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "IteractionBase.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Key: public IteractionBase {
					
					private:
						static int idKey;
						string code;
						string action;
					
					public:
						Key(string code, string action);
						
						string generateSql();
						
						int getIdKey();
						static void setIdKey(int id);
						static void incIdKey();
						
						string getCode();
						void setCode(string code);
						
						string getAction();
						void setAction(string action);
						
						string getType();
						
					};
				}
			}
		}
	}
}

#endif /*_KEY_H_*/

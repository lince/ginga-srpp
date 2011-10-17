#ifndef _META_H_
#define _META_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "IteractionBase.h"
#include "Program.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Meta: public IteractionBase {
					
					private:
						static int idMeta;
						Program* program;
						string meta;
					
					public:
						Meta(Program* program, string meta);
						
						string generateSql();
						
						int getIdMeta();
						static void setIdMeta(int id);
						static void incIdMeta();
						
						Program* getProgram();
						void setProgram(Program* program);
						
						string getMeta();
						void setMeta(string meta);
						
					};
				}
			}
		}
	}
}

#endif /*_META_H_*/

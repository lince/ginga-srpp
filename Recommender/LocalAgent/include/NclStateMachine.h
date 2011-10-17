#ifndef _NCLSTATEMACHINE_H_
#define _NCLSTATEMACHINE_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "Iteraction.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class NclStateMachine {
					
					private:
						static int idNclStateMachine;
						Iteraction* iteraction;
						
					public:
						NclStateMachine(Iteraction* iteraction);
						
						string generateSql();
						
						int getIdNclStateMachine();
						static void setIdNclStateMachine(int id);
						static void incIdNclStateMachine();
						
						Iteraction* getIteraction();
						void setIteraction(Iteraction* iteraction);
					};
				}
			}
		}
	}
}

#endif /*_NCLSTATEMACHINE_H_*/

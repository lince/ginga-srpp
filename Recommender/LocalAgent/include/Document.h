#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "NclStateMachine.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Document {
					
					private:
						static int idDocument;
						string id;
						NclStateMachine* nclStateMachine;
						
					public:
						Document(string id, NclStateMachine* nclStateMachine);
						
						string generateSql();
						
						int getIdDocument();
						static void setIdDocument(int id);
						static void incIdDocument();
						
						string getId();
						void setId(string id);
						
						NclStateMachine* getNclStateMachine();
						void setNclStateMachine(NclStateMachine* nclStateMachine);
					};
				}
			}
		}
	}
}

#endif /*_DOCUMENT_H_*/

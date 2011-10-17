#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "Document.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Context {
					
					private:
						static int idContext;
						string id;
						Context* context;
						Document* document;
						
					public:
						Context(string id, Context* context, Document* document);
						
						string generateSql();
						
						int getIdContext();
						static void setIdContext(int id);
						static void incIdContext();
						
						string getId();
						void setId(string id);
						
						Context* getContext();
						void setContext(Context* context);
						
						Document* getDocument();
						void setDocument(Document* document);
					};
				}
			}
		}
	}
}

#endif /*_CONTEXT_H_*/

#ifndef _MEDIA_H_
#define _MEDIA_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "Context.h"
#include "Document.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Media {
					
					private:
						static int idMedia;
						string id;
						string status;
						string time;
						Context* context;
						Document* document;
						
					public:
						Media(string id, string status, string time, Context* context, Document* document);
						
						string generateSql();
						
						int getIdMedia();
						static void setIdMedia(int id);
						static void incIdMedia();
						
						string getId();
						void setId(string id);
						
						string getStatus();
						void setStatus(string status);
						
						string getTime();
						void setTime(string time);
						
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

#endif /*_MEDIA_H_*/

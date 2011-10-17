#include "../include/Context.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Context::idContext = 0;

					Context::Context(string id, Context* context, Document* document) {
						this->id = id;
						this->context = context;
						this->document = document;
					}
					
					string Context::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						string cId;
						if(context == NULL)
							cId = util->IntToString(0);
						else
							cId = util->IntToString(context->getIdContext());
						return "insert into context (id, idSContext, idDocument) values ('" + id + "', " + cId + ", " + util->IntToString(document->getIdDocument()) + ");";
					}
					
					int Context::getIdContext(){return idContext;}
					void Context::setIdContext(int id){idContext = id;}
					void Context::incIdContext(){idContext++;}
					
					string Context::getId(){return id;}
					void Context::setId(string id){this->id = id;}
					
					Context* Context::getContext(){return context;}
					void Context::setContext(Context* context){this->context = context;}
					
					Document* Context::getDocument(){return document;}
					void Context::setDocument(Document* document){this->document = document;}

				}
			}
		}
	}
}


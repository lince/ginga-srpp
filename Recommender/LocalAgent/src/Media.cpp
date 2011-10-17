#include "../include/Media.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Media::idMedia = 1;

					Media::Media(string id, string status, string time, Context* context, Document* document) {
						this->id = id;
						this->status = status;
						this->time = time;
						this->context = context;
						this->document = document;
					}
					
					string Media::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						string cId;
						if(context == NULL)
							cId = util->IntToString(0);
						else
							cId = util->IntToString(context->getIdContext());
						return "insert into media (id, status, time, idContext, idDocument) values ('" + id + "', '" + status + "', '" + time + "', " + cId + ", " + util->IntToString(document->getIdDocument()) + ");";
					}
					
					int Media::getIdMedia(){return idMedia;}
					void Media::setIdMedia(int id){idMedia = id;}
					void Media::incIdMedia(){idMedia++;}
					
					string Media::getId(){return id;}
					void Media::setId(string id){this->id = id;}
					
					string Media::getStatus(){return status;}
					void Media::setStatus(string status){this->status = status;}
					
					string Media::getTime(){return time;}
					void Media::setTime(string time){this->time = time;}
					
					Context* Media::getContext(){return context;}
					void Media::setContext(Context* context){this->context = context;}
					
					Document* Media::getDocument(){return document;}
					void Media::setDocument(Document* document){this->document = document;}

				}
			}
		}
	}
}


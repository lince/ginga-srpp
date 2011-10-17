#include "../include/Document.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Document::idDocument = 1;
				
					Document::Document(string id, NclStateMachine* nclStateMachine) {
						this->id = id;
						this->nclStateMachine = nclStateMachine;
					}
					
					string Document::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into document (id, idNclStateMachine) values ('" + id + "', " + util->IntToString(nclStateMachine->getIdNclStateMachine()) + ");";
					}
					
					int Document::getIdDocument(){return idDocument;}
					void Document::setIdDocument(int id){idDocument = id;}
					void Document::incIdDocument(){idDocument++;}
					
					string Document::getId(){return id;}
					void Document::setId(string id){this->id = id;}
					
					NclStateMachine* Document::getNclStateMachine(){return nclStateMachine;}
					void Document::setNclStateMachine(NclStateMachine* nclStateMachine){this->nclStateMachine = nclStateMachine;}

				}
			}
		}
	}
}


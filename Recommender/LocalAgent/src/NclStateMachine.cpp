#include "../include/NclStateMachine.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int NclStateMachine::idNclStateMachine = 1;

					NclStateMachine::NclStateMachine(Iteraction* iteraction) {
						this->iteraction = iteraction;
					}
					
					string NclStateMachine::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into nclstatemachine (idIteraction) values (" + util->IntToString(iteraction->getIdIteraction()) + ");";
					}
					
					int NclStateMachine::getIdNclStateMachine(){return idNclStateMachine;}
					void NclStateMachine::setIdNclStateMachine(int id){idNclStateMachine = id;}
					void NclStateMachine::incIdNclStateMachine(){idNclStateMachine++;}
					
					Iteraction* NclStateMachine::getIteraction(){return iteraction;}
					void NclStateMachine::setIteraction(Iteraction* iteraction){this->iteraction = iteraction;}

				}
			}
		}
	}
}


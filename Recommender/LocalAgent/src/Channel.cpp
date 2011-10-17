#include "../include/Channel.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Channel::idChannel = 1;
					
					Channel::Channel(int code, string name) {
						this->code = code;
						this->name = name;
					}
					
					string Channel::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into channel (code, name) values ('" + util->IntToString(code) + "', '" + name + "');";
					}
					
					int Channel::getIdChannel(){return idChannel;}
					void Channel::setIdChannel(int id){idChannel = id;}
					void Channel::incIdChannel(){idChannel++;}
					
					int Channel::getCode(){return code;}
					void Channel::setCode(int code){this->code = code;}
					
					string Channel::getName(){return name;}
					void Channel::setName(string name){this->name = name;}

				}
			}
		}
	}
}


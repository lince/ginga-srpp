#include "../include/Key.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Key::idKey = 1;

					Key::Key(string code, string action) {
						this->code = code;
						this->action = action;
					}
					
					string Key::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into key (code, action) values ('" + code + "', '" + action + "');";
					}
					
					int Key::getIdKey(){return idKey;}
					void Key::setIdKey(int id){idKey = id;}
					void Key::incIdKey(){idKey++;}
					
					string Key::getCode(){return code;}
					void Key::setCode(string code){this->code = code;}
					
					string Key::getAction(){return action;}
					void Key::setAction(string action){this->action = action;}
					
					string Key::getType() {
						string type;
						if(action.compare("RED") == 0 || action.compare("GREEN") == 0 || action.compare("YELLOW") == 0 || action.compare("BLUE") == 0) {
							type = "interaction";
						}
						else if(action.compare("CURSOR_UP") == 0 || action.compare("CURSOR_DOWN") == 0 || action.compare("CURSOR_LEFT") == 0 || action.compare("CURSOR_RIGHT") == 0) {
							type = "navigation";
						}
						else if(action.compare("CHANNEL_UP") == 0 || action.compare("CHANNEL_DOWN") == 0) {
							type = "channelChange";
						}
						else if(action.compare("VOLUME_UP") == 0 || action.compare("VOLUME_DOWN") == 0) {
							type = "volumeChange";
						}
						else if(action.compare("PAUSE") == 0 || action.compare("STOP") == 0 || action.compare("PLAY") == 0) {
							type = "controlPresentation";
						}
						else {
							type = "other";
						}
						return type;
					}
						
				}
			}
		}
	}
}


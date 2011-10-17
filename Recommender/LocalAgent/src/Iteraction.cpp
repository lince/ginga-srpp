#include "../include/Iteraction.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {

					int Iteraction::idIteraction = 1;

//					Iteraction::Iteraction(Program* program, Volume* volume, Key* key, Device* device, User* user, string time, string type) {
					Iteraction::Iteraction(Program* program, Volume* volume, Key* key, string user, string time, string type) {
						this->program = program;
						this->volume = volume;
						this->key = key;
						this->device = device;
						this->user = user;
						this->time = time;
						this->type = type;
					}
					
					string Iteraction::generateSql(string tecla, string volum, string program){
						Utilitario* util = Utilitario::getInstance();
						/*string time = getTima();
						string type = getType();
						string user = getUser();*/
						return "insert into iteraction (time, type, idProgram, idVolume, idKey, idUser, idDevice ) values ('" + 
						    time + string("', '") + 
						    type + string("', ") + 
						    program + string(", ") + 
 						    volum + string(", ") +		
						    tecla + string(", ") +  
						    user + string(", 0 );");
						  //  user->getID() + string(", 0 );");
						  //  util->IntToString(user->getIdUser()) + string(", 0 );");						    
					}
					
					int Iteraction::getIdIteraction(){return idIteraction;}
					void Iteraction::setIdIteraction(int id){idIteraction = id;}
					void Iteraction::incIdIteraction(){idIteraction++;}
					
					Program* Iteraction::getProgram(){return program;}
					void Iteraction::setProgram(Program* program){this->program = program;}
					
					Volume* Iteraction::getVolume(){return volume;}
					void Iteraction::setVolume(Volume* volume){this->volume = volume;}
					
					Key* Iteraction::getKey(){return key;}
					void Iteraction::setKey(Key* key){this->key = key;}

					Device* Iteraction::getDevice(){return device;}
					void Iteraction::setDevice(Device* device){this->device = device;}

					string Iteraction::getUser(){return user;}
					void Iteraction::setUser(string user){this->user = user;}

					string Iteraction::getTime(){return time;}
					void Iteraction::setTime(string time){this->time = time;}
					
					string Iteraction::getType(){return type;}
					void Iteraction::setType(string type){this->type = type;}

				}
			}
		}
	}
}


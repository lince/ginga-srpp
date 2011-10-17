#include "../include/Program.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Program::idProgram = 1;

					Program::Program(int age, int code, Channel* channel, string name, string genre, string subGenre) {
						this->age = age;
						this->code = code;
						this->channel = channel;
						this->name = name;
						this->genre = genre;
						this->subGenre = subGenre;
					}
					
					string Program::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into program (age, genre, code, name, subGenre, idChannel) values (" + util->IntToString(age) + ", '" + genre + "', " + util->IntToString(code) + ", '" + name + "', '" + subGenre + "', " + util->IntToString(channel->getIdChannel()) + ");";
					}
					
					int Program::getIdProgram(){return idProgram;}
					void Program::setIdProgram(int id){idProgram = id;}
					void Program::incIdProgram(){idProgram++;}
					
					int Program::getAge(){return age;}
					void Program::setAge(int age){this->age = age;}
					
					int Program::getCode(){return code;}
					void Program::setCode(int code){this->code = code;}
					
					Channel* Program::getChannel(){return channel;}
					void Program::setChannel(Channel* channel){this->channel = channel;}
					
					string Program::getName(){return name;}
					void Program::setName(string name){this->name = name;}
					
					string Program::getGenre(){return genre;}
					void Program::setGenre(string genre){this->genre = genre;}
					
					string Program::getSubGenre(){return subGenre;}
					void Program::setSubGenre(string subGenre){this->subGenre = subGenre;}

				}
			}
		}
	}
}


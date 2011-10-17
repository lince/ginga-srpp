#include "../include/Meta.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Meta::idMeta = 1;

					Meta::Meta(Program* program, string meta) {
						this->program = program;
						this->meta = meta;
					}
					
					string Meta::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into meta (meta, idProgram) values ('" + meta + "', " + util->IntToString(program->getIdProgram()) + ");";
					}
					
					int Meta::getIdMeta(){return idMeta;}
					void Meta::setIdMeta(int id){idMeta = id;}
					void Meta::incIdMeta(){idMeta++;}
					
					Program* Meta::getProgram(){return program;}
					void Meta::setProgram(Program* program){this->program = program;}
					
					string Meta::getMeta(){return meta;}
					void Meta::setMeta(string meta){this->meta = meta;}
						
				}
			}
		}
	}
}


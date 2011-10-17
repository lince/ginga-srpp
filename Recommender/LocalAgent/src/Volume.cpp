#include "../include/Volume.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
				
					int Volume::idVolume = 1;

					Volume::Volume(int level, bool mute) {
						this->level = level;
						this->mute = mute;
					}
					
					string Volume::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into volume (level, mute) values (" + util->IntToString(level) + ", " + util->IntToString(mute) + ");";
					}
					
					int Volume::getIdVolume(){return idVolume;}
					void Volume::setIdVolume(int id){idVolume = id;}
					void Volume::incIdVolume(){idVolume++;}
					
					int Volume::getLevel(){return level;}
					void Volume::setLevel(int level){this->level = level;}
					
					int Volume::getMute(){if(mute) return 1;else return 0;}
					void Volume::setMute(bool mute){this->mute = mute;}
						
				}
			}
		}
	}
}


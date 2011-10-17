#ifndef _VOLUME_H_
#define _VOLUME_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;
#include "IteractionBase.h"

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Volume: public IteractionBase {
					
					private:
						static int idVolume;
						int level;
						bool mute;
					
					public:
						Volume(int level, bool mute);
						
						string generateSql();
						
						int getIdVolume();
						static void setIdVolume(int id);
						static void incIdVolume();
						
						int getLevel();
						void setLevel(int level);
						
						int getMute();
						void setMute(bool mute);
						
					};
				}
			}
		}
	}
}

#endif /*_VOLUME_H_*/

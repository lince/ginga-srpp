#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;

using namespace std;
#include "Device.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                    class User {
                    private:
						static int idUser;

						string ID;
						string userGenre;
						string locationZip;
						string locationLatitude;
						string locationLongitude;
						string userBirth;

						Device* device;

						string docId; // N entendi
						string filename; // N entendi

                    public:
					      User(string ID, string userGenre, string locationZip, string locationLatitude, string locationLongitude, 
						      string userBirth, string docId, string filename );
								      
					      string generateSql();					      
					      string generateSql2();
					      string generateSql3();
					      string generateSql4();
					      string generateSql5(int id);
			      
					      int getIdUser();
					      void setIdUser(int id);
					      void incIdUser();

					      string getID();
					      void setID(string ID);			      
					      
					      string getUserGenre();
					      void setUserGenre(string userGenre);

					      string getLocationZip();
					      void setLocationZip(string locationZip);

					      string getLocationLatitude();
					      void setLocationLatitude(string locationLatitude);

					      string getLocationLongitude();
					      void setLocationLongitude(string locationLongitude);

					      string getUserBirth();
					      void setUserBirth(string userBirth);

					      Device* getDevice();
					      void setDevice(Device* device);

					      string getDocId();
					      void setDocId(string docId);

					      string getFilename();
					      void setFilename(string filename);

					  };
				}
			}
		}
	}
}				

#endif /*_USER_H_*/

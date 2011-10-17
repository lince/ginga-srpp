#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <string>
#include <ginga/recommender/utils/Utils.h>
using namespace ::br::ufscar::lince::ginga::recommender::util;

using namespace std;

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {
					class Device {
					
					private:
						static int idDevice;
						string prof;
						string serialNumber;
						string tp;
						
						
					public:
						Device(string prof, string serialNumber, string tp);
						
						string generateSql();
						
						int getIdDevice();
						static void setIdDevice(int id);
						static void incIdDevice();
						
						string getProf();
						void setProf(string prof);
						
						string getSerialNumber();
						void setSerialNumber(string serialNumber);

						string getTp();
						void setTp(string tp);

					};

				}

			}

		}

	}

};

#endif /*_CONTEXT_H_*/

#include "../include/Device.h"

namespace br {
	namespace ufscar {
		namespace lince {
			namespace ginga {
				namespace recommender {

					int Device::idDevice = 1;

					Device::Device(string prof, string serialNumber, string tp) {
						this->prof=prof;
						this->serialNumber=serialNumber;
						this->tp=tp;
					}

					string Device::generateSql() {
						Utilitario* util = Utilitario::getInstance();
						return "insert into device (prof, serialNumber, tp) values ('" + prof + "', '" +
							serialNumber+ "', '" + tp + "');";
					}
					
					int Device::getIdDevice()				{return idDevice;		}
					void Device::setIdDevice(int id)			{idDevice = id;			}
					void Device::incIdDevice()				{idDevice++;			}
					
					string Device::getProf()				{return prof;			}
					void Device::setProf(string prof)			{this->prof = prof;		}
					
					string Device::getSerialNumber()			{return serialNumber;		}
					void Device::setSerialNumber(string serialNumber)	{this->serialNumber = serialNumber;}

					string Device::getTp()					{return tp;			}
					void Device::setTp(string tp)				{this->tp = tp;			}

				}
			}
		}
	}
}

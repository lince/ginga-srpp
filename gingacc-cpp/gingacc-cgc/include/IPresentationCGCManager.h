#ifndef IPRESENTATIONCGCMANAGER_H_
#define IPRESENTATIONCGCMANAGER_H_

#include <string>
using namespace std;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace cgc {

	class IPresentationCGCManager {

		public:
			virtual ~IPresentationCGCManager(){};
			//virtual void* getTuner()=0;
			virtual void close()=0;
			virtual bool openNclFile(string nclFile)=0;
			virtual void startPresentation(string formatterId)=0;
			virtual void waitUnlockCondition()=0;
			virtual void setIsLocalNcl(bool isLocal, void* tuner)=0;
			virtual void* getDsmccListener()=0;
	};
}
}
}
}
}
}

#endif /*IPRESENTATIONCGCMANAGER_H_*/

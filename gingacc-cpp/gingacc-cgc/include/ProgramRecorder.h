#ifndef PROGRAMRECORDER_H_
#define PROGRAMRECORDER_H_

#define BUFFER_SIZE    ( 188 * 10 ) + 1

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#include <map>

#include "ProgramHeader.h"
//#include "IPresentationCGCManager.h"
using namespace std;

#include "tuner/ITuner.h"
using namespace ::br::pucrio::telemidia::ginga::core::tuning;

#include "tsparser/IDemuxer.h"
#include "tsparser/ITSFilter.h"
#include "tsparser/ITSPacket.h"
#include "tsparser/Pat.h"
#include "tsparser/Pmt.h"
using namespace ::br::pucrio::telemidia::ginga::core::tsparser;
using namespace ::br::pucrio::telemidia::ginga::core::tsparser::si;

#include "gingalssm/IPresentationEngineManager.h"
using namespace ::br::pucrio::telemidia::ginga::lssm;

#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;

#include "dataprocessing/IDataProcessor.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing;

#include "ncl/generator/INclGenerator.h"
using namespace ::br::pucrio::telemidia::ncl;
using namespace ::br::ufscar::lince::ncl::generator;

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace cgc {

	class ProgramRecorder {

		private:
			static ProgramRecorder* _instance;
			IComponentManager* cm;
			IPresentationEngineManager* presentation;
			ITuner* tuner;
			IDemuxer* demuxer;
			IDataProcessor* dataProcessor;
			ITSFilter* pidFilter;
			set<ITSPacket*> packetSet;
			NclDocument* nclDocument;
			static HLoggerPtr logger;
		
		private:
			ProgramRecorder();
	
		public:
			~ProgramRecorder(){};
			
			static ProgramRecorder* getInstance();
			
			void setPresentation(IPresentationEngineManager* presentation);
			
			IPresentationEngineManager* getPresentation();
			
			ITuner* getTuner();
			
			void setTuner(ITuner* tuner);
			
			IDemuxer* getDemuxer();
			
			void setDemuxer(IDemuxer* demuxer);
			
			IDataProcessor* getDataProcessor();
			
			void setDataProcessor(IDataProcessor* dataProcessor);
			
			void insertPacket(ITSPacket* packet);
			
			void showNcl(string file);
			
			static string getExt(short type);
			
			static string completePath(unsigned int id, short type, string dir);
			
			static void listRecords(string path);
			
			static ProgramHeader* getRecord(string path);
			
			static bool removeRecord(string path);
			
			static bool removeRecords(string cdir);
			
			void setNclDocument(NclDocument* nclDocument);
			
			void salvaNcl(string path);
			
			map<unsigned int, Pmt*>* getPrograms();
			
			void listPrograms();
			
			Pmt* getProgramByPid(unsigned int pid);
			
			map<unsigned int, short>* getElStreams(Pmt* program);

			void listElStreams(Pmt* program);
			
			int createRecord(unsigned int pid, unsigned int user, string dir);
			
			void showProgram(string path);
			
	};
}
}
}
}
}
}	

#endif /*PROGRAMRECORDER_H_*/

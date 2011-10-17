#ifndef PROGRAMHEADER_H_
#define PROGRAMHEADER_H_

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
using namespace std;

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace cgc {

    class ProgramHeader {
    
        private:
			unsigned int id;
			unsigned int pcr;
			unsigned int programNumber;
			unsigned int user;
			unsigned int dia;
			unsigned int mes;
			unsigned int ano;
			unsigned int hora;
			unsigned int minuto;
			unsigned int segundo;
			string path;
			string nclFile;
			map<unsigned int, short> elStreams;
			HLoggerPtr logger;
			           
        public:
            ProgramHeader();
            
            ProgramHeader(unsigned int id, unsigned int pcr, unsigned int programNumber, unsigned int user,
            					string path, string nclFile, map<unsigned int, short> elStreams);
            
            unsigned int getId();
            
            unsigned int getPcr();
            
            unsigned int getProgramNumber();
            
            unsigned int getUser();
            
            unsigned int getDia();
            
            unsigned int getMes();
            
            unsigned int getAno();
            
            unsigned int getHora();
            
            unsigned int getMinuto();
            
            unsigned int getSegundo();
            
            string getPath();
            
            string getNclFile();
            
            map<unsigned int, short> getElstreams();
            
            void setTime();

            bool recordTxtProgramHeader();
            
            bool recordXmlProgramHeader();

            bool recordBinProgramHeader();
            
            bool readXmlProgramHeader(string opath);
            
            bool readTxtProgramHeader(string opath);
            
            void print();
    };

}
}
}
}
}
}
#endif /*PROGRAMHEADER_H_*/

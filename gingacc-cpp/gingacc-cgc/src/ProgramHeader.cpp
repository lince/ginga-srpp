#include "../include/ProgramHeader.h"

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace cgc {

    ProgramHeader::ProgramHeader()
    {
		logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.core.cgc.programheader");
    }
    
    ProgramHeader::ProgramHeader(unsigned int id, unsigned int pcr, unsigned int programNumber, unsigned int user,
    									string path, string nclFile, map<unsigned int, short> elStreams)
    {
        logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.core.cgc.programheader");
        this->id = id;
        this->pcr = pcr;
        this->programNumber = programNumber;
        this->user = user;
        this->path = path;
        this->nclFile = nclFile;
        this->elStreams = elStreams;
        this->setTime();
    }
    
    unsigned int ProgramHeader::getId()
    {
        return id;
    }
    
    unsigned int ProgramHeader::getPcr()
    {
    	return pcr;
    }
    
    unsigned int ProgramHeader::getProgramNumber()
    {
    	return programNumber;
    }
    
    unsigned int ProgramHeader::getUser()
    {
        return user;
    }

    unsigned int ProgramHeader::getDia()
    {
    	return dia;
    }
    
    unsigned int ProgramHeader::getMes()
    {
    	return mes;
    }
    
    unsigned int ProgramHeader::getAno()
    {
    	return ano;
    }
    
    unsigned int ProgramHeader::getHora()
    {
    	return hora;
    }
    
    unsigned int ProgramHeader::getMinuto()
    {
    	return minuto;
    }
    
    unsigned int ProgramHeader::getSegundo()
    {
    	return segundo;
    }

    string ProgramHeader::getPath()
    {
        return path;
    }
    
    string ProgramHeader::getNclFile()
    {
        return nclFile;
    }
    
    map<unsigned int, short> ProgramHeader::getElstreams()
    {
    	return elStreams;
    }
    
    void ProgramHeader::setTime()
    {
        time_t tempo;
        struct tm t;
        
        tempo = time(NULL);
        t = *(localtime(&tempo));
        this->ano = t.tm_year + 1900;
        this->mes = t.tm_mon + 1;
        this->dia = t.tm_mday;
        this->hora = t.tm_hour;
        this->minuto = t.tm_min;
        this->segundo = t.tm_sec;
    }

    bool ProgramHeader::recordTxtProgramHeader()
    {
    	try
    	{
    		ofstream file;
    		string name, aux;
    		stringstream sstr;
    		sstr << id;
    		aux = sstr.str();
    		aux += ".hdr";
    		name = path;
    		name += aux;
    		file.open(name.c_str(), ios::out|ios::trunc);
    		file << "#HEADER#" << endl;
		    file << "ID#" << id << endl;
		    file << "PCR#" << pcr << endl;
		    file << "PROGRAMN#" << programNumber << endl;
		    file << "USER#" << user << endl;
		    file << "PATH#" << path << endl;
		    file << "NCLFILE#" << nclFile << endl;
		    file << "DATE#" << dia << "/" << mes << "/" << ano << endl;
			file << "TIME#" << hora << ":" << minuto << ":" << segundo << endl;
			map<unsigned int, short>::iterator it;
			for(it = elStreams.begin(); it != elStreams.end(); ++it)
			{
		    	file << endl << "#SERVICE#" << endl;
			    file << "ELYD#" << (*it).first << endl;
			    file << "TYPE#" << (*it).second << endl;
		    }
		    file.close();		
    		return true;
    	}
    	catch(exception)
    	{
    		LoggerUtil_error(logger, "Error. Couldn't open the file!");
    		return false;
    	}
    }
    
    bool ProgramHeader::recordXmlProgramHeader()
    {
    	try
    	{
    		ofstream file;
    		string name, aux;
    		stringstream sstr;
    		sstr << id;
    		aux = sstr.str();
    		aux += ".xml";
    		name = path;
    		name += aux;
    		file.open(name.c_str(), ios::out|ios::trunc);
    		file << "<\?xml version=\"1.0\" encoding=\"ISO-8859-1\"\?>" << endl;
    		file << "<header>" << endl;
		    file << "\t<title>" << nclFile << "</title>" << endl;
		    file << "\t<stream id=\"" << id << "\" programn=\"" << programNumber << "\" user=\"" 
		    	<< user << "\" path=\"" << path << "\" pcr=\"" << pcr << "\" day=\"" << dia
		    	<< "\" mon=\"" << mes << "\" year=\"" << ano << "\" hour=\"" << hora << "\" min=\"" <<
		    	minuto << "\" sec=\"" << segundo << "\">" << endl;
		    
		    map<unsigned int, short>::iterator it;
			for(it = elStreams.begin(); it != elStreams.end(); ++it)
			{
				file << "\t\t<service elid=\"" << (*it).first << "\" type=\"" << (*it).second << "\" />" << endl;
			}
			file << "\t</stream>" << endl;
			file << "</header>" << endl;
		    file.close();
    		return true;
    	}
    	catch(exception)
    	{
    		LoggerUtil_error(logger, "Error. Couldn't open the file!");
    		return false;
    	}
    }
    
    bool ProgramHeader::recordBinProgramHeader()
    {
    	try
    	{
    		fstream file;
    		file.open(this->path.c_str(), ios::out|ios::binary|ios::app);
    		file.write(reinterpret_cast<char *>(this), sizeof(ProgramHeader));
    		file.close();
    		return true;
    	}
    	catch(exception)
    	{
    		LoggerUtil_error(logger, "Error. Couldn't open the file!");
    		return false;
    	}
    }
    
    bool ProgramHeader::readXmlProgramHeader(string opath)
    {
    	try
    	{
    		fstream file;
    		string line, cut, cut2;
    		size_t posi, posf, tam;
    		file.open(opath.c_str(), ios::in);
    		file.seekg(0, ios::beg);
    		while(!file.eof())
    		{
    			getline(file, line);
    			// GET NCLFILE
    			posi = line.find("<title>");
    			if(posi != string::npos)
    			{
    				posf = line.find("</title>");
    				tam = (posf) - (posi + 7);
    				cut = line.substr(posi + 7, tam);
    				this->nclFile = cut;
    			}
    			
    			// GET ID
    			posi = line.find("<stream");
    			if(posi != string::npos)
    			{
    				posf = line.find("programn");
    				tam = (posf - 2) - (posi + 12) ;
    				cut = line.substr(posi + 12, tam);
    				this->id = atoi(cut.c_str());
    			}
    			
    			// GET PROGRAMNUMBER
    			posi = line.find("programn");
    			if(posi != string::npos)
    			{
    				posf = line.find("user");
    				tam = (posf - 2) - (posi + 10) ;
    				cut = line.substr(posi + 10, tam);
    				this->programNumber = atoi(cut.c_str());
    			}
    			
    			// GET USER
    			posi = line.find("user");
    			if(posi != string::npos)
    			{
    				posf = line.find("path");
    				tam = (posf - 2) - (posi + 6) ;
    				cut = line.substr(posi + 6, tam);
    				this->user = atoi(cut.c_str());
    			}
    			
    			// GET PATH
    			posi = line.find("path");
    			if(posi != string::npos)
    			{
    				posf = line.find("pcr");
    				tam = (posf - 2) - (posi + 6) ;
    				cut = line.substr(posi + 6, tam);
    				this->path = cut;
    			}
    			
    			// GET PCR
    			posi = line.find("pcr");
    			if(posi != string::npos)
    			{
    				posf = line.find("day");
    				tam = (posf - 2) - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->pcr = atoi(cut.c_str());
    			}
    			
    			// GET DAY
    			posi = line.find("day");
    			if(posi != string::npos)
    			{
    				posf = line.find("mon");
    				tam = (posf - 2) - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->dia = atoi(cut.c_str());
    			}
    			
    			// GET MON
    			posi = line.find("mon");
    			if(posi != string::npos)
    			{
    				posf = line.find("year");
    				tam = (posf - 2) - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->mes = atoi(cut.c_str());
    			}
    			
    			// GET YEAR
    			posi = line.find("year");
    			if(posi != string::npos)
    			{
    				posf = line.find("hour");
    				tam = (posf - 2) - (posi + 6) ;
    				cut = line.substr(posi + 6, tam);
    				this->ano = atoi(cut.c_str());
    			}
    			
    			// GET HOUR
    			posi = line.find("hour");
    			if(posi != string::npos)
    			{
    				posf = line.find("min");
    				tam = (posf - 2) - (posi + 6) ;
    				cut = line.substr(posi + 6, tam);
    				this->hora = atoi(cut.c_str());
    			}
    			
    			// GET MIN
    			posi = line.find("min");
    			if(posi != string::npos)
    			{
    				posf = line.find("sec");
    				tam = (posf - 2) - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->minuto = atoi(cut.c_str());
    			}
    			
    			// GET SEC
    			posi = line.find("sec");
    			if(posi != string::npos)
    			{
    				posf = line.find(">");
    				tam = (posf - 1) - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->segundo = atoi(cut.c_str());
    			}
    			
    			// GET ELID
    			posi = line.find("elid");
    			if(posi != string::npos)
    			{
    				posf = line.find("type");
    				tam = (posf - 2) - (posi + 6) ;
    				cut = line.substr(posi + 6, tam);
    			}
    			
    			// GET TYPE
    			posi = line.find("type");
    			if(posi != string::npos)
    			{
    				posf = line.find("/>");
    				tam = (posf - 2) - (posi + 6) ;
    				cut2 = line.substr(posi + 6, tam);
    				
    				// SET ELEMENTARY STREAMS
					elStreams[atoi(cut.c_str())] = atoi(cut2.c_str());
    			}
    		}
    		file.close();
    	}
    	catch(exception)
    	{
    		LoggerUtil_error(logger, "Error. Couldn't open the file!");
    		return false;
    	}
    }
    
    bool ProgramHeader::readTxtProgramHeader(string opath)
    {
    	try
    	{
    		fstream file;
    		string line, cut, cut2;
    		size_t posi, posf, tam;
    		file.open(opath.c_str(), ios::in);
    		file.seekg(0, ios::beg);
    		while(!file.eof())
    		{
    			getline(file, line);
    			
    			// GET ID
    			posi = line.find("ID#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 3);
    				this->id = atoi(cut.c_str());
    			}
    			
    			// GET USER
    			posi = line.find("USER#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 5);
    				this->user = atoi(cut.c_str());
    			}
    			
    			// GET PATH
    			posi = line.find("PATH#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 5);
    				this->path = cut;
    			}
    			
    			// GET PCR
    			posi = line.find("PCR#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 4);
    				this->pcr = atoi(cut.c_str());
    			}
    			
    			// GET PROGRAMNUMBER
    			posi = line.find("PROGRAMN#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 9);
    				this->programNumber = atoi(cut.c_str());
    			}
    			
    			// GET NCLFILE
    			posi = line.find("NCLFILE#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 8);
    				this->nclFile = cut;
    			}
    			
    			// GET DAY
    			posi = line.find("DATE#");
    			if(posi != string::npos)
    			{
    				posf = line.find_first_of('/');
    				tam = posf - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->dia = atoi(cut.c_str());
    			}
    			
    			// GET MON
    			posi = line.find_first_of('/');
    			if(posi != string::npos)
    			{
    				posf = line.find_last_of('/');
    				tam = posf - (posi + 1);
    				cut = line.substr(posi + 1, tam);
    				this->mes = atoi(cut.c_str());
    			}
    			
    			// GET YEAR
    			posi = line.find_last_of('/');
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 1);
    				this->ano = atoi(cut.c_str());
    			}
    			
    			// GET HOUR
    			posi = line.find("TIME#");
    			if(posi != string::npos)
    			{
    				posf = line.find_first_of(':');
    				tam = posf - (posi + 5) ;
    				cut = line.substr(posi + 5, tam);
    				this->hora = atoi(cut.c_str());
    			}
    			
    			// GET MIN
    			posi = line.find_first_of(':');
    			if(posi != string::npos)
    			{
    				posf = line.find_last_of(':');
    				tam = posf - (posi + 1);
    				cut = line.substr(posi + 1, tam);
    				this->minuto = atoi(cut.c_str());
    			}
    			
    			// GET SEC
    			posi = line.find_last_of(':');
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 1);
    				this->segundo = atoi(cut.c_str());
    			}
    			
    			// GET ELID
    			posi = line.find("ELYD#");
    			if(posi != string::npos)
    			{
    				cut = line.substr(posi + 5);
    			}
    			
    			// GET TYPE
    			posi = line.find("TYPE#");
    			if(posi != string::npos)
    			{
    				cut2 = line.substr(posi + 5);
    				// SET ELEMENTARY STREAMS
					elStreams[atoi(cut.c_str())] = atoi(cut2.c_str());
    			}
    		}
    		file.close();
    	}
    	catch(exception)
    	{
    		LoggerUtil_error(logger, "Error. Couldn't open the file!");
    		return false;
    	}
    }
    
    void ProgramHeader::print()
    {
        cout << "#HEADER#" << endl;
	    cout << "ID#" << id << endl;
		cout << "PCR#" << pcr << endl;
		cout << "PROGRAMN#" << programNumber << endl;
	    cout << "USER#" << user << endl;
	    cout << "PATH#" << path << endl;
	    cout << "NCLFILE#" << nclFile << endl;
	    cout << "DATE#" << dia << "/" << mes << "/" << ano << endl;
		cout << "TIME#" << hora << ":" << minuto << ":" << segundo << endl;
		map<unsigned int, short>::iterator it;
		for(it = elStreams.begin(); it != elStreams.end(); ++it)
		{
	    	cout << endl << "#SERVICE#" << endl;
		    cout << "ELID#" << (*it).first << endl;
		    cout << "TYPE#" << (*it).second << endl;
	    }
    }

}
}
}
}
}
}


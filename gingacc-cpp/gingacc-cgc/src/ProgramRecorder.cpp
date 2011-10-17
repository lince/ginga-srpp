#include "../include/ProgramRecorder.h"

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace cgc {

	ProgramRecorder* ProgramRecorder::_instance = NULL;
	
	HLoggerPtr ProgramRecorder::logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.core.cgc.programrecorder");
	
	ProgramRecorder::ProgramRecorder()
	{
		logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.core.cgc.programrecorder");
		cm = IComponentManager::getCMInstance();
		presentation = NULL;
		tuner = NULL;
		demuxer = NULL;
		nclDocument = NULL;
	}
	
	ProgramRecorder* ProgramRecorder::getInstance()
	{
		if(_instance == NULL)
		{
			_instance = new ProgramRecorder();
		}
		return _instance;
	}
	
	void ProgramRecorder::setPresentation(IPresentationEngineManager* presentation)
	{
		this->presentation = presentation;
	}
	
	IPresentationEngineManager* ProgramRecorder::getPresentation()
	{
		return this->presentation;
	}
	
	void ProgramRecorder::showNcl(string file)
	{
		if(presentation != NULL)
		{
			if(presentation->openNclFile(file))
			{
				presentation->startPresentation(file);
				presentation->waitUnlockCondition();
			}
			else
			{
				LoggerUtil_error(logger, "Error. Couldn't open the file!");
			}
		}
		else
		{
			LoggerUtil_warning(logger, "Presentation is NULL!");
		}
	}
	
	ITuner* ProgramRecorder::getTuner()
	{
		return tuner;
	}
	
	void ProgramRecorder::setTuner(ITuner* tuner)
	{
		if(tuner != NULL)
			this->tuner = tuner;
		else
			cout << "tuner is null" << endl;
	}
	
	IDemuxer* ProgramRecorder::getDemuxer()
	{
		return demuxer;
	}
	
	void ProgramRecorder::setDemuxer(IDemuxer* demuxer)
	{
		if(demuxer != NULL)
			this->demuxer = demuxer;
	}
	
	IDataProcessor* ProgramRecorder::getDataProcessor()
	{
		return dataProcessor;
	}
	
	void ProgramRecorder::setDataProcessor(IDataProcessor* dataProcessor)
	{
		if(dataProcessor != NULL)
			this->dataProcessor = dataProcessor;
	}
	
	void ProgramRecorder::insertPacket(ITSPacket* packet)
	{
		packetSet.insert(packet);
	}
	
	string ProgramRecorder::getExt(short type)
	{
		string str;		
		switch(type)
		{
			case 0x01:
			case 0x02:
				str += ".mpeg";
				break;
			case 0x03:
			case 0x04:
				str += ".mpeg";
				break;
			case 0x05:
				str += ".pvts"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x06:
				str += ".pvtd"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x07:
				str += ".mheg"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x0A:
				str += ".dsmcca"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x0B:
				str += ".dsmccb"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x0C:
				str += ".dsmccc"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x0D:
				str += ".dsmccd"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x0F:
				str += ".m4p";
				break;
			case 0x10:
				str += ".mpeg";
				break;
			case 0x1B:
				str += ".h264"; // TODO: VERIFICAR EXTENSÃO DE ARQUIVO
				break;
			case 0x81:
				str += ".ac3";
				break;
			case 0x8A:
				str += ".dts";
				break;
			default:
				str += "";
				break;
		}
		return str;
	}
	
	string ProgramRecorder::completePath(unsigned int id, short type, string dir)
	{
		string str, str2;
		stringstream sstr;
		sstr << id;
		str2 = sstr.str();
		str = dir;
		str += str2;
		str += getExt(type);
		return str;
	}
	
	void ProgramRecorder::listRecords(string path)
	{
		DIR *dir = opendir(path.c_str());
		size_t pos;
		string aux, ext, opath;
		struct dirent *ent;
		while (ent = readdir(dir))
		{
			aux = string(ent->d_name);
			pos = aux.find('.');
			if(pos != string::npos)
			{
				ext = aux.substr(pos + 1);
				if(ext == "xml")
				{
					opath = path;
					opath += ent->d_name;
					ProgramHeader rdXml;
					rdXml.readXmlProgramHeader(opath);
					rdXml.print();
				}
				else if(ext == "hdr")
				{
					opath = path;
					opath += ent->d_name;
					ProgramHeader rdTxt;
					rdTxt.readTxtProgramHeader(opath);
					rdTxt.print();
				}
			}
		}
	}
	
	ProgramHeader* ProgramRecorder::getRecord(string path)
	{
		size_t pos;
		string ext;
		pos = path.find('.');
		if(pos != string::npos)
		{
			ext = path.substr(pos + 1);
			if(ext == "xml")
			{
				ProgramHeader* rdHdr;
				rdHdr = new ProgramHeader();
				rdHdr->readXmlProgramHeader(path);
				return rdHdr;
			}
			else if(ext == "hdr")
			{
				ProgramHeader* rdHdr;
				rdHdr = new ProgramHeader();
				rdHdr->readTxtProgramHeader(path);
				return rdHdr;
			}
			else
			{
				LoggerUtil_error(logger, "Error. Wrong file!");
			}
		}
		return NULL;
	}
	
	bool ProgramRecorder::removeRecord(string path)
	{
		size_t pos;
		string ext;
		bool ret = true;
		map<unsigned int, short> streams;
		pos = path.find('.');
		if(pos != string::npos)
		{
			ext = path.substr(pos + 1);
			if(ext == "xml")
			{
				ProgramHeader rdHdr;
				rdHdr.readXmlProgramHeader(path);
				streams = rdHdr.getElstreams();
				map<unsigned int, short>::iterator it;
				for(it = streams.begin(); it != streams.end(); ++it)
				{
					if(remove(completePath((*it).first, (*it).second, rdHdr.getPath()).c_str()) != 0)
					{
						LoggerUtil_error(logger, "Error. Couldn't delete the file!");
						if(ret == true)
							ret = false;
					}
					else
					{
						LoggerUtil_info(logger, "File successfully deleted!");
						ret = false;
					}
				}
			}
			else if(ext == "hdr")
			{
				ProgramHeader rdHdr;
				rdHdr.readTxtProgramHeader(path);
				streams = rdHdr.getElstreams();
				map<unsigned int, short>::iterator it;
				for(it = streams.begin(); it != streams.end(); ++it)
				{
					if(remove(completePath((*it).first, (*it).second, rdHdr.getPath()).c_str()) != 0)
					{
						LoggerUtil_error(logger, "Error. Couldn't delete the file!");
						if(ret == true)
							ret = false;
					}
					else
					{
						ret = false;
					}
				}
			}
			else
			{
				LoggerUtil_error(logger, "Error. Wrong file!");
			}
		}
		
		if(remove(path.c_str()) != 0)
		{
			LoggerUtil_error(logger, "Error. Couldn't delete the file!");
			if(ret == true)
				ret = false;
		}
		else
		{
			LoggerUtil_info(logger, "File successfully deleted!");
			ret = false;
		}
		
		return ret;
	}
	
	bool ProgramRecorder::removeRecords(string cdir)
	{
		DIR *dir = opendir(cdir.c_str());
		size_t pos;
		string aux, ext, opath;
		bool ret;
		struct dirent *ent;
		while (ent = readdir(dir))
		{
			aux = string(ent->d_name);
			pos = aux.find('.');
			if(pos != string::npos)
			{
				ext = aux.substr(pos + 1);
				if(ext == "xml" || ext == "hdr")
				{
					opath = cdir;
					opath += ent->d_name;
					ret = removeRecord(opath);
				}			
			}
		}
		return ret;
	}
	
	void ProgramRecorder::setNclDocument(NclDocument* nclDocument)
	{
		this->nclDocument = nclDocument;
	}
	
	void ProgramRecorder::salvaNcl(string path)
	{
		INclGenerator* nclGenerator = ((NclGeneratorCreator*)(cm->getObject("NclGenerator")))();
		nclGenerator->generateNclFile(path, nclDocument);
	}
	
	map<unsigned int, Pmt*>* ProgramRecorder::getPrograms()
	{		
		tuner = ((TunerCreator*)(cm->getObject("Tuner")))();
		LoggerUtil_info(logger, "Tuner created!");
		if(tuner != NULL)
		{
			demuxer = ((demCreator*)(cm->getObject("Demuxer")))(tuner);
			LoggerUtil_info(logger, "Demuxer created!");
			if(demuxer != NULL)
			{	
				tuner->addListener((ITunerListener*)demuxer);
				return demuxer->getPat()->getProgramsInfo();
			}
			else
			{
				LoggerUtil_warning(logger, "Demuxer is NULL!");
				return NULL;
			}
		}
		else
		{
			LoggerUtil_warning(logger, "Tuner is NULL!");
			return NULL;
		}
	}
	
	void ProgramRecorder::listPrograms()
	{		
		map<unsigned int, Pmt*>* programMap;
		programMap = getPrograms();
		
		map<unsigned int, Pmt*>::iterator mapIterator;
		for(mapIterator = programMap->begin(); mapIterator != programMap->end(); ++mapIterator)
		{
			cout << "Pid: " << (*mapIterator).first << "\tProgramNumber: " << (*mapIterator).second->getProgramNumber() << endl;
		}
	}
	
	Pmt* ProgramRecorder::getProgramByPid(unsigned int pid)
	{		
		map<unsigned int, Pmt*>* programMap;
		map<unsigned int, Pmt*>::iterator mapIterator;
		programMap = getPrograms();
		mapIterator = programMap->find(pid);
		
		return (*mapIterator).second;
	}
	
	map<unsigned int, short>* ProgramRecorder::getElStreams(Pmt* program)
	{
		return program->getElStreams();
	}
	
	void ProgramRecorder::listElStreams(Pmt* program)
	{
		map<unsigned int, short>* elStreams;
		elStreams = program->getElStreams();
		
		map<unsigned int, short>::iterator mapIterator;
		for(mapIterator = elStreams->begin(); mapIterator != elStreams->end(); ++mapIterator)
		{
			cout << "Pid: " << (*mapIterator).first << "\tType: " << program->getStreamTypeName((*mapIterator).second) << endl;
		}
	}
	
	int ProgramRecorder::createRecord(unsigned int pid, unsigned int user, string dir)
	{
		Pmt* program;
		program = getProgramByPid(pid);
		
		if(program != NULL)
		{
			string nclFile;
			stringstream sStr;
			sStr << pid;
			string str;
			str = sStr.str();
			nclFile = dir;
			nclFile += str;
			nclFile += ".ncl";
			salvaNcl(nclFile);
		
			map<unsigned int, short> elStreams;
			elStreams = *getElStreams(program);
		
			ProgramHeader* header;
			header = new ProgramHeader(pid, program->getPCRPid(), program->getProgramNumber(), user, dir, nclFile, elStreams);
		
			header->recordXmlProgramHeader();
		
			delete header;
		
			map<unsigned int, short>::iterator mapIterator;
			for(mapIterator = elStreams.begin(); mapIterator != elStreams.end(); ++mapIterator)
			{
				char* buf = NULL;
				int fd = -1;
				int numbytes = 0;
				unsigned int esPid = 0;
				string filename = "";
			
				esPid = (*mapIterator).first;
				filename = completePath(esPid, (*mapIterator).second, dir);
		
				tuner = ((TunerCreator*)(cm->getObject("Tuner")))();
				LoggerUtil_info(logger, "Tuner created!");
				if(tuner != NULL)
				{
					demuxer = ((demCreator*)(cm->getObject("Demuxer")))(tuner);
					LoggerUtil_info(logger, "Demuxer created!");
					if(demuxer != NULL)
					{	
						tuner->addListener((ITunerListener*)demuxer);
						dataProcessor = ((dpCreator*)(cm->getObject("DataProcessor")))();
						if(dataProcessor != NULL)
						{
							dataProcessor->createPidSectionFilter(esPid, demuxer);
							if ((fd = open(filename.c_str(), O_RDONLY | O_LARGEFILE)) < 0)
							{
								LoggerUtil_error(logger, "Error. Unable to open the file!");
								return -1;
							}

							if ((buf = (char*) malloc(BUFFER_SIZE)) == NULL)
							{
								LoggerUtil_error(logger, "Error. Buffer alocation failed!");
								return -1;
							}
					
							int k = 0;
							while ((numbytes = read(fd, buf, BUFFER_SIZE)) > 0)
							{
								if(numbytes < BUFFER_SIZE)
									break;
								demuxer->receiveData(buf, numbytes);
								lseek(fd,-1,SEEK_CUR);
								k = k + numbytes;
							}
						}
						else
						{
							delete dataProcessor;
							delete demuxer;
							delete tuner;
							LoggerUtil_warning(logger, "DataProcessor is NULL!");
							return -1;
						}
					}
					else
					{
						delete demuxer;
						delete tuner;
						LoggerUtil_warning(logger, "Demuxer is NULL!");
						return -1;
					}
				}
				else
				{
					delete tuner;
					LoggerUtil_warning(logger, "Tuner is NULL!");
					return -1;
				}
			}
		}
		else
		{
			delete program;
			LoggerUtil_warning(logger, "Program is NULL!");
			return -1;
		}
		
		return 0;
	}

	void ProgramRecorder::showProgram(string path)
	{
		ProgramHeader* header;
		header = getRecord(path);
		if(header)
		{
			showNcl(header->getNclFile());
		}
		else
		{
			LoggerUtil_error(logger, "Error. Wrong file!");
		}
	}

}
}
}
}
}
}

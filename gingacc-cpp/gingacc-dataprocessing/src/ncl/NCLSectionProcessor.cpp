/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "../../include/ncl/NCLSectionProcessor.h"
#include "../../include/ncl/Metadata.h"
#include "../../include/ncl/DataFile.h"

#include "../../include/ncl/parser/NCLMetadataParser.h"

#include "util/functions.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace dataprocessing {
namespace ncl {
	NCLSectionProcessor::NCLSectionProcessor() {
		dataToProcess = NULL;
		metadata      = NULL;
	}

	NCLSectionProcessor::~NCLSectionProcessor() {
		clear();
	}

	string NCLSectionProcessor::getRootUri() {
		string rootUri = "";

		if (metadata != NULL) {
			rootUri = metadata->getRootUri();
		}

		return rootUri;
	}

	void NCLSectionProcessor::clear() {
		clearMetadata();
		clearDataToProcess();
	}

	void NCLSectionProcessor::clearMetadata() {
		if (metadata != NULL) {
			delete metadata;
			metadata = NULL;
		}
	}

	void NCLSectionProcessor::clearDataToProcess() {
		vector<StreamData*>::iterator i;
		StreamData* sd;

		if (dataToProcess != NULL) {
			i = dataToProcess->begin();
			while (i != dataToProcess->end()) {
				sd = *i;
				delete[] sd->stream;
				delete sd;
				++i;
			}

			delete dataToProcess;
			dataToProcess = NULL;
		}
	}

	void NCLSectionProcessor::createDirectory(string newDir) {
		vector<string>* dirs;
		vector<string>::iterator i;
		string dir = "";

		dirs = split(newDir, "/");
		if (dirs == NULL) {
			return;
		}

		i = dirs->begin();
		while (i != dirs->end()) {
			dir = dir + "/" + *i;
			mkdir(dir.c_str(), 0777);
			//cout << "PrefetchManager::createDirectory '";
			//cout << dir << "'" << endl;
			++i;
		}

		delete dirs;
	}

	void NCLSectionProcessor::addDataToProcess(char* stream, int streamSize) {
		StreamData* sd;

		if (dataToProcess == NULL) {
			dataToProcess = new vector<StreamData*>;
		}

		sd         = new StreamData;
		sd->stream = stream;
		sd->size   = streamSize;
		dataToProcess->push_back(sd);
	}

	void NCLSectionProcessor::processDataFile(char* stream, int streamSize) {
		int structId, fd, bw;
		IDataFile* df;
		string fileUri;
		string baseUri;

		structId = (((unsigned char)stream[1]) & 0xFF);
		df       = metadata->getDataFile(structId);
		fileUri  = df->getUri();
		if (!isAbsolutePath(fileUri)) {
			baseUri = metadata->getBaseUri();
			fileUri = baseUri + "/" + fileUri;
		}

		fileUri = updatePath(fileUri);
		createDirectory(fileUri.substr(0, fileUri.find_last_of("/")));

		cout << "NCLSectionProcessor::processDataFile '" << fileUri;
		cout << "'" << endl;

		remove((char*)(fileUri.c_str()));
		fd = open(fileUri.c_str(), O_LARGEFILE | O_WRONLY | O_CREAT);
		if (fd > 0) {
			bw = write(fd, stream + 2, df->getSize());
			close(fd);

		} else {
			cout << "NCLSectionProcessor::processDataFile Warning! ";
			cout << " can't create file '" << fileUri << "'";
			cout << endl;
		}
		delete[] stream;
	}

	void NCLSectionProcessor::process(char* stream, int streamSize) {
		int structType = (((unsigned char)stream[0]) & 0xFF);

		switch (structType) {
			case INCLStructure::ST_METADA:
				clearMetadata();
				metadata = NCLMetadataParser::parse(stream + 2, streamSize - 2);
				break;

			case INCLStructure::ST_DATAFILE:
				if (metadata == NULL) {
					addDataToProcess(stream, streamSize);

				} else {
					processDataFile(stream, streamSize);
				}
				break;

			case INCLStructure::ST_EVENTMAP:
				//TODO: EVENT-MAP
				break;

			default:
				cout << "NCLSectionProcessor::process Warning! Unrecognized";
				cout << " structure type: '" << structType << "'";
				cout << endl;
				break;
		}
	}

	void NCLSectionProcessor::mount() {
		StreamData* streamData;
		vector<StreamData*>::iterator i;

		if (metadata == NULL) {
			cout << "NCLSectionProcessor::mount Warning! can't mount:";
			cout << " metadata(" << metadata << ")" << endl;

		} else if (dataToProcess != NULL) {
			i = dataToProcess->begin();
			while (i != dataToProcess->end()) {
				streamData = *i;
				processDataFile(streamData->stream, streamData->size);
				delete streamData;
				++i;
			}
			dataToProcess->clear();
			delete dataToProcess;
			dataToProcess = NULL;
		}
	}

	bool NCLSectionProcessor::isConsolidated() {
		if (metadata == NULL) {
			return false;
		}

		return metadata->isConsolidated();
	}

	IMetadata* NCLSectionProcessor::getMetadata() {
		return metadata;
	}

	//creating stream
	vector<StreamData*>* NCLSectionProcessor::createNCLSections(
			string componentTag,
			string name,
			string baseUri,
			vector<string>* files,
			map<int, string>* eventMap) {

		vector<StreamData*>* streams;
		vector<string>::iterator i;

		int baseUriLen, fileUriLen;
		int fileSize;
		int structureId = 0;
		string fileUri;
		IDataFile* dataFile;

		clear();
		metadata = new Metadata(name);
		metadata->setBaseUri(baseUri);

		baseUriLen = baseUri.length();

		i = files->begin();
		while (i != files->end()) {
			fileUri = *i;

			fileUriLen = fileUri.length();
			if (fileUriLen > baseUriLen &&
					fileUri.substr(0, baseUriLen) == baseUri) {

				fileUri = fileUri.substr(
						baseUriLen, fileUriLen - baseUriLen);
			}

			structureId++;

			if (isAbsolutePath(fileUri)) {
				fileSize = Metadata::getFileSize(fileUri);

			} else {
				fileSize = Metadata::getFileSize(
						updatePath(baseUri + "/" + fileUri));
			}
			cout << "NCLSectionProcessor::createNCLSections file '";
			cout << fileUri << "' has '" << fileSize << "' bytes (base";
			cout << " uri is '" << baseUri << "'" << endl;

			if (fileSize > 0) {
				dataFile = new DataFile(structureId);
				dataFile->setComponentTag(componentTag);
				dataFile->setSize(fileSize);
				dataFile->setUri(fileUri);

				if (fileUri.length() > 4 &&
						fileUri.substr(fileUri.length() - 4, 4) == ".ncl") {

					metadata->setRootObject(dataFile);

				} else {
					metadata->addDataFile(dataFile);
				}

			} else {
				cout << "NCLSectionProcessor::prepareFilesToNCLSections ";
				cout << "Warning! Can't include file: '" << fileUri;
				cout << "'" << endl;
			}

			++i;
		}

		streams = metadata->createNCLSections();
		clear();

		//TODO: map-event stream
		return streams;
	}
}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::dataprocessing::ncl::
		INCLSectionProcessor* createNCLSectionProcessor() {

	return (new ::br::pucrio::telemidia::ginga::core::dataprocessing::ncl::
			NCLSectionProcessor());
}

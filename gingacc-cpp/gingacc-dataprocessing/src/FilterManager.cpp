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

#include "../include/FilterManager.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace dataprocessing {
	FilterManager::FilterManager() {
		this->sections          = new
				map<string, map<unsigned int, ITransportSection*>*>;

		this->streamTypeFilters = new map<short, SectionFilter*>;
		this->pidFilters        = new map<int, SectionFilter*>;
		this->reading           = false;

		pthread_mutex_init(&filterMutex, NULL);
	}

	FilterManager::~FilterManager() {
		//TODO: delete maps content

		pthread_mutex_lock(&filterMutex);
		if (streamTypeFilters != NULL) {
			delete streamTypeFilters;
			streamTypeFilters = NULL;
		}

		if (pidFilters != NULL) {
			delete pidFilters;
			pidFilters = NULL;
		}

		if (sections != NULL) {
			delete sections;
			sections = NULL;
		}

		pthread_mutex_unlock(&filterMutex);
		pthread_mutex_destroy(&filterMutex);
	}

	ITSFilter* FilterManager::createStreamTypeSectionFilter(
			short streamType, IDemuxer* demux, IFilterListener* listener) {

		SectionFilter* sf = NULL;
		int tId;

		pthread_mutex_lock(&filterMutex);
		if (streamTypeFilters->count(streamType) == 0) {
			sf = new SectionFilter();
			sf->setListener(listener);

			(*streamTypeFilters)[streamType] = sf;
			if ((demux->getCaps() & DPC_CAN_DEMUXBYHW) &&
					(demux->getCaps() & DPC_CAN_FILTERPID) &&
					(demux->getCaps() & DPC_CAN_FILTERTID)) {

				tId = IDemuxer::getTableIdFromStreamType(streamType);
				if (tId >= 0) {
					demux->addFilter(sf, -1, tId);
				}

			} else {
				demux->addStreamTypeFilter(streamType, sf);
			}
		}

		pthread_mutex_unlock(&filterMutex);

		return sf;
	}

	ITSFilter* FilterManager::createPidSectionFilter(
			int pid, IDemuxer* demux, IFilterListener* listener) {

		SectionFilter* sf = NULL;

		pthread_mutex_lock(&filterMutex);
		if (pidFilters->count(pid) == 0) {
			sf = new SectionFilter();
			sf->setListener(listener);

			(*pidFilters)[pid] = sf;

			if ((demux->getCaps() & DPC_CAN_DEMUXBYHW) &&
					(demux->getCaps() & DPC_CAN_FILTERPID) &&
					(demux->getCaps() & DPC_CAN_FILTERTID)) {

				demux->addFilter(sf, pid, -1);

			} else {
				demux->addPidFilter(pid, sf);
			}
		}

		pthread_mutex_unlock(&filterMutex);
		return sf;
	}

	void FilterManager::destroyFilter(IDemuxer* demux, short streamType) {
		SectionFilter* sf;
		map<short, SectionFilter*>::iterator i;

		cout << "FilterManager::destroyFilter type '";
		cout << streamType << "'" << endl;

		pthread_mutex_lock(&filterMutex);
		i = streamTypeFilters->find(streamType);
		if (i != streamTypeFilters->end()) {
			sf = i->second;
			streamTypeFilters->erase(i);
			demux->removeFilter(sf);
		}

		pthread_mutex_unlock(&filterMutex);
	}

	void FilterManager::destroyFilter(IDemuxer* demux, ITSFilter* filter) {
		map<short, SectionFilter*>::iterator i;

		cout << "FilterManager::destroyFilter filter '";
		cout << filter << "'" << endl;

		pthread_mutex_lock(&filterMutex);
		i = streamTypeFilters->begin();
		while (i != streamTypeFilters->end()) {
			if (filter == i->second) {
				streamTypeFilters->erase(i);
				break;
			}
		}

		demux->removeFilter(filter);

		pthread_mutex_unlock(&filterMutex);
	}

	//TODO: destroy pid filter

	bool FilterManager::processSection(ITransportSection* section) {
		int sectionFd;
		int bytesSaved;
		string sectionName;
		string::size_type len;
		unsigned int sn, lsn; //sectionNumber and lastSectionNumber
		ITransportSection* sec;
		map<unsigned int, ITransportSection*>* secs;
		unsigned int i;

		if (section == NULL) {
			cout << "FilterManager::processSection ";
			cout << "Warning! Section is NULL" << endl;
			return false;
		}

		if (section->getPayloadSize() <= 0 ||
			    section->getPayloadSize() > 4096) {

			cout << "FilterManager::processSection ";
			cout << "Warning! Wrong payloadSize" << endl;
			return false;
		}

		sectionName = section->getSectionName();
		len = sectionName.length();
		if (sectionName == "") {
			cout << "FilterManager::processSection ";
			cout << "Warning! Empty section name" << endl;
			return false;

		} else if ((len < 8 || sectionName.substr(0,8) != "carousel") &&
				(len < 3 || sectionName.substr(0,3) != "epg")) {

			cout << "FilterManager::processSection ";
			cout << "Warning! Wrong section name: ";
			cout << sectionName.c_str() << endl;
			return false;
		}

		pthread_mutex_lock(&filterMutex);
		if (sections->count(sectionName) == 0) {
			cout << "FilterManager::processSection creating map for section '";
			cout << sectionName << "'" << endl;
			secs = new map<unsigned int, ITransportSection*>;
			(*sections)[sectionName] = secs;

		} else {
			secs = (*sections)[sectionName];
		}
		pthread_mutex_unlock(&filterMutex);

		sn = section->getSectionNumber();
		lsn = section->getLastSectionNumber();
		if (secs->count(sn) != 0) {
			_warn("FilterManager Warning! Adding section %s in an existent "
				  "position=%d\n", sectionName.c_str(), sn);

			return false;

		} else {
			(*secs)[sn] = section;
		}

		// All sections received.
		if (secs->size() == (lsn + 1)) {
			sectionFd = open(sectionName.c_str(),
					 O_CREAT|O_WRONLY|O_LARGEFILE|O_APPEND|O_SYNC, 0777);

			if (sectionFd < 0) {
				cout << "FilterManager Warning! error open file ";
				cout << sectionName.c_str();
				cout << "." << endl;
				return false;
			}

			i = 0;
			while (i <= lsn) {
				if (secs->count(i) == 0) {
					cout << "FilterManager Warning! cant find pos '";
					cout << i << "'" << endl;
					close(sectionFd);
					return false;
				}

				sec = (*secs)[i];
				bytesSaved = write(
						sectionFd,
						sec->getPayload(), sec->getPayloadSize());

				if (bytesSaved != (int)(sec->getPayloadSize())) {
					cout << "FilterManager Warning! bytesSaved = ";
					cout << bytesSaved << " dataSize = ";
					cout << sec->getPayloadSize() << endl;
				}

				i++;
				delete sec;
				sec = NULL;
			}
			close(sectionFd);

			addProcessedSection(sectionName);

			delete secs;
			secs = NULL;

			cout << "FilterManager::processSection all sections ";
			cout << "received and processed for '";
			cout << sectionName << "'" << endl;

			return true;
		}

		return false;
	}

	void FilterManager::addProcessedSection(string sectionName) {
		map<short, SectionFilter*>::iterator i;
		map<int, SectionFilter*>::iterator j;
		SectionFilter* sf;

		pthread_mutex_lock(&filterMutex);
		i = streamTypeFilters->begin();
		while (i != streamTypeFilters->end()) {
			cout << "FilterManager::addProcessedSection '";
			cout << sectionName << "'" << endl;
			sf = i->second;
			sf->addProcessedSection(sectionName);
			++i;
		}

		j = pidFilters->begin();
		while (j != pidFilters->end()) {
			(j->second)->addProcessedSection(sectionName);
			++j;
		}

		if (sections->count(sectionName) != 0) {
			sections->erase(sections->find(sectionName));
		}
		pthread_mutex_unlock(&filterMutex);
		/*cout << "FilterManager::addProcessedSection '" << sectionName << "'";
		cout << " done!" << endl;*/
	}
}
}
}
}
}
}

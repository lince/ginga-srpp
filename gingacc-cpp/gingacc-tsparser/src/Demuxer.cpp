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

#include "../include/Demuxer.h"

#include "../include/PipeFilter.h"
#include "../include/PSIFilter.h"
#include "../include/PesFilter.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace tsparser {
	vector<Pat*>* Demuxer::pats = new vector<Pat*>;
	unsigned int Demuxer::sectionPid = 0;

	set<unsigned int>* Demuxer::knownSectionPids = new set<unsigned int>;

	Demuxer::Demuxer(ITuner* tuner) {
		createPSI();
		createMaps();

		this->tuner = tuner;
		this->tuner->addListener(this);

		audioFilter = NULL;
		videoFilter = NULL;

		knownSectionPids->insert(PAT_PID);
		knownSectionPids->insert(CAT_PID);
		knownSectionPids->insert(TSDT_PID);
		knownSectionPids->insert(NIT_PID);
		knownSectionPids->insert(SDT_PID);
		knownSectionPids->insert(EIT_PID);
		knownSectionPids->insert(CDT_PID);

		isWaitingPI    = false;
		debugPackCount = 1;
		debugDest      = 0;
		pthread_cond_init(&flagCondSignal, NULL);
		pthread_mutex_init(&flagLockUntilSignal, NULL);
	}

	Demuxer::~Demuxer() {
		clearPSI();
		clearMaps();
	}

	void Demuxer::createPSI() {
		this->pat  = new Pat();
		this->pmts = new map<unsigned int, Pmt*>;
	}

	void Demuxer::clearPSI() {
		map<unsigned int, Pmt*>::iterator i;

		if (pat != NULL) {
			delete pat;
			pat = NULL;
		}

		if (pmts != NULL) {
			i = pmts->begin();
			while (i != pmts->end()) {
				delete i->second;
				++i;
			}

			delete pmts;
			pmts = NULL;
		}
	}

	void Demuxer::createMaps() {
		this->pidFilters       = new map<unsigned int, ITSFilter*>;
		this->stFilters        = new map<short, ITSFilter*>;
		this->pesFilters       = new map<unsigned int, ITSFilter*>;

		this->feFilters        = new set<IFrontendFilter*>;
		this->feFiltersToSetup = new set<IFrontendFilter*>;
	}

	void Demuxer::clearMaps() {
		map<unsigned int, ITSFilter*>::iterator i;
		map<short, ITSFilter*>::iterator j;
		set<IFrontendFilter*>::iterator k;

		if (stFilters != NULL) {
			j = stFilters->begin();
			while (j != stFilters->end()) {
				delete j->second;
				++j;
			}

			delete stFilters;
			stFilters = NULL;
		}

		if (pidFilters != NULL) {
			i = pidFilters->begin();
			while (i != pidFilters->end()) {
				delete i->second;
				++i;
			}

			delete pidFilters;
			pidFilters = NULL;
		}

		if (pesFilters != NULL) {
			i = pesFilters->begin();
			while (i != pesFilters->end()) {
				delete i->second;
				++i;
			}

			delete pesFilters;
			pesFilters = NULL;
		}

		if (feFilters != NULL) {
			k = feFilters->begin();
			while (k != feFilters->end()) {
				removeFilter(*k);
				++k;
			}

			delete feFilters;
			feFilters = NULL;
		}

		if (feFiltersToSetup != NULL) {
			k = feFiltersToSetup->begin();
			while (k != feFiltersToSetup->end()) {
				delete (*k);
				++k;
			}

			delete feFiltersToSetup;
			feFiltersToSetup = NULL;
		}
	}

	void Demuxer::resetDemuxer() {
		clearPSI();
		clearMaps();
		createPSI();
		createMaps();
	}

	void Demuxer::removeFilter(IFrontendFilter* filter) {
		INetworkInterface* ni = tuner->getCurrentInterface();

		if (ni != NULL && (ni->getCaps() & DPC_CAN_FILTERPID)) {
			ni->removeFilter(filter);
		}
	}

	void Demuxer::setupUnsolvedFilters() {
		set<IFrontendFilter*>::iterator i;
		INetworkInterface* ni;
		IFrontendFilter* filter;
		int aPid, vPid;

		i = feFiltersToSetup->begin();
		while (i != feFiltersToSetup->end()) {
			filter = *i;

			if (setupFilter(filter)) {
				feFiltersToSetup->erase(i);
				i = feFiltersToSetup->begin();

			} else {
				++i;
			}
		}

		if (audioFilter != NULL && videoFilter != NULL) {
			aPid = pat->getDefaultMainAudioPid();
			vPid = pat->getDefaultMainVideoPid();

			if (aPid != 0 && vPid != 0) {
				cout << "Demuxer::setupUnsolvedFilters aPid = '";
				cout << aPid << "' vPid = '" << vPid;
				cout << "'" << endl;

				ni = tuner->getCurrentInterface();
				if (ni != NULL) {
					audioFilter->setPid(aPid);
					videoFilter->setPid(vPid);

					ni->attachFilter(audioFilter);
					ni->attachFilter(videoFilter);

					feFilters->insert(audioFilter);
					feFilters->insert(videoFilter);

					audioFilter = NULL;
					videoFilter = NULL;
				}

			} else {
				cout << "Demuxer::setupUnsolvedFilters can't solve A/V PIDs";
				cout << endl;
			}
		}
	}

	bool Demuxer::setupFilter(IFrontendFilter* filter) {
		Pmt* pmt;
		vector<unsigned int>* pids;
		vector<unsigned int>::iterator i;
		unsigned int pid;
		int tid;
		bool attached = false;

		if (pmts->empty()) {
			pmt = NULL;
		} else {
			pmt = pmts->begin()->second;
		}

		if (pmt != NULL) {
			tid = filter->getTid();
			pids = pmt->getPidsByTid(tid);

			i = pids->begin();
			while (i != pids->end()) {
				pid = *i;
				if (pid > 0 || (pid == 0 && tid == 0)) {
					if (filter != NULL) {
						filter->setPid(pid);
						filter->setTid(-1);
						feFilters->insert(filter);
						attachFilter(filter);

					} else {

					}
					attached = true;

				} else {
					cout << "Demuxer::setupFilter can't getPidByTid pid = '";
					cout << pid << "' tid = '" << filter->getTid() << "'";

					cout << " PMT print: " << endl;
					pmt->print();
				}

				++i;
			}

		} else {
			cout << "Demuxer::setupFilter can't get pmt" << endl;
		}

		return attached;
	}

	void Demuxer::demux(ITSPacket* packet) {
		unsigned int pid, newVer, currVer;
		short streamType;
		vector<unsigned int>* pids;
		vector<unsigned int>::iterator i;
		Pmt* pmt, * newPmt;
		char tsPacketPayload[184];

		/*if (packet->getStartIndicator()) {
			cout << "Demuxer::demux filtred packet number '";
			cout << debugPackCount;
			cout << "'" << endl;
		}
		debugPackCount++;*/

		pid = packet->getPid();

		/* Verifies if the PID is for a PAT */
		if (pid == 0x00) {
			if (pat->isConsolidated()) {
				/* Verifies if the PAT is OK */
				if(!pat->hasUnprocessedPmt()){
					if (isWaitingPI) {
						/* Free the mutex that is waiting for the PAT */
						pthread_cond_signal(&flagCondSignal);
					}
				}
				//TODO: handle pat updates

			/* PAT is not consolidated yet ! */
			} else if (packet->getAdaptationFieldControl() == 1 ||
					    packet->getAdaptationFieldControl() == 3) {

				packet->getPayload(tsPacketPayload);
				pat->process(tsPacketPayload, 184); /* Mount PAT HEADER */
				if (pat->processSectionPayload()) { /* Mount unProcessed PMTs */
					_debug("Demuxer::demux - PAT mounted!\n");
					addPat(pat);
					pids = pat->getUnprocessedPmtPids();
					i = pids->begin();
					while (i != pids->end()) { /* Create each PMT */
						pmt = new Pmt(*i, pat->getProgramNumberByPid(*i));
						(*pmts)[*i] = pmt;
						++i;
					}
				}
			}

		/* If is not a PAT, verifies if it is a PMT */
		} else if (pmts->count(pid) != 0) {
			packet->getPayload(tsPacketPayload);
			pmt = (*pmts)[pid];
			if (pmt->isConsolidated()) { /* If the PMT is OK, try update the PMT */
				newPmt = new Pmt(pid, pmt->getProgramNumber());
				newPmt->process(tsPacketPayload, 184);
				if (newPmt->processSectionPayload()) { /* Process the new PMT */
					newVer = newPmt->getVersionNumber();
					currVer = pmt->getVersionNumber();
					if (newVer != currVer) { /* If the version is different update */
						cout << "demuxer replace pmt id = '" << pid << "'";
						cout << " newVer = '" << newVer;
						cout << "' currVer = '" << currVer << "': ";
						newPmt->print();
						cout << endl;
						pat->replacePmt(pid, newPmt);
						(*pmts)[pid] = newPmt;

					} else { /* If the version is the same ignores */
						//_debug("Demuxer:demux - found a equal PMT. Ignoring\n");
						delete newPmt;
					}

				} else {
					//TODO: make possible to have pmt carried in two or more
					// ts packets
					_warn("Warning! Demuxer::demux - make possible to have "
					      "PMT carried in two or more ts packets.\n");
					delete newPmt;
				}

			} else if (!pmt->hasProcessed()) { /* Tries to consolidate the PMT */
				pmt->process(tsPacketPayload, 184);
				if (pmt->processSectionPayload()) {
					_debug("Demuxer:demux - PMT pid=%d processed!\n",
							pmt->getPid());
					pat->addPmt(pmt);
				}
			}

		} else if (pidFilters->count(pid) != 0) { /* Its not a PAT or PMT */
			pidFilters->find(pid)->second->receiveTSPacket(packet);

		} else if (stFilters->size() > 0) { /* Its not a PAT or PMT */
			streamType = pat->getStreamType(pid);
			if (stFilters->count(streamType) != 0) {
				stFilters->find(streamType)->second->receiveTSPacket(packet);

			} else if (stFilters->count(debugDest) != 0) {
				stFilters->find(debugDest)->second->receiveTSPacket(packet);
			}

		} else if (pesFilters->count(0) != 0) {
			(*pesFilters)[0]->receiveTSPacket(packet);
		}

		delete packet;
		packet = NULL;
	}

	map<unsigned int, Pmt*>* Demuxer::getProgramsInfo() {
		return pat->getProgramsInfo();
	}

	unsigned int Demuxer::getTSId() {
		return pat->getTSId();
	}

	Pat* Demuxer::getPat() {
		return pat;
	}

	int Demuxer::getDefaultMainVideoPid() {
		while (!pat->isConsolidated() || pat->hasUnprocessedPmt()) {
			if (pat->isConsolidated()) {
				pat->checkConsistency();
			}
			usleep(1000);
		}

		return pat->getDefaultMainVideoPid();
	}

	int Demuxer::getDefaultMainAudioPid() {
		while (!pat->isConsolidated() || pat->hasUnprocessedPmt()) {
			if (pat->isConsolidated()) {
				pat->checkConsistency();
			}
			usleep(1000);
		}

		return pat->getDefaultMainAudioPid();
	}

	void Demuxer::setDestination(short int streamType) {
		cout << "Demuxer::setDestination '" << streamType << "'" << endl;
		this->debugDest = streamType;
	}

	void Demuxer::removeFilter(ITSFilter* tsFilter) {
		FrontendFilter* filter;
		set<IFrontendFilter*>::iterator i;

		i = feFilters->begin();
		while (i != feFilters->end()) {
			filter = (FrontendFilter*)(*i);
			if (filter->getTSFilter() == tsFilter) {
				feFilters->erase(i);
				removeFilter(filter);
				//delete tsFilter;
				break;
			}
			++i;
		}
	}

	void Demuxer::addFilter(ITSFilter* tsFilter, int pid, int tid) {
		IFrontendFilter* filter;

		if (pid == -1 && tid == -1) {
			delete tsFilter;
			tsFilter = NULL;
			return;
		}

		filter = new FrontendFilter(tsFilter);
		filter->setPid(pid);
		filter->setTid(tid);

		addFilter(filter);
	}

	void Demuxer::addFilter(IFrontendFilter* filter) {
		if (filter->getPid() != -1) {
			filter->setTid(-1);
			feFilters->insert(filter);
			attachFilter(filter);

		} else if (!setupFilter(filter)) {
			feFiltersToSetup->insert(filter);
		}
	}

	void Demuxer::attachFilter(IFrontendFilter* filter) {
		INetworkInterface* ni;

		ni = tuner->getCurrentInterface();
		if (ni != NULL) {
			ni->attachFilter(filter);
		}
	}

	void Demuxer::createPatFilter(INetworkInterface* ni) {
		IFrontendFilter* ff;

		ff = new PSIFilter(this);
		ff->setPid(PAT_PID);
		ff->setTid(PAT_TID);

		ni->attachFilter(ff);
		feFilters->insert(ff);
	}

	void Demuxer::createPmtFilter(INetworkInterface* ni) {
		IFrontendFilter* ff;
		map<unsigned int, Pmt*>::iterator i;

		if (pmts == NULL || pmts->empty()) {
			return;
		}

		i = pmts->begin();
		while (i != pmts->end()) {
			ff = new PSIFilter(this);
			ff->setPid(i->first);
			ff->setTid(PMT_TID);

			ni->attachFilter(ff);
			feFilters->insert(ff);
			++i;
		}
	}

	void Demuxer::receiveSection(
			char* section, int secLen, IFrontendFilter* f) {

		unsigned int pid, newVer, currVer;
		vector<unsigned int>* pids;
		vector<unsigned int>::iterator i;
		Pmt* pmt, * newPmt;
		INetworkInterface* ni;

		pid = f->getPid();
		if (pid == 0x00) {
			if (pat->isConsolidated()) {
				//TODO: handle pat updates

			} else {
				pat->process(section, secLen);
				if (pat->processSectionPayload()) {
					addPat(pat);
					pids = pat->getUnprocessedPmtPids();
					i = pids->begin();
					while (i != pids->end()) {
						pmt = new Pmt(*i, pat->getProgramNumberByPid(*i));
						(*pmts)[*i] = pmt;
						++i;
					}
					cout << "Demuxer::receiveSection PAT mounted" << endl;
					ni = tuner->getCurrentInterface();
					if (ni != NULL) {
						ni->removeFilter(f);
						createPmtFilter(ni);
					}
				}
			}

		} else if (pmts->count(pid) != 0) {
			pmt = (*pmts)[pid];
			if (pmt->isConsolidated()) {
				newPmt = new Pmt(pid, pmt->getProgramNumber());
				newPmt->process(section, secLen);
				if (newPmt->processSectionPayload()) {
					newVer = newPmt->getVersionNumber();
					currVer = pmt->getVersionNumber();
					if (newVer != currVer) {
						cout << "Demuxer::receiveSection ";
						cout << "replace pmt id = '" << pid << "'";
						cout << " newVer = '" << newVer;
						cout << "' currVer = '" << currVer << "': ";
						newPmt->print();
						cout << endl;
						pat->replacePmt(pid, newPmt);
						(*pmts)[pid] = newPmt;

					} else {
						delete newPmt;
					}

				} else {
					delete newPmt;
				}

			} else if (!pmt->hasProcessed()) {
				pmt->process(section, secLen);
				if (pmt->processSectionPayload()) {
					cout << "Demuxer::receiveSection call pat->addPmt";
					cout << endl;

					pmt->print();

					pat->addPmt(pmt);
					ni = tuner->getCurrentInterface();
					if (ni != NULL) {
						ni->removeFilter(f);
					}

					if (!pat->hasUnprocessedPmt()) {
						cout << "Demuxer::receiveSection trying to solve ";
						cout << "unsolved filters";
						cout << endl;

						if (isWaitingPI) {
							pthread_cond_signal(&flagCondSignal);
						}

						setupUnsolvedFilters();
					}
				}
			}
		}
	}

	void Demuxer::addPidFilter(unsigned int pid, ITSFilter* filter) {
		(*pidFilters)[pid] = filter;
	}

	void Demuxer::addSectionFilter(unsigned int tid, ITSFilter* filter) {

	}

	void Demuxer::addStreamTypeFilter(short streamType, ITSFilter* filter) {
		(*stFilters)[streamType] = filter;
		cout << "Demuxer::addStreamTypeFilter '" << streamType << "'" << endl;
	}

	void Demuxer::addPesFilter(short type, ITSFilter* filter) {
		INetworkInterface* ni;
		int vPid, aPid, pPid;

		ni = tuner->getCurrentInterface();
		if (ni == NULL) {
			return;
		}

		if (type == PFT_DEFAULTTS) {
			if (ni->getCaps() & DPC_CAN_FILTERPID) {
				vPid = pat->getDefaultMainVideoPid();
				aPid = pat->getDefaultMainAudioPid();
				pPid = pat->getDefaultProgramPid();

				cout << "Demuxer::addPesFilter aPid = '" << aPid << "'";
				cout << " vPid = '" << vPid << "'" << endl;

				ni->createPesFilter(0x00, PFT_OTHER, true);
				ni->createPesFilter(pPid, PFT_OTHER, true);
				ni->createPesFilter(aPid, PFT_AUDIO, true);
				ni->createPesFilter(vPid, PFT_VIDEO, true);

				if (!((PipeFilter*)filter)->isDVRReader()) {
					((PipeFilter*)filter)->setDVRReader(
							0,
							true,
							ni->getPesFilterOutput());

					((PipeFilter*)filter)->start();
				}

				(*pesFilters)[pat->getFirstProgramNumber()] = filter;

			} else {
				if (pesFilters->find(0) == pesFilters->end()) {
					(*pesFilters)[0] = filter;

				} else {
					delete filter;
				}
			}

		} else if (type == PFT_PCR) {


		} else if (type == PFT_VIDEO) {
			addVideoFilter(0, filter);
		}
	}

	void Demuxer::addVideoFilter(unsigned int pid, ITSFilter* f) {
		IFrontendFilter* ff;
		unsigned int convPid = pid;
		INetworkInterface* ni;

		ni = tuner->getCurrentInterface();
		if (ni == NULL) {
			return;
		}

		if (pid == 0) {
			convPid = pat->getDefaultMainVideoPid();
		}

		ff = new PesFilter(f);
		ff->setPid(convPid);

		ni->attachFilter(ff);
		feFilters->insert(ff);
	}

	void Demuxer::receiveData(char* buff, unsigned int size) {
		TSPacket* packet;
		unsigned int i = 0;

		while (i < size) {
			// Check TS packet boundaries.
			if (((buff[i] & 0xFF) == 0x47) &&
					((i + 188 == size) || ((buff[i + 188] & 0xFF) == 0x47))) {

				packet = new TSPacket(buff + i);
				if (packet->isConstructionFailed()) {
					_warn("Demuxer::receiveData - construction of TSPacket "
						  "failed!\n");

					delete packet;
					packet = NULL;

				} else {
					demux(packet);
				}
				i = i + 188;

			} else if (i + 188 < size) {
				/*cout << "Demuxer::receiveData hunting when i = '";
				cout << i << "' and size = '" << size << "'";
				cout << " current byte value = '" << (buff[i] & 0xFF);
				cout << "' next sync = '" << (buff[i + 188] & 0xFF);
				cout << "'" << endl;*/

				i++;
				i = i + hunt(buff + i, size - i);

			} else {
				/*cout << "Demuxer::receiveData breaking when i = '";
				cout << i << "' and size = '" << size << "'" << endl;*/
				break;
			}
		}
	}

	void Demuxer::updateStatus(short newStatus, IChannel* channel) {
		INetworkInterface* ni;

		switch (newStatus) {
			case TS_SWITCHING_CHANNEL:
				resetDemuxer();
				break;

			case TS_NEW_CHANNEL_TUNED:
				ni = tuner->getCurrentInterface();
				if (ni->getCaps() & DPC_CAN_FILTERPID) {
					createPatFilter(ni);
				}
				break;

			case TS_NEW_SERVICE_TUNED:
				break;

			case TS_SWITCHING_INTERFACE:
				break;

			case TS_NEW_INTERFACE_SELECTED:
				break;
		}
	}

	void Demuxer::addPat(Pat* pat) {
		pats->push_back(pat);
	}

	bool Demuxer::isSectionStream(unsigned int pid) {
		vector<Pat*>::iterator i;

		if (pid == Demuxer::sectionPid || knownSectionPids->count(pid) != 0) {
			return true;
		}

		i = pats->begin();
		while (i != pats->end()) {
			if ((*i)->isSectionType(pid)) {
				return true;
			}
			++i;
		}
		return false;
	}

	void Demuxer::setSectionPid(unsigned int pid) {
		Demuxer::sectionPid = pid;
	}

	unsigned int Demuxer::hunt(char* buff, unsigned int size) {
		unsigned int ix = 0;

		while (ix < size) {
			if ((buff[ix] & 0xFF) == 0x47) {
				break;
			}
			ix++;
		}
		return ix;
	}

	short Demuxer::getCaps() {
		INetworkInterface* ni;

		ni = tuner->getCurrentInterface();
		if (ni != NULL) {
			return ni->getCaps();

		} else {
			cout << "Demuxer::getCaps return 0 (NULL NI)" << endl;
		}

		return 0;
	}

	bool Demuxer::waitProgramInformation() {
		isWaitingPI = true;
		pthread_mutex_lock(&flagLockUntilSignal);
		pthread_cond_wait(&flagCondSignal, &flagLockUntilSignal);
		isWaitingPI = false;
		pthread_mutex_unlock(&flagLockUntilSignal);
		return true;
	}

	bool Demuxer::waitBuffers() {
		::usleep(3000000);
		return true;
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::tsparser::IDemuxer*
		createDemuxer(ITuner* tuner) {

	return new ::br::pucrio::telemidia::ginga::core::tsparser::Demuxer(
			tuner);
}

extern "C" void destroyDemuxer(
		::br::pucrio::telemidia::ginga::core::tsparser::IDemuxer* dem) {

	delete dem;
}

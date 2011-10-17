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

#include "../config.h"
#if HAVE_TUNER && HAVE_TSPARSER && HAVE_DSMCC
#include "../include/DataWrapperListener.h"

#if HAVE_COMPSUPPORT
#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;
#else
#include "../../gingacc-system/include/io/GingaLocatorFactory.h"
#endif

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace lssm {
#if HAVE_COMPSUPPORT
	static IComponentManager* cm = IComponentManager::getCMInstance();
#endif
	DataWrapperListener::DataWrapperListener(
			IPresentationEngineManager* pem) {

		this->pem         = pem;
		this->documentUri = "";
		this->autoMount   = false;
	}

	DataWrapperListener::~DataWrapperListener() {
		this->pem = NULL;
	}

	void DataWrapperListener::autoMountOC(bool autoMountIt) {
		this->autoMount = autoMountIt;
	}

	void DataWrapperListener::objectMounted(
			string ior, string clientUri, string name) {

		IGingaLocatorFactory* glf = NULL;

#if HAVE_COMPSUPPORT
		glf = ((GingaLocatorFactoryCreator*)(cm->getObject(
				"GingaLocatorFactory")))();
#else
		glf = GingaLocatorFactory::getInstance();
#endif

		cout << "DataWrapperListener::objectMounted ior '" << ior;
		cout << "', uri '" << clientUri << "' and name '" << name;
		cout << "'" << endl;
		if (glf != NULL) {
			glf->addClientLocation(ior, clientUri, name);
		}

		if (name.find(".ncl") != std::string::npos && documentUri == "") {
			documentUri = updatePath(clientUri + "/" + name);
		}
	}

	void DataWrapperListener::receiveStreamEvent(IStreamEvent* event) {
		string eventType;
		string eventData;

		eventType = event->getEventName();
		eventData.assign(event->getData(), event->getDescriptorLength());
/*
		cout << "DsmccWrapper::receiveStreamEvent ";
		cout << "eventId = '" << (eventData[0] & 0xFF) << (eventData[1] & 0xFF);
		cout << "' commandTag = '" << (eventData[11] & 0xFF);
		cout << "' eventType = '" << eventType;
		cout << "' and payload = '" << eventData;
		cout << "'" << endl;
*/
		if (eventType == "gingaEditingCommands") {
			if (pem != NULL) {
				pem->editingCommand(eventData);
			}

		} else {
			//TODO: fix eventName stuffs
			if (pem != NULL) {
				pem->editingCommand(eventData);
			}
		}
	}

	void DataWrapperListener::serviceDomainMounted(
			map<string, string>* names, map<string, string>* paths) {

		map<string, string>::iterator i;
		string nclName;

		if (!autoMount) {
			cout << "DataWrapperListener::serviceDomainMounted waiting ";
			cout << "ncl editing commands";
			cout << endl;
			return;
		}

		i = names->begin();
		while (i != names->end()) {
			if (i->second.find("main.ncl") != std::string::npos) {
				nclName = i->second;
				i = paths->find(i->first);
				if (i != paths->end()) {
					pem->startPresentation(i->second + nclName);
					return;
				}
			}
			++i;
		}
	}

	void DataWrapperListener::receiveEventInfo(set<IEventInfo*>* events) {
		map<string, string> t;
		IEventInfo* ei;
		set<IEventInfo*>::iterator i;
		set<IMpegDescriptor*>* descs;
		IShortEventDescriptor* sed;

		cout << "PresentationEngineManager::receiveEventInfo ";
		i = events->begin();
		while (i != events->end()) {
			ei = *i;

			t["class"] = "ncl";
			t["type"] = "epgFactory";
			t["call"] = "addEvent";

			t["id"] = itos(ei->getEventId());
			t["startTime"] = ei->getFormattedStartTime();
			t["duration"] = ei->getFormattedDuration();
			t["isRunning"] = itos(ei->getRunningStatus());
			cout << t["id"] << ", " << t["startTime"] << ", ";
			cout << t["duration"] << ", " << t["isRunning"] << ", ";
			descs = ei->getDescriptors();
			//TODO: handle descriptors
			if (!descs->empty() &&
					(*(descs->begin()))->getDescriptorTag() == 0x4D) {

				sed = (IShortEventDescriptor*)(*(descs->begin()));
				t["language"] = ((string)(sed->getLanguageCode())).substr(
						0, 3);

				t["name"] = (string)(sed->getEventName());
				t["description"] = (string)(sed->getDescription());

				cout << t["language"] << ", " << t["name"] << ", ";
				cout << t["description"] << "." ;
			}

			//pem->pushEPGEventToEPGFactory(t);
			t.clear();
			++i;
		}

		t["class"] = "ncl";
		t["type"] = "epgFactory";
		t["call"] = "startDocument";
		//pem->pushEPGEventToEPGFactory(t);
		cout << endl;
	}
}
}
}
}
}
#endif //HAVE_TUNER && HAVE_TSPARSER && HAVE_DSMCC

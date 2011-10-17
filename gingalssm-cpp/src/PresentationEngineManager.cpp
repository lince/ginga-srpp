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

#if HAVE_COMPSUPPORT
#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;
#else
#include "../../gingacc-player/include/ProgramAV.h"
#include "../../gingacc-system/include/io/GingaLocatorFactory.h"
#include "../../gingacc-system/include/io/LocalDeviceManager.h"
#include "../../gingacc-system/include/io/InputManager.h"

#include "../../gingancl/include/Formatter.h"
using namespace ::br::pucrio::telemidia::ginga::ncl;

#include "../../gingacc-player/include/ShowButton.h"
#endif

#include "player/IShowButton.h"
#include "player/IApplicationPlayer.h"
using namespace ::br::pucrio::telemidia::ginga::core::player;

#if HAVE_DSMCC && HAVE_TSPARSER && HAVE_TUNER
#include "dataprocessing/ncl/edit/EventDescriptor.h"
using namespace ::br::pucrio::telemidia::ginga::core::dataprocessing::ncl;

#include "../include/DataWrapperListener.h"
#endif

#include "system/io/interface/input/CodeMap.h"
#include "system/io/IGingaLocatorFactory.h"
#include "system/io/IInputManager.h"
#include "system/io/ILocalDeviceManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include "../include/PresentationEngineManager.h"
using namespace ::br::pucrio::telemidia::ginga::lssm;

struct inputEventNotification {
	PresentationEngineManager* p;
	int code;
	string parameter;
#if HAVE_TUNER
	ITuner* tuner;
#endif
	vector<string>* cmds;
};

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace lssm {
#if HAVE_COMPSUPPORT
	static IComponentManager* cm = IComponentManager::getCMInstance();
#endif

#if HAVE_COMPSUPPORT
	static IShowButton* sb = ((WidgetCreator*)(cm->getObject("ShowButton")))();
#else
	static IShowButton* sb = ShowButton::getInstance();
#endif

	ILocalDeviceManager* PresentationEngineManager::dm = NULL;
	IInputManager* PresentationEngineManager::im       = NULL;

	PresentationEngineManager::PresentationEngineManager(
			int devClass,
			int xOffset,
			int yOffset,
			int width,
			int height,
			bool enableGfx) : Thread::Thread() {
			
#if HAVE_RECOMMENDER
		Utilitario::getInstance()->setTimeStart(0);
#endif

		int deviceNumber  = 0;
		string deviceName = "systemScreen(" + itos(devClass) + ")";

		if (dm == NULL) {
#if HAVE_COMPSUPPORT
			dm = ((LocalDeviceManagerCreator*)(
					cm->getObject("LocalDeviceManager")))();
#else
			dm = LocalDeviceManager::getInstance();
#endif
		}

		x = 0;
		if (xOffset > 0) {
			x = xOffset;
		}

		y = 0;
		if (yOffset > 0) {
			y = yOffset;
		}

		deviceNumber = dm->createDevice(deviceName);

		w = dm->getDeviceWidth(deviceNumber);
		if (width > 0 && (width < w || w == 0)) {
			w = width;
		}

		h = dm->getDeviceHeight(deviceNumber);
		if (height > 0 && (height < h || h == 0)) {
			h = height;
		}

		if (h == 0 || w == 0) {
			cout << "PEM::PEM Warning! Trying to create an ";
			cout << "invalid window" << endl;
			w = 1;
			h = 1;
		}

		this->devClass             = devClass;
		this->enableGfx            = enableGfx;
		this->formattersToRelease  = new set<INCLPlayer*>;
		this->formatters           = new map<string, INCLPlayer*>;
		this->dsmccListener        = NULL;
		this->tuner                = NULL;

#ifdef DataWrapperListener_H_
		this->dsmccListener        = new DataWrapperListener(this);
#endif

		this->paused               = false;
		this->standAloneApp        = true;
		this->isLocalNcl           = true;
		this->closed               = false;
		this->currentPrivateBaseId = -1;
		this->commands             = NULL;
		this->timeBaseProvider     = NULL;

#if HAVE_COMPSUPPORT
		ipav = ((ProgramHandlerCreator*)(cm->getObject("ProgramAV")))();
		im   = ((InputManagerCreator*)(cm->getObject("InputManager")))();
		privateBaseManager = ((PrivateBaseManagerCreator*)(cm->getObject(
				"PrivateBaseManager")))();

#else
		ipav = ProgramAV::getInstance();
		im   = InputManager::getInstance();
		privateBaseManager = PrivateBaseManager::getInstance();
#endif

		im->addInputEventListener(this);

		ProgramRecorder::getInstance()->setPresentation(this);

	}

	PresentationEngineManager::~PresentationEngineManager() {
		if (!closed) {
			close();
		}
	}

	void PresentationEngineManager::autoMountOC(bool autoMountIt) {
#ifdef DataWrapperListener_H_
		((DataWrapperListener*)dsmccListener)->autoMountOC(autoMountIt);
#endif
	}

	void PresentationEngineManager::setCurrentPrivateBaseId(
			unsigned int baseId) {

		cout << "PresentationEngineManager::setCurrentPrivateBaseId '";
		cout << baseId << "'";
		cout << endl;
		this->currentPrivateBaseId = (int)baseId;
	}

	void PresentationEngineManager::setTimeBaseProvider(
			ITimeBaseProvider* tmp) {

		timeBaseProvider = tmp;
	}

	void PresentationEngineManager::setTimeBaseInfo(INCLPlayer* p) {
		if (timeBaseProvider != NULL) {
			p->setTimeBaseProvider(timeBaseProvider);
		}
	}

	void PresentationEngineManager::editingCommand(string editingCommand) {
		vector<string>* args;
		vector<string>::iterator i;
		IGingaLocatorFactory* glf = NULL;
		//NclDocument* document     = NULL;
		INCLPlayer* docPlayer     = NULL;
		string commandTag, privateDataPayload, baseId, docId;
		string docIor, docUri, arg, uri, ior, uName;

#if HAVE_COMPSUPPORT
		glf = ((GingaLocatorFactoryCreator*)(cm->getObject(
				"GingaLocatorFactory")))();
#else
		glf = GingaLocatorFactory::getInstance();
#endif

#if HAVE_DSMCC
		commandTag         = EventDescriptor::getCommandTag(editingCommand);
		privateDataPayload = EventDescriptor::getPrivateDataPayload(
				editingCommand);

		args   = split(privateDataPayload, ",");
		i      = args->begin();
		baseId = EventDescriptor::extractMarks(*i);
		++i;

		//parse command
		if (commandTag == EC_OPEN_BASE) {


		} else if (commandTag == EC_ACTIVATE_BASE) {


		} else if (commandTag == EC_DEACTIVATE_BASE) {


		} else if (commandTag == EC_SAVE_BASE) {


		} else if (commandTag == EC_CLOSE_BASE) {


		} else if (commandTag == EC_ADD_DOCUMENT) {
			cout << "PresentationEngineManager::editingCommand (addDocument)";
			cout << endl;

			docUri = "";
			docIor = "";
			while (i != args->end()) {
				if ((*i).find("x-sbtvd://") != std::string::npos) {
					uri = EventDescriptor::extractMarks(*i);
					++i;
					ior = EventDescriptor::extractMarks(*i);

					if (uri.find("x-sbtvd://") != std::string::npos) {
						uri = uri.substr(uri.find("x-sbtvd://") + 10,
								uri.length() - (uri.find("x-sbtvd://") + 10));
					}

					if (docUri == "") {
						docUri = uri;
						docIor = ior;
					}

					cout << "PresentationEngineManager::editingCommand ";
					cout << " command '" << arg << "' creating locator ";
					cout << "uri '" << uri << "', ior '" << ior;
					cout << "'" << endl;
					if (glf != NULL) {
						glf->createLocator(uri, ior);
					}

				} else {
					uri = *i;
					ior = uri;
					if (docUri == "") {
						docIor = uri;
						docUri = uri;
					}
				}

				++i;
			}

			if (docUri == docIor) {
				cout << "PresentationEngineManager::editingCommand";
				cout << " calling addDocument '" << docUri;
				cout << "' in private base '" << baseId;
				cout << endl;

				lock();
				docPlayer = createNclPlayer(baseId, docUri);
				unlock();

			} else {
				cout << "PresentationEngineManager::editingCommand calling ";
				cout << "getLocation '" << docUri << "' for ior '";
				cout << docIor << "'" << endl;

				if (glf != NULL) {
					uri = glf->getLocation(docUri);
					uName = glf->getName(docIor);
					lock();
					docPlayer = createNclPlayer(baseId, uri + uName);
					unlock();
				}
			}

		} else {
			cout << "PresentationEngineManager::editingCommand not to base";
			cout << endl;

			docId  = EventDescriptor::extractMarks(*i);
			docPlayer = getNclPlayer(baseId, docId);
			if (docPlayer != NULL) {
				if (commandTag == EC_START_DOCUMENT) {
					setTimeBaseInfo(docPlayer);
				}
				docPlayer->editingCommand(commandTag, privateDataPayload);

			} else {
				cout << "PresentationEngineManager::editingCommand can't ";
				cout << "find NCL player for '" << docId << "'";
				cout << endl;
			}
		}

#endif //HAVE_DSMCC
	}

	void PresentationEngineManager::setBackgroundImage(string uri) {
		updateFormatters(UC_BACKGROUND, uri);
	}

	void PresentationEngineManager::getScreenShot() {
		updateFormatters(UC_PRINTSCREEN);
	}

	void PresentationEngineManager::close() {
		map<int, set<INCLPlayer*>*>::iterator i;

		closed = true;

		if (im != NULL) {
			im->removeInputEventListener(this);
			im->release();
			im = NULL;
		}

		if (sb != NULL) {
			delete sb;
			sb = NULL;
		}

		lock();
		if (formattersToRelease != NULL) {
			delete formattersToRelease;
			formattersToRelease = NULL;
		}

		if (formatters != NULL) {
			delete formatters;
			formatters = NULL;
		}
		unlock();

		if (dm != NULL) {
			//dm->clearWidgetPools();
			dm->release();
		}
	}

	void PresentationEngineManager::registerKeys() {
		set<int>* keys;
		keys = new set<int>;

		keys->insert(CodeMap::KEY_GREATER_THAN_SIGN);
		keys->insert(CodeMap::KEY_LESS_THAN_SIGN);

		keys->insert(CodeMap::KEY_SUPER);
		keys->insert(CodeMap::KEY_PRINTSCREEN);

		keys->insert(CodeMap::KEY_F10);
		keys->insert(CodeMap::KEY_POWER);

		keys->insert(CodeMap::KEY_F11);
		keys->insert(CodeMap::KEY_STOP);

		keys->insert(CodeMap::KEY_F12);
		keys->insert(CodeMap::KEY_PAUSE);

#if HAVE_TUNER
		this->tuner = NULL;
		keys->insert(CodeMap::KEY_PAGE_UP);
		keys->insert(CodeMap::KEY_PAGE_DOWN);
		keys->insert(CodeMap::KEY_CHANNEL_UP);
		keys->insert(CodeMap::KEY_CHANNEL_DOWN);
#endif

		if (commands != NULL && !commands->empty()) {
			keys->insert(CodeMap::KEY_PLUS_SIGN);
		}

		if (im != NULL) {
			im->addInputEventListener(this, keys);
		}
	}

	bool PresentationEngineManager::getIsLocalNcl() {
		return this->isLocalNcl;
	}

	void PresentationEngineManager::setIsLocalNcl(bool isLocal, void* tuner) {
		if (this->tuner != NULL && this->tuner != tuner) {
#if HAVE_TUNER
			delete (ITuner*)(this->tuner);
#endif
		}

		this->tuner      = tuner;
		this->isLocalNcl = isLocal;
	}

	INCLPlayer* PresentationEngineManager::createNclPlayer(
			string baseId, string fname) {

		NclPlayerData* data   = NULL;
		NclDocument* document = NULL;
		INCLPlayer* formatter = NULL;

		if (formatters->find(fname) != formatters->end()) {
			formatter = (*formatters)[fname];

		} else {
			data           = createNclPlayerData();
			data->baseId   = baseId;
			data->playerId = fname;

#if HAVE_COMPSUPPORT
			formatter = ((NCLPlayerCreator*)(cm->getObject("Formatter")))(data);

#else
			formatter = new Formatter(data);
#endif
			document = (NclDocument*)(formatter->setCurrentDocument(fname));

			if (formatters->empty()) {
				registerKeys();
			}

			formatter->addListener(this);
			(*formatters)[fname] = formatter;
		}

		return formatter;
	}

	NclPlayerData* PresentationEngineManager::createNclPlayerData() {
		NclPlayerData* data     = NULL;

		data = new NclPlayerData;
		data->baseId            = "";
		data->playerId          = "";
		data->devClass          = devClass;
		data->x                 = x;
		data->y                 = y;
		data->w                 = w;
		data->h                 = h;
		data->enableGfx         = enableGfx;
		data->parentDocId       = "";
		data->nodeId            = "";
		data->docId             = "";
		data->focusManager      = NULL;

		return data;
	}

	bool PresentationEngineManager::openNclFile(string fname) {
		INCLPlayer* formatter;
		double time = getCurrentTimeMillis();

		lock();
		if (formatters->find(fname) != formatters->end()) {
			unlock();
			return false;
		}

		time = getCurrentTimeMillis() - time;
		cout << "PresentationEngineManager::openNclFile";
		cout << " document process time: " << time;
		cout << endl;

		formatter = createNclPlayer(itos(currentPrivateBaseId), fname);
		unlock();

		return (formatter != NULL);
	}

	//#if HAVE_GINGAJ
	void PresentationEngineManager::openNclDocument(
			string docUri, int x, int y, int w, int h) {

		cout << "PresentationEngineManager::openNclDocument docUri '";
		cout << docUri << "' x = '" << x << "', y = '" << y << "', w = '";
		cout << w << "', h = '" << h << "'" << endl;
	}

	//#if HAVE_DSMCC
	void* PresentationEngineManager::getDsmccListener() {
		return this->dsmccListener;
	}

	void PresentationEngineManager::pausePressed() {
		if (paused) {
			updateFormatters(UC_RESUME);

		} else {
			updateFormatters(UC_PAUSE);
		}
	}

	void PresentationEngineManager::startPresentation(string fname) {
		INCLPlayer* formatter;

		formatter = getNclPlayer(fname);
		if (formatter == NULL) {
			openNclFile(fname);
			formatter = getNclPlayer(fname);
			if (formatter == NULL) {
				return;
			}
		}

		setTimeBaseInfo(formatter);
		formatter->play();
	}

	void PresentationEngineManager::stopPresentation() {
		cout << "PresentationEngineManager::stopPresentation" << endl;

		updateFormatters(UC_STOP);
		checkStatus();
	}

	void PresentationEngineManager::setCmdFile(string cmdFile) {
		ifstream fis;
		string cmd;

		fis.open(cmdFile.c_str(), ifstream::in);
		if (!fis.is_open()) {
			cout << "PresentationEngineManager";
			cout << "::setCmdFile Warning! can't open '" << cmdFile;
			cout << "'" << endl;
			return;
		}

		if (commands != NULL) {
			delete commands;
			commands = NULL;
		}

		commands = new vector<string>;

		while (fis.good()) {
			fis >> cmd;
			if (cmd != "" && cmd.substr(0, 1) != "#") {
				commands->push_back(cmd);
			}
		}
	}

	void PresentationEngineManager::waitUnlockCondition() {
		Thread::waitForUnlockCondition();
	}

	void PresentationEngineManager::presentationCompleted(string formatterId) {
		cout << "PresentationEngineManager";
		cout << "::presentationCompleted for '" << formatterId;
		cout << "'" << endl;

		releaseFormatter(formatterId);
	}

	void PresentationEngineManager::releaseFormatter(string formatterId) {
		map<string, INCLPlayer*>::iterator i;
		INCLPlayer* formatter;

		lock();
		if (formatterId == "") {
			i = formatters->begin();

		} else {
			i = formatters->find(formatterId);
		}

		if (i != formatters->end()) {
			formatter = i->second;
			formatters->erase(i);
			formattersToRelease->insert(formatter);
			unlock();
			Thread::start();

		} else {
			unlock();
		}

		checkStatus();
	}

	bool PresentationEngineManager::checkStatus() {
		bool checked = false;

		lock();
		if (formatters->empty() && isLocalNcl) {
			sb->stop();
			::usleep(900000);

			checked = true;
		}
		unlock();

		if (checked) {
			ipav->release();
			close();
			unlockConditionSatisfied();
		}

		return checked;
	}

	void PresentationEngineManager::updateStatus(
			short code, string parameter, short type) {

		struct inputEventNotification* ev;

		switch (code) {
			case IPlayer::PL_NOTIFY_STOP:
				if (type == IPlayer::TYPE_PRESENTATION) {
					ev = new struct inputEventNotification;

					ev->p         = this;
					ev->parameter = parameter;
					ev->code      = code;

#ifdef DSMCCWRAPPERLISTENER_H_
					ev->tuner = NULL;
#endif

					pthread_t notifyThreadId_;
					pthread_create(
							&notifyThreadId_,
							0,
							PresentationEngineManager::eventReceived,
							(void*)ev);

					pthread_detach(notifyThreadId_);
				}
				break;

			default:
				break;
		}
	}

	bool PresentationEngineManager::userEventReceived(IInputEvent* ev) {
		struct inputEventNotification* evR;

		evR            = new struct inputEventNotification;
		evR->p         = this;
		evR->parameter = "";
		evR->code      = ev->getKeyCode();
#ifdef DSMCCWRAPPERLISTENER_H_
		evR->tuner     = (ITuner*)this->tuner;
#endif
		evR->cmds      = commands;

		pthread_t notifyThreadId_;
		pthread_create(
				&notifyThreadId_,
				0, PresentationEngineManager::eventReceived, (void*)evR);

		pthread_detach(notifyThreadId_);
		
		return true;
	}

	void* PresentationEngineManager::eventReceived(void* ptr) {
		struct inputEventNotification* ev;
		PresentationEngineManager* p;
		string parameter;
		vector<string>* cmds;

		ev             = (struct inputEventNotification*)ptr;

		const int code = ev->code;
		p              = ev->p;
		parameter      = ev->parameter;
		cmds           = ev->cmds;

#ifdef DSMCCWRAPPERLISTENER_H_
		ITuner* t;
		t = ev->tuner;
#endif

		if (parameter != "" && code == IPlayer::PL_NOTIFY_STOP) {
			::usleep(100000);
			p->presentationCompleted(parameter);

		} else if (code == CodeMap::KEY_POWER || code == CodeMap::KEY_F10) {
			sb->stop();
			::usleep(900000);
			printTimeStamp();
			cout << "PresentationEngineManager::eventReceived power off!";
			cout << endl;
			p->setIsLocalNcl(true, NULL);
			p->stopPresentation();

		} else if (code == CodeMap::KEY_STOP || code == CodeMap::KEY_F11) {
			sb->stop();
			::usleep(900000);
			printTimeStamp();
			cout << "PresentationEngineManager::eventReceived stop apps!";
			cout << endl;
			p->stopPresentation();

		} else if (code == CodeMap::KEY_PRINTSCREEN ||
				code == CodeMap::KEY_SUPER) {

			p->getScreenShot();

		} else if (code == CodeMap::KEY_PAUSE || code == CodeMap::KEY_F12) {

			sb->pause();
			p->pausePressed();

#ifdef DSMCCWRAPPERLISTENER_H_
		} else if (t != NULL) {
			if (code == CodeMap::KEY_CHANNEL_UP ||
					code == CodeMap::KEY_PAGE_UP) {

				t->channelUp();

			} else if (code == CodeMap::KEY_CHANNEL_DOWN ||
					code == CodeMap::KEY_PAGE_DOWN) {

				t->channelDown();
			}
#endif

		} else if (code == CodeMap::KEY_PLUS_SIGN && cmds != NULL) {
			if (!cmds->empty()) {
				string cmd = *(cmds->begin());
				cout << "RUNNING CURRENT COMMAND '" << cmd;
				cout << "'" << endl;

				p->readCommand(cmd);
				cmds->erase(cmds->begin());
				cmds->push_back(cmd);
			}

		} else if (code == CodeMap::KEY_GREATER_THAN_SIGN) {
			cout << ">> TIME SHIFT >>" << endl;
			p->updateFormatters(UC_SHIFT, "forward");

		} else if (code == CodeMap::KEY_LESS_THAN_SIGN) {
			cout << "<< TIME SHIFT <<" << endl;
			p->updateFormatters(UC_SHIFT, "backward");
		}

		delete ev;
		return NULL;
	}

	void PresentationEngineManager::readCommand(string command) {
		string cmdTag = "", cmdParams = "", editingCmd = "", cmdHeader = "";

		if (command.find("addDocument") != std::string::npos) {
			cmdTag = "5";

		} else if (command.find("startDocument") != std::string::npos) {
			cmdTag = "7";
		}

		if (cmdTag != "" && command.find("(") != std::string::npos) {
			cmdParams = command.substr(
					command.find_first_of("(") + 1,
					command.length() - (command.find_first_of("(") + 1));

			if (cmdParams.find(")") != std::string::npos) {
				cmdParams     = cmdParams.substr(
						0, cmdParams.find_last_of(")"));

				editingCmd    = cmdTag + "1" + cmdParams;
				cmdHeader     = "02000000000";
				cmdHeader[10] = editingCmd.length();
				editingCommand(cmdHeader + editingCmd);
			}
		}
	}

/*
	void PresentationEngineManager::addRegion(string location) {
		if (location != "" && formatter != NULL) {
			//TODO choose the parent region
			formatter->addRegion(currentDocument->getId(), NULL, location);
		}
	}

	void PresentationEngineManager::removeRegion(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeRegion(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addRegionBase(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addRegionBase(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeRegionBase(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeRegionBase(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addRule(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addRule(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeRule(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeRule(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addRuleBase(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addRuleBase(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeRuleBase(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeRuleBase(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addConnector(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addConnector(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeConnector(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeConnector(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addConnectorBase(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addConnectorBase(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeConnectorBase(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeConnectorBase(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addTransition(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addTransition(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeTransition(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeTransition(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addTransitionBase(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addTransitionBase(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeTransitionBase(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeTransitionBase(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addDescriptor(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addDescriptor(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeDescriptor(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeDescriptor(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addDescriptorBase(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addDescriptorBase(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeDescriptorBase(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeDescriptorBase(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addImportBase(string id, string location) {
		if (location != "" && id != "" && formatter != NULL) {
			formatter->addImportBase(currentDocument->getId(), id, location);
		}
	}

	void PresentationEngineManager::removeImportBase(
		    string id, string location) {

		if (id != "" && location != "" && formatter != NULL) {
			formatter->removeImportBase(currentDocument->getId(), id, location);
		}
	}

	void PresentationEngineManager::addImportedDocumentBase(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addImportedDocumentBase(
				    currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeImportedDocumentBase(string id) {
		if (id != "" && formatter != NULL) {
			formatter->removeImportedDocumentBase(currentDocument->getId(), id);
		}
	}

	void PresentationEngineManager::addImportNCL(string location) {
		if (location != "" && formatter != NULL) {
			formatter->addImportNCL(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::removeImportNCL(string location) {
		if (location != "" && formatter != NULL) {
			formatter->removeImportNCL(currentDocument->getId(), location);
		}
	}

	void PresentationEngineManager::addNode(string id, string location) {
		if (location != "" && id != "" && formatter != NULL) {
			formatter->addNode(currentDocument->getId(), id, location);
		}
	}

	void PresentationEngineManager::removeNode(
		    string compositionId, string nodeId) {

		if (compositionId != "" && nodeId != "" && formatter != NULL) {
    		formatter->removeNode(
    			    currentDocument->getId(), compositionId, nodeId);
  		}
  	}

	void PresentationEngineManager::addInterface(string id, string location) {
		if (location != "" && id != "" && formatter != NULL) {
			formatter->addInterface(currentDocument->getId(), id, location);
		}
	}

	void PresentationEngineManager::removeInterface(
		    string nodeId, string interfaceId) {

		if (nodeId != "" && interfaceId != "" && formatter != NULL) {
			formatter->removeInterface(
				    currentDocument->getId(), nodeId, interfaceId);
		}
	}

	void PresentationEngineManager::addLink(string id, string location) {
		if (location != "" && id != "" && formatter != NULL) {
			formatter->addLink(currentDocument->getId(), id, location);
		}
	}

	void PresentationEngineManager::removeLink(
		    string compositionId, string linkId) {

		if (compositionId != "" && linkId != "" && formatter != NULL) {
			formatter->removeLink(
				    currentDocument->getId(), compositionId, linkId);
		}
	}
*/

	INCLPlayer* PresentationEngineManager::getNclPlayer(string docLocation) {
		map<string, INCLPlayer*>::iterator i;
		INCLPlayer* nclPlayer;

		lock();
		i = formatters->find(docLocation);
		if (i != formatters->end()) {
			nclPlayer = i->second;
			unlock();

			return nclPlayer;
		}

		unlock();
		return NULL;
	}

	INCLPlayer* PresentationEngineManager::getNclPlayer(
			string baseId, string docId) {

		string docLocation;

		docLocation = privateBaseManager->getDocumentLocation(baseId, docId);
		return getNclPlayer(docLocation);
	}

	void PresentationEngineManager::updateFormatters(
			short command, string parameter) {

		map<string, INCLPlayer*>::iterator i;
		INCLPlayer* formatter;

		lock();
		i = formatters->begin();
		while (i != formatters->end()) {
			formatter = i->second;
			switch (command) {
				case UC_STOP:
					formatter->removeListener(this);
					formatter->stop();
					formattersToRelease->insert(formatter);
					break;

				case UC_PRINTSCREEN:
					cout << "PRINTSCREEN" << endl;
					formatter->getScreenShot();
					unlock();
					return;

				case UC_BACKGROUND:
					formatter->setBackgroundImage(parameter);
					break;

				case UC_PAUSE:
					paused = true;
					formatter->pause();
					break;

				case UC_RESUME:
					paused = false;
					formatter->resume();
					break;

				case UC_SHIFT:
					cout << "PresentationEngineManager::updateFormatters";
					cout << " shifting time" << endl;
					formatter->timeShift(parameter);
					unlock();
					return;

				default:
					break;
			}
			++i;
		}

		if (command == UC_STOP) {
			cout << "PresentationEngineManager::updateFormatters";
			cout << " UC_STOP" << endl;
			formatters->clear();
			Thread::start();
		}

		unlock();
	}

	void PresentationEngineManager::run() {
		set<INCLPlayer*>::iterator i;

		lock();
		if (formattersToRelease != NULL && !formattersToRelease->empty()) {
			i = formattersToRelease->begin();
			while (i != formattersToRelease->end()) {
				delete *i;
				++i;
			}
			formattersToRelease->clear();
		}
		unlock();
	}
}
}
}
}
}

using namespace ::br::pucrio::telemidia::ginga::lssm;
extern "C" IPresentationEngineManager* createPEM(
		int devClass, int xOffset, int yOffset, int w, int h, bool enableGfx) {

	return new PresentationEngineManager(
			devClass, xOffset, yOffset, w, h, enableGfx);
}

extern "C" void destroyPEM(IPresentationEngineManager* pem) {
	delete pem;
}

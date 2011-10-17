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

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

#include "../config.h"
#if HAVE_COMPSUPPORT
#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;
#else
#include "../../ncl30/include/layout/DeviceLayout.h"
#include "../../gingacc-system/include/io/LocalDeviceManager.h"
#include "../../gingacc-multidevice/include/services/DeviceDomain.h"
#include "../../gingancl/include/multidevice/FormatterMultiDevice.h"
#include "../../gingancl/include/multidevice/FormatterPassiveDevice.h"
#include "../../gingancl/include/multidevice/FormatterActiveDevice.h"
#include "../../gingalssm/include/CommonCoreManager.h"
#include "../../gingalssm/include/PresentationEngineManager.h"
#endif

#include "ncl/layout/IDeviceLayout.h"
using namespace ::br::pucrio::telemidia::ncl::layout;

#include "system/io/ILocalDeviceManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include "multidevice/services/IDeviceDomain.h"
using namespace ::br::pucrio::telemidia::ginga::core::multidevice;

#include "gingancl/multidevice/IFormatterMultiDevice.h"
using namespace ::br::pucrio::telemidia::ginga::ncl::multidevice;

#include "gingalssm/ICommonCoreManager.h"
#include "gingalssm/IPresentationEngineManager.h"
using namespace ::br::pucrio::telemidia::ginga::lssm;

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

#include <cgc/ProgramRecorder.h>
using namespace br::ufscar::lince::ginga::core::cgc;

#include <recommender/exportxml/createXML.h>

#include "../config.h"

/*#if HAVE_RECOMMENDER
#include "recommender/Scheduler.h"
using namespace br::ufscar::lince::ginga::recommender;
#endif*/

#include <string>
#include <iostream>
using namespace std;

void printHelp() {
	cout << endl << "Usage: gingaNcl [OPTIONS]... [<--ncl> NCLFILE]" << endl;
	cout << "Example: gingaNcl --ncl test.ncl" << endl;
	cout << endl << "OPTIONS are:" << endl;
	cout << "-h, --help                    Display this information." << endl;
	cout << "-i, --insert-delay <value>    Insert a delay before application ";
	cout << "processing." << endl;
	cout << "-i, --insert-oc-delay <value> Insert a delay before tune main A/V ";
	cout << "(to exclusively process OC elementary streams)." << endl;
	cout << "-v, --version                 Display version." << endl;
	cout << "    --enable-log [mode]       Enable log mode:" << endl;
	cout << "                                Turn on verbosity, gather all";
	cout << " output" << endl;
	cout << "                                messages and write them into a";
	cout << " device" << endl;
	cout << "                                according to mode." << endl;
	cout << "                                  [mode] can be:" << endl;
	cout << "                                     'file'   write messages into";
	cout << " a file" << endl;
	cout << "                                              named logFile.txt.";
	cout << endl;
	cout << "                                     'stdout' write messages into";
	cout << " /dev/stdout" << endl;
	cout << "                                     'null'   write messages into";
	cout << " /dev/null" << endl;
	cout << "                                              (default mode).";
	cout << endl;
	cout << "    --x-offset <value>        Offset of left display coord." << endl;
	cout << "    --y-offset <value>        Offset of top display coord." << endl;
	cout << "    --set-width <value>       Force a width display value." << endl;
	cout << "    --set-height <value>      Force a height display value.";
	cout << endl;
	cout << "    --device-class <value>    Define the device class through the";
	cout << endl;
	cout << "                                    presentation" << endl;
	cout << "    --disable-gfx             Disable gfx loading presented during";
	cout << " application processing." << endl;
	cout << "    --enable-automount        Enable trigger for main.ncl ";
	cout << " applications received via a transport protocol." << endl;
	cout << "    --enable-remove-oc-filter Enable processor economies after";
	cout << " first time that the object carousel is mounted." << endl;
}

void printVersion() {
	cout << endl;
	cout << "This is ginga (open source version):  a free " << endl;
	cout << " support for iTV middleware named Ginga-NCL." << endl;
	cout << "Ginga binary v0.11.2." << endl;
	cout << "(c) 2002-2010 The TeleMidia PUC-Rio Team." << endl << endl;
}

string updateFileUri(string file) {
	if (!isAbsolutePath(file)) {
		return getCurrentPath() + file;
	}

	return file;
}

int main(int argc, char *argv[]) {
	//HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince");
	//LoggerUtil::configure();

	ICommonCoreManager* ccm = NULL;
	IPresentationEngineManager* pem = NULL;
	ILocalDeviceManager* dm = NULL;
	IFormatterMultiDevice* fmd = NULL;
#if HAVE_COMPSUPPORT
	IComponentManager* cm;
#endif

	string nclFile       = "", param = "", bgUri = "", cmdFile = "";
	int i, devClass      = 0;
	int xOffset          = 0, yOffset = 0, w = 0, h = 0, maxTransp = 0;
	double delayTime     = 0;
	double ocDelay       = 0;
	bool isRemoteDoc     = false;
	bool removeOCFilter  = false;
	bool forceQuit       = true;
	bool enableGfx       = true;
	bool autoMount       = false;
	bool enableCac       = false;
	bool enableCgc       = false;
	bool enableXml		 = false;
	string name;

	setLogToNullDev();

	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
			printHelp();
			return 0;

		} else if ((strcmp(argv[i], "-v") == 0) ||
			    (strcmp(argv[i], "--version")) == 0) {

			printVersion();
			return 0;

		} else if ((strcmp(argv[i], "--ncl") == 0) && ((i + 1) < argc)) {
			nclFile.assign(argv[i+1], strlen(argv[i+1]));
			cout << "argv = '" << argv[i+1] << "' nclFile = '";
			cout << nclFile << "'" << endl;

		} else if ((strcmp(argv[i], "--cac") == 0) && ((i + 1) < argc)) {
			nclFile.assign(argv[i+1], strlen(argv[i+1]));
			cout << "argv = '" << argv[i+1] << "' nclFile = '";
			cout << nclFile << "'" << endl;
			enableCac = true;

		} else if (strcmp(argv[i], "--cgc") == 0) {
			enableCgc = true;

		} else if (strcmp(argv[i], "--xml") == 0) {
			name = argv[i+1];
			enableXml = true;

		} else if ((strcmp(argv[i], "--enable-log") == 0) && ((i + 1) < argc)) {
			if (strcmp(argv[i+1], "file") == 0) {
				setLogToFile();

			} else if (strcmp(argv[i+1], "stdout") == 0) {
				setLogToStdoutDev();
			}

		} else if ((strcmp(argv[i], "--force-quit") == 0) && ((i + 1) < argc)) {
			if (strcmp(argv[i+1], "false") == 0) {
				forceQuit = false;
			}

		} else if ((strcmp(argv[i], "--x-offset") == 0) && ((i + 1) < argc)) {
			if (isNumeric(argv[i+1])) {
				param = argv[i+1];
				xOffset = stof(param);
			}

		} else if ((strcmp(argv[i], "--y-offset") == 0) && ((i + 1) < argc)) {
			if (isNumeric(argv[i+1])) {
				param = argv[i+1];
				yOffset = stof(param);
			}

		} else if ((strcmp(argv[i], "--set-width") == 0) && ((i + 1) < argc)) {
			if (isNumeric(argv[i+1])) {
				param = argv[i+1];
				w = stof(param);
			}

		} else if ((strcmp(argv[i], "--set-height") == 0) && ((i + 1) < argc)) {
			if (isNumeric(argv[i+1])) {
				param = argv[i+1];
				h = stof(param);
			}

		} else if ((strcmp(argv[i], "--set-max-transp") == 0) &&
				((i + 1) < argc)) {

			if (isNumeric(argv[i+1])) {
				param = argv[i+1];
				maxTransp = stof(param);
			}

		} else if ((strcmp(argv[i], "--set-bg-image") == 0) &&
				((i + 1) < argc)) {

			param = argv[i+1];
			if (fileExists(param)) {
				bgUri = param;
				cout << "main bgUri = '" << bgUri << "'";
				cout << " OK" << endl;

			} else {
				cout << "main Warning: can't set bgUri '" << param << "'";
				cout << " file does not exist" << endl;
			}

		} else if (((strcmp(argv[i], "--device-class") == 0) ||
				(strcmp(argv[i], "--dev-class") == 0)) && ((i + 1) < argc)) {

			if (strcmp(argv[i+1], "1") == 0 ||
					strcmp(argv[i+1], "passive") == 0) {

				devClass = 1;

			} else if (strcmp(argv[i+1], "2") == 0 ||
					strcmp(argv[i+1], "active") == 0) {

				devClass = 2;
			}

		} else if (((strcmp(argv[i], "-i") == 0) ||
				(strcmp(argv[i], "--insert-delay") == 0)) &&
				((i + 1) < argc)) {

			if (isNumeric(argv[i+1])) {
				param     = argv[i+1];
				delayTime = stof(param);
			}

		} else if (strcmp(argv[i], "--insert-oc-delay") == 0 &&
				((i + 1) < argc)) {

			if (isNumeric(argv[i+1])) {
				param   = argv[i+1];
				ocDelay = stof(param);
			}

		} else if (strcmp(argv[i], "--disable-gfx") == 0) {
			enableGfx = false;

		} else if (strcmp(argv[i], "--enable-automount") == 0) {
			autoMount = true;

		} else if (strcmp(argv[i], "--enable-remove-oc-filter") == 0) {
			removeOCFilter = true;

		} else if ((strcmp(argv[i], "--enable-cmdfile") == 0) &&
				((i + 1) < argc)) {

			cmdFile.assign(argv[i+1], strlen(argv[i+1]));
			cout << "argv = '" << argv[i+1] << "' cmdFile = '";
			cout << cmdFile << "'" << endl;
		}
	}

	if (delayTime > 0) {
		::usleep(delayTime);
	}

	initTimeStamp();
	initializeCurrentPath();
#if HAVE_COMPSUPPORT
	cm = IComponentManager::getCMInstance();
#endif

	if (nclFile != "") {
		nclFile = updateFileUri(nclFile);

		if (argc > 1 && nclFile.substr(0, 1) != "/") {
			cout << "ginga main() remote NCLFILE" << endl;
			isRemoteDoc = true;
		}
	}

	cout << "ginga main() NCLFILE = " << nclFile.c_str() << endl;

	if (devClass == 1) {
#if HAVE_COMPSUPPORT
		dm = ((LocalDeviceManagerCreator*)(cm->getObject(
				"LocalDeviceManager")))();

		dm->createDevice("systemScreen(0)");
		fmd = ((FormatterMultiDeviceCreator*)(cm->getObject(
				"FormatterMultiDevice")))(
						NULL, devClass, xOffset, yOffset, w, h);
#else
		dm  = LocalDeviceManager::getInstance();
		dm->createDevice("systemScreen(0)");
		fmd = new FormatterPassiveDevice(NULL, xOffset, yOffset, w, h);
#endif

		if (bgUri != "") {
			fmd->setBackgroundImage(bgUri);
		}
		getchar();

	} else if (devClass == 2) {
#if HAVE_COMPSUPPORT
		dm = ((LocalDeviceManagerCreator*)(cm->getObject(
				"LocalDeviceManager")))();

		dm->createDevice("systemScreen(0)");
		fmd = ((FormatterMultiDeviceCreator*)(cm->getObject(
				"FormatterMultiDevice")))(
						NULL, devClass, xOffset, yOffset, w, h);
#else
		dm  = LocalDeviceManager::getInstance();
		dm->createDevice("systemScreen(0)");
		fmd = new FormatterActiveDevice(NULL, xOffset, yOffset, w, h);
#endif

		if (bgUri != "") {
			fmd->setBackgroundImage(bgUri);
		}

		getchar();

	} else {

		if (nclFile == "") {
			enableGfx = false;
		}

#if HAVE_COMPSUPPORT
		pem = ((PEMCreator*)(cm->getObject("PresentationEngineManager")))(
				devClass, xOffset, yOffset, w, h, enableGfx);
#else
		pem = new PresentationEngineManager(
				devClass, xOffset, yOffset, w, h, enableGfx);
#endif

		if (pem == NULL) {
			return -2;
		}

		if (bgUri != "") {
			cout << endl << endl;
			cout << "main '" << bgUri << "'" << endl;
			cout << endl << endl;
			pem->setBackgroundImage(bgUri);
		}

		if (cmdFile != "") {
			cmdFile = updateFileUri(cmdFile);
			pem->setCmdFile(cmdFile);
		}

		if (nclFile == "") {
			pem->setIsLocalNcl(false, NULL);
			pem->autoMountOC(autoMount);

#if HAVE_COMPSUPPORT
			ccm = ((CCMCreator*)(cm->getObject("CommonCoreManager")))(pem);

#else
			ccm = new CommonCoreManager(pem, xOffset, yOffset, w, h);
#endif
			ccm->removeOCFilterAfterMount(removeOCFilter);
			if(enableXml) {
				exportXML("/usr/local/etc/ginga/files/recommender/" + name);
			}
			else {
				ccm->setOCDelay(ocDelay);
				ccm->startPresentation();
			}
			if(enableCgc)
			{
				if(ProgramRecorder::getInstance()->getTuner() == NULL)
					cout << "tá nulo" << endl;
				else
					cout << "nao tá nulo" << endl;
			}

		} else if (!enableCac && (fileExists(nclFile) || isRemoteDoc)) {
			cout << "WITHOUT CAC == ginga main() NCLFILE = " << nclFile.c_str() << endl;
			pem->setIsLocalNcl(forceQuit, NULL);
			if (pem->openNclFile(nclFile)) {
				pem->startPresentation(nclFile);
				
/*#if HAVE_RECOMMENDER
	        		Scheduler* agenda = Scheduler::getInstance();
				cout << "-----------Before if(agenda != NULL)" << endl;
	        		if(agenda != NULL){
				cout << "-----------Before agenda->checkRecommenderModule" << endl;
	           			agenda->checkRecommenderModule();
	        		}
#endif*/
				pem->waitUnlockCondition();

/*#if HAVE_RECOMMENDER
				(Scheduler::getInstance())->stop();
				pthread_join(Scheduler::getInstance()->getThread(), NULL);
#endif*/
			}
		} else if (enableCac && fileExists(nclFile)) {
			cout << "WITH CAC == ginga main() NCLFILE = " << nclFile.c_str() << endl;
			pem->setIsLocalNcl(forceQuit, NULL);
			ProgramRecorder::getInstance()->showNcl(nclFile);
		}

		if (pem != NULL) {
			delete pem;
		}

		if (ccm != NULL) {
			delete ccm;
		}

#if HAVE_COMPSUPPORT
		printTimeStamp();
		cm->releaseComponentFromObject("PresentationEngineManager");
#endif
	}



	cout << "MAIN ALL DONE!" << endl;
	if (!forceQuit) {
		cout << "Press a key to continue" << endl;
		getchar();
	}
	return 0;
}

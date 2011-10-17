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

#ifndef _IComponentManager_H_
#define _IComponentManager_H_

#include "component/IComponent.h"

#include <dlfcn.h>
#include <iostream>
#include <map>
#include <string>
using namespace std;

typedef void* CMCreator();

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace cm {
	class IComponentManager {
		public:
			virtual ~IComponentManager(){};
			virtual void* getObject(string objectName)=0;
			virtual set<string>* getObjectsFromInterface(
					string interfaceName)=0;

			virtual map<string, set<string>*>* getUnsolvedDependencies()=0;
			virtual bool releaseComponentFromObject(string objName)=0;
			virtual void refreshComponentDescription()=0;
			virtual map<string, IComponent*>* getComponentDescription()=0;

		private:
			virtual void* getComponent(string dLibName)=0;
			virtual void* getSymbol(void* component, string symbolName)=0;
			virtual bool releaseComponent(void* component)=0;

		public:
			static IComponentManager* getCMInstance() {
				void* cmComponent = dlopen("libgingacccm.so", RTLD_LAZY);
				if (cmComponent == NULL) {
					cerr << "IComponentManager warning: cant load ";
					cerr << "component libgingaccm' => ";
					cerr << dlerror() << endl;
					return NULL;
				}

				dlerror();

				CMCreator* cmCreator = (CMCreator*)(dlsym(
						cmComponent, "createCM"));

				const char* dlsym_error = dlerror();
				if (dlsym_error != NULL) {
					cerr << "ComponentManager warning: cant load symbol '";
					cerr << "createCM' => " << dlsym_error << endl;
					return NULL;
				}

				IComponentManager* icm = (IComponentManager*)(cmCreator());

				dlerror();
				return icm;
			}
	};
}
}
}
}
}
}

#endif //_IComponentManager_H_

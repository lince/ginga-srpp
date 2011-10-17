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

#ifndef IINPUTMANAGER_H_
#define IINPUTMANAGER_H_

#include "interface/input/IEventBuffer.h"
#include "interface/input/IInputEventListener.h"

#include <set>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace system {
namespace io {
  class IInputManager {
	public:
		virtual ~IInputManager(){};

		virtual void release()=0;
		virtual void addInputEventListener(
				IInputEventListener* listener, set<int>* events=NULL)=0;

		virtual void removeInputEventListener(IInputEventListener* listener)=0;

		virtual void addApplicationInputEventListener(
				IInputEventListener* listener)=0;

		virtual void removeApplicationInputEventListener(
				IInputEventListener* listener)=0;

		virtual void postEvent(IInputEvent* event)=0;
		virtual void postEvent(int keyCode)=0;

		virtual void setAxisValues(int x, int y, int z)=0;
		virtual void setAxisBoundaries(int x, int y, int z)=0;
		virtual int getCurrentXAxisValue()=0;
		virtual int getCurrentYAxisValue()=0;
  };
}
}
}
}
}
}
}

typedef ::br::pucrio::telemidia::ginga::core::system::io::IInputManager*
		InputManagerCreator();

typedef void InputManagerDestroyer(
		::br::pucrio::telemidia::ginga::core::system::io::IInputManager* im);

#endif /*IINPUTMANAGER_H_*/

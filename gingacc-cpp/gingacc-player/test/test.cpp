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

#include "../include/ImagePlayer.h"
using namespace ::br::pucrio::telemidia::ginga::core::player;

#include "system/io/ILocalDeviceManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::system;

#include "../include/PlayersComponentSupport.h"

int main() {
	IWindow* w;
	ISurface* s;
	ILocalDeviceManager* dm;
	IPlayer* player;
	IPlayer* img;

#if HAVE_COMPSUPPORT
	IComponentManager* cm = IComponentManager::getCMInstance();
	dm = ((LocalDeviceManagerCreator*)(cm->getObject("LocalDeviceManager")))();
#else
	dm = LocalDeviceManager::getInstance();
#endif

	dm->createDevice("systemScreen(0)");
#if HAVE_COMPSUPPORT
	w = ((WindowCreator*)(cm->getObject("Window")))(10, 10, 100, 100);
#else
	w = new DFBWindow(10, 10, 100, 100);
#endif

	w->setCaps(w->getCap("ALPHACHANNEL"));
	w->draw();
	w->show();

	player = new Player("teste");
	img = new ImagePlayer("/root/bg_initializing.png");
	s = img->getSurface();
	w->renderFrom(s);

	//TODO: tests
	cout << "Player test has shown image. press enter to continue" << endl;
	getchar();
	dm->clearWidgetPools();
	cout << "Player test has released widgets. press enter to continue" << endl;
	getchar();

	return 0;
}

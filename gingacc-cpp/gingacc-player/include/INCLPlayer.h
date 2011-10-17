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

#ifndef _INCLPlayer_H_
#define _INCLPlayer_H_

#include "system/io/interface/content/ITimeBaseProvider.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include "IPlayer.h"

#include <vector>

typedef struct {
	string baseId;
	string playerId;
	int devClass;
	int x;
	int y;
	int w;
	int h;
	bool enableGfx;
	string parentDocId;
	string nodeId;
	string docId;
	void* focusManager;
} NclPlayerData;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {
	class INCLPlayer : public IPlayer {
		public:
			static const int DEEPEST_LEVEL = -1;

			virtual ~INCLPlayer(){};

			virtual void setStandByState(bool standBy)=0;
			virtual void setTimeBaseProvider(
					ITimeBaseProvider* timeBaseProvider)=0;

			virtual void setBackgroundImage(string uri)=0;
			virtual void setParentLayout(void* layout)=0;
			virtual string getScreenShot()=0;
			virtual void* setCurrentDocument(string fName)=0;
			virtual void setDepthLevel(int level)=0;
			virtual int getDepthLevel()=0;
			virtual bool editingCommand(
					string commandTag, string editingCommand)=0;

			virtual void pushEPGEventToEPGFactory(map<string, string> t)=0;

			virtual void setNotifyContentUpdate(bool notify)=0;
			virtual void addListener(IPlayerListener* listener)=0;
			virtual void removeListener(IPlayerListener* listener)=0;
			virtual void notifyListeners(
					short code, string paremeter, short type)=0;

			virtual void setSurface(io::ISurface* surface)=0;
			virtual io::ISurface* getSurface()=0;
			virtual int64_t getVPts(){return 0;};
			virtual double getMediaTime()=0;
			virtual void setMediaTime(double newTime)=0;
			virtual bool setKeyHandler(bool isHandler)=0;
			virtual void setScope(
					string scope,
					short type, double begin=-1, double end=-1)=0;

			virtual void play()=0;
			virtual void stop()=0;
			virtual void abort()=0;
			virtual void pause()=0;
			virtual void resume()=0;

			virtual string getPropertyValue(string name)=0;
			virtual void setPropertyValue(
					string name,
					string value, double duration=-1, double by=-1)=0;

			virtual void setReferenceTimePlayer(IPlayer* player)=0;
			virtual void addTimeReferPlayer(IPlayer* referPlayer)=0;
			virtual void removeTimeReferPlayer(IPlayer* referPlayer)=0;
			virtual void notifyReferPlayers(int transition)=0;
			virtual void timebaseObjectTransitionCallback(int transition)=0;
			virtual void setTimeBasePlayer(IPlayer* timeBasePlayer)=0;
			virtual bool hasPresented()=0;
			virtual void setPresented(bool presented)=0;
			virtual bool isVisible()=0;
			virtual void setVisible(bool visible)=0;
			virtual bool immediatelyStart()=0;
			virtual void setImmediatelyStart(bool immediatelyStartVal)=0;
			virtual void forceNaturalEnd()=0;
			virtual bool isForcedNaturalEnd()=0;
			virtual bool setOutWindow(io::IWindow* w)=0;

			/*Exclusive for ChannelPlayer*/
			virtual IPlayer* getSelectedPlayer()=0;
			virtual void setPlayerMap(map<string, IPlayer*>* objs)=0;
			virtual map<string, IPlayer*>* getPlayerMap()=0;
			virtual IPlayer* getPlayer(string objectId)=0;
			virtual void select(IPlayer* selObject)=0;

			/*Exclusive for Application Players*/
			virtual void setCurrentScope(string scopeId)=0;
			virtual void flip()=0;

			virtual string getDepUris(vector<string>* uris, int targetDev=0)=0;

			virtual void timeShift(string direction)=0;
	};
}
}
}
}
}
}

typedef ::br::pucrio::telemidia::ginga::core::player::INCLPlayer*
	NCLPlayerCreator(NclPlayerData* data);

typedef void NCLPlayerDescroyer(
		::br::pucrio::telemidia::ginga::core::player::INCLPlayer* p);

#endif //_INCLPlayer_H_

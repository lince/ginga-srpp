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

#ifndef PMT_H_
#define PMT_H_

#include "TSSection.h"
using namespace ::br::pucrio::telemidia::ginga::core::tsparser;

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

#include <map>
#include <string>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace tsparser {
namespace si {
	class Pmt : public TSSection {
		protected:
			map<unsigned int, short>* streams;
			map<short, string>* streamTypeNames;
			//map<unsigned int, AITInfo*>* aits;

			unsigned int pid;
			unsigned int programNumber;
			unsigned int pcrPid;
			bool processed;

		public:
			Pmt(unsigned int pid, unsigned int programNumber);
			void addElementaryStream(unsigned int pid, short esType);
			//void addAITInfo(unsigned int pid, AITInfo* info);
			unsigned int getPid();
			void setPid(unsigned int pid);
			vector<unsigned int>* getPidsByTid(unsigned int tid);
			unsigned int getProgramNumber();
			void setProgramNumber(unsigned int programNumber);
			bool hasPid(unsigned int somePid);
			string getStreamType(unsigned int somePid);
			bool isSectionType(unsigned int pid);
			short getStreamTypeValue(unsigned int somePid);
			string getStreamTypeName(short streamType);
			bool processSectionPayload();
			bool hasProcessed();
			map<unsigned int, short>* getStreamsInformation();
			unsigned int getPCRPid();
			unsigned int getDefaultMainVideoPid();
			unsigned int getDefaultMainAudioPid();
			map<unsigned int, short>* getElStreams();
			void print();
	 };
}
}
}
}
}
}
}

#endif /*PMT_H_*/

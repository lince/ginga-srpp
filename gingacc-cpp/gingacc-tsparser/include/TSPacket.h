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

#ifndef TSPACKET_
#define TSPACKET_

#include "Pat.h"
using namespace ::br::pucrio::telemidia::ginga::core::tsparser::si;

#include "ITSPacket.h"

#include <iostream>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace tsparser {
	class TSPacket : public ITSPacket {
	protected:
		//
		// ISO/IEC 13818-1 TS PACKET HEADER
		//

	        // Value must be 0x47. (1 byte)
		char syncByte;

		// Error indicator flag. (1 bit)
		//   1    Error
		//   0    OK
		bool transportErrorIndication;

		// Indicator that carries PES packets or PSI data.
		// (1 bit)
		//   1    Indicates that the payload of this TS packet
		//        will start with the first byte of a PES
		//        packet.
		//   0    Indicates no PES packet shall start in this
		//        TS packet.
		bool payloadUnitStartIndicator;

		// Indicates that the associated packet is of greater
		// priority than other packets having the same PID.
		bool transportPriority;

		// Indicates the type of the data stored in
		// the payload.  (13 bits)
		//   0x00    PAT
		//   0x01    CAT
		//   0x02    TS description table
		//   0x03    IPMP control
		unsigned int pid;

		// Indicates the scrambling mode of the TS packet.
		// (2 bits)
		//   00    Not scrambled
		//   01    User Defined
		//   10    User Defined
		//   11    User Defined
		unsigned int transportScramblingControl;

		// Indicates whether this TS packet header is followed
		// by an adaptation field and/or payload. (2 bits)
		//   00    ISO future use; packet must be discarded
		//   01    no adaptation field (payload only)
		//   10    adaptation field only (no payload)
		//   11    adaptation field followed by payload
		static const unsigned int FUTURE_USE = 0;
		static const unsigned int PAYLOAD_ONLY = 1;
		static const unsigned int NO_PAYLOAD = 2;
		static const unsigned int ADAPT_PAYLOAD = 3;
		unsigned int adaptationFieldControl;

		// Incrementing with each TS packet with the same PID.
		// This field shall not be incremented when the
		// adaptationFieldControl is set to 00 or 10. (4 bits)
		unsigned int continuityCounter;

		// TODO: AdaptationField?

		// Number of bytes immediately following this
		// pointerField until the first byte of the first
		// section that is present in the payload.  The
		// maximum value is 182.  If the
		// payloadUnitStartIndicator is set to 1, then the
		// first byte of the payload is the pointerField
		// itself.
		unsigned int pointerField;

		// Packet payload data.  (184 bytes) If pointerField
		// is present, then the payload size is 183 bytes.
		char payload[TS_PAYLOAD_SIZE];
		unsigned int payloadSize;
	public:
		TSPacket(char* packetData);
		~TSPacket();

	protected:
		bool create(char data[TS_PACKET_SIZE]);
		bool constructionFailed;

	public:
		bool isConstructionFailed();
		unsigned int getPid();
		void getPacketData(char streamData[TS_PACKET_SIZE]);
		void getPayload(char streamData[TS_PAYLOAD_SIZE]);
		unsigned int getPayloadSize();
		bool getStartIndicator();
		unsigned int getPointerField();
		unsigned int getAdaptationFieldControl();
		unsigned int getContinuityCounter();
		void setContinuityCounter(unsigned int counter);
		void print();
	};
}
}
}
}
}
}

#endif /*TSPACKET_*/

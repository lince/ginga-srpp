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

#ifndef IAPPLICATIONPLAYER_H_
#define IAPPLICATIONPLAYER_H_

#include <string>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace player {
	//nclEdit base
	static const string EC_OPEN_BASE                     = "0x0";
	static const string EC_ACTIVATE_BASE                 = "0x1";
	static const string EC_DEACTIVATE_BASE               = "0x2";
	static const string EC_SAVE_BASE                     = "0x3";
	static const string EC_CLOSE_BASE                    = "0x4";

	//nclEdit document
	static const string EC_ADD_DOCUMENT                  = "0x5";
	static const string EC_REMOVE_DOCUMENT               = "0x6";
	static const string EC_START_DOCUMENT                = "0x7";
	static const string EC_STOP_DOCUMENT                 = "0x8";
	static const string EC_PAUSE_DOCUMENT                = "0x9";
	static const string EC_RESUME_DOCUMENT               = "0xA";

	//nclEdit region
	static const string EC_ADD_REGION                    = "0xB";
	static const string EC_REMOVE_REGION                 = "0xC";
	static const string EC_ADD_REGION_BASE               = "0xD";
	static const string EC_REMOVE_REGION_BASE            = "0xE";

	//nclEdit rule
	static const string EC_ADD_RULE                      = "0xF";
	static const string EC_REMOVE_RULE                   = "0x10";
	static const string EC_ADD_RULE_BASE                 = "0x11";
	static const string EC_REMOVE_RULE_BASE              = "0x12";

	//nclEdit connector
	static const string EC_ADD_CONNECTOR                 = "0x13";
	static const string EC_REMOVE_CONNECTOR              = "0x14";
	static const string EC_ADD_CONNECTOR_BASE            = "0x15";
	static const string EC_REMOVE_CONNECTOR_BASE         = "0x16";

	//nclEdit descriptor
	static const string EC_ADD_DESCRIPTOR                = "0x17";
	static const string EC_REMOVE_DESCRIPTOR             = "0x18";
	static const string EC_ADD_DESCRIPTOR_SWITCH         = "0x19";
	static const string EC_REMOVE_DESCRIPTOR_SWITCH      = "0x1A";
	static const string EC_ADD_DESCRIPTOR_BASE           = "0x1B";
	static const string EC_REMOVE_DESCRIPTOR_BASE        = "0x1C";

	//nclEdit transition
	static const string EC_ADD_TRANSITION                = "0x1D";
	static const string EC_REMOVE_TRANSITION             = "0x1E";
	static const string EC_ADD_TRANSITION_BASE           = "0x1F";
	static const string EC_REMOVE_TRANSITION_BASE        = "0x20";

	//nclEdit reuse
	static const string EC_ADD_IMPORT_BASE               = "0x21";
	static const string EC_REMOVE_IMPORT_BASE            = "0x22";
	static const string EC_ADD_IMPORTED_DOCUMENT_BASE    = "0x23";
	static const string EC_REMOVE_IMPORTED_DOCUMENT_BASE = "0x24";
	static const string EC_ADD_IMPORT_NCL                = "0x25";
	static const string EC_REMOVE_IMPORT_NCL             = "0x26";

	//nclEdit components
	static const string EC_ADD_NODE                      = "0x27";
	static const string EC_REMOVE_NODE                   = "0x28";

	//nclEdit interface
	static const string EC_ADD_INTERFACE                 = "0x29";
	static const string EC_REMOVE_INTERFACE              = "0x2A";

	//nclEdit link
	static const string EC_ADD_LINK                      = "0x2B";
	static const string EC_REMOVE_LINK                   = "0x2C";

	static const string EC_SET_PROPERTY_VALUE            = "0x2D";
}
}
}
}
}
}

#endif /*IAPPLICATIONPLAYER_H_*/

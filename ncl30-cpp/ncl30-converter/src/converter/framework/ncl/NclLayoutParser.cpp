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

#include "../../../../include/framework/ncl/NclLayoutParser.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace converter {
namespace framework {
namespace ncl {
	NclLayoutParser::NclLayoutParser(
		    DocumentParser *documentParser) : ModuleParser(documentParser) {


	}

	void *NclLayoutParser::parseRegion(
		    DOMElement *parentElement, void *objGrandParent) {

		wclog << "parseRegion" << endl;
		void *parentObject;
		DOMNodeList *elementNodeList;
		DOMElement *element;
		DOMNode *node;
		string elementTagName;
		void *elementObject;

		parentObject = createRegion(parentElement, objGrandParent);
		if (parentObject == NULL) {
			return NULL;
		}

		elementNodeList = parentElement->getChildNodes();
		for (int i = 0; i < (int)elementNodeList->getLength(); i++) {
			node = elementNodeList->item(i);
			if (node->getNodeType()==DOMNode::ELEMENT_NODE) {
				element = (DOMElement*)node;
				elementTagName = XMLString::transcode(element->getTagName());
				wclog << ">>" << elementTagName.c_str() << ": ";
				wclog << XMLString::transcode(element->getAttribute(XMLString::transcode("id")));

				if (XMLString::compareIString(
					    elementTagName.c_str(), "region") == 0) {

					elementObject = parseRegion(element, parentObject);
					if (elementObject != NULL) {
						addRegionToRegion(parentObject, elementObject);
					}
				}
			}
		}

		return parentObject;
	}

	void *NclLayoutParser::parseRegionBase(
		    DOMElement *parentElement, void *objGrandParent) {

		wclog << "parseRegionBase" << endl;
		void *parentObject;
		DOMNodeList *elementNodeList;
		DOMElement *element;
		DOMNode *node;
		string elementTagName;
		void *elementObject;

		parentObject = createRegionBase(parentElement, objGrandParent);
		if (parentObject == NULL) {
			return NULL;
		}

		elementNodeList = parentElement->getChildNodes();
		for (int i = 0; i < (int)elementNodeList->getLength(); i++) {
			node = elementNodeList->item(i);
			if(node->getNodeType()==DOMNode::ELEMENT_NODE){
				element = (DOMElement*)node;
				elementTagName = XMLString::transcode( element->getTagName() );
				wclog << ">>" << elementTagName.c_str() << ": ";
				wclog << XMLString::transcode(element->getAttribute(XMLString::transcode("id")));

				if (XMLString::compareIString(
					    elementTagName.c_str(), "importBase") == 0) {

					elementObject = getImportParser()->parseImportBase(
						    element, parentObject);

					if (elementObject != NULL) {
						addImportBaseToRegionBase(parentObject, elementObject);
					}

				} else if (XMLString::compareIString(
					    elementTagName.c_str(), "region") == 0) {

					elementObject = parseRegion(element, parentObject);
					if (elementObject != NULL) {
						addRegionToRegionBase(parentObject, elementObject);
					}
				}
			}
		}

		return parentObject;
	}

	NclImportParser *NclLayoutParser::getImportParser() {
		return importParser;
	}

	void NclLayoutParser::setImportParser(NclImportParser *importParser) {
		this->importParser = importParser;
	}
}
}
}
}
}
}

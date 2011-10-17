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

#include "../../../include/ncl/NclDocumentConverter.h"
#include "../../../include/ncl/NclConnectorsConverter.h"
#include "../../../include/ncl/NclImportConverter.h"
#include "../../../include/ncl/NclPresentationControlConverter.h"
#include "../../../include/ncl/NclComponentsConverter.h"
#include "../../../include/ncl/NclStructureConverter.h"
#include "../../../include/ncl/NclPresentationSpecConverter.h"
#include "../../../include/ncl/NclLayoutConverter.h"
#include "../../../include/ncl/NclInterfacesConverter.h"
#include "../../../include/ncl/NclLinkingConverter.h"
#include "../../../include/ncl/NclTransitionConverter.h"
#include "../../../include/ncl/NclMetainformationConverter.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace converter {
namespace ncl {
	NclDocumentConverter::NclDocumentConverter(
			IPrivateBaseContext* baseContext, IDeviceLayout* deviceLayout) :
				NclDocumentParser(deviceLayout) {

		this->privateBaseContext = baseContext;
		this->ownManager         = false;
		this->parseEntityVar     = false;

		setDocumentPath(getCurrentPath() + "/");
		initialize();
		setDependencies();
	}

	NclDocumentConverter::~NclDocumentConverter() {
		if (structureParser != NULL) {
			delete structureParser;
			structureParser = NULL;
		}

		if (componentsParser != NULL) {
			delete componentsParser;
			componentsParser = NULL;
		}

		if (connectorsParser != NULL) {
			delete connectorsParser;
			connectorsParser = NULL;
		}

		if (linkingParser != NULL) {
			delete linkingParser;
			linkingParser = NULL;
		}

		if (interfacesParser != NULL) {
			delete interfacesParser;
			interfacesParser = NULL;
		}

		if (layoutParser != NULL) {
			delete layoutParser;
			layoutParser = NULL;
		}

		if (transitionParser != NULL) {
			delete transitionParser;
			transitionParser = NULL;
		}

		if (presentationControlParser != NULL) {
			delete presentationControlParser;
			presentationControlParser = NULL;
		}

		if (importParser != NULL) {
			delete importParser;
			importParser = NULL;
		}

		if (metainformationParser != NULL) {
			delete metainformationParser;
			metainformationParser = NULL;
		}
	}

	void NclDocumentConverter::initialize() {
		presentationSpecificationParser = new
			    NclPresentationSpecConverter(this);

		structureParser = new NclStructureConverter(this);
		componentsParser = new NclComponentsConverter(this);
		connectorsParser = new NclConnectorsConverter(this);
		linkingParser = new NclLinkingConverter(this,
			    (NclConnectorsConverter*) connectorsParser);

		interfacesParser = new NclInterfacesConverter(this);
		layoutParser = new NclLayoutConverter(this, deviceLayout);
		transitionParser = new NclTransitionConverter(this);
		presentationControlParser = new NclPresentationControlConverter(this);
		importParser = new NclImportConverter(this);
		metainformationParser = new NclMetainformationConverter(this);
	}

	string NclDocumentConverter::getAttribute(
			void* element, string attribute) {

		return XMLString::transcode(((DOMElement*)element)->getAttribute(
					     XMLString::transcode(attribute.c_str())));
	}

	Node *NclDocumentConverter::getNode(string nodeId) {
		NclDocument *document;

		document = (NclDocument*)NclDocumentParser::getObject(
			    "return", "document");

		return document->getNode(nodeId);
	}

	bool NclDocumentConverter::removeNode(Node *node) {
		NclDocument *document;
		document = (NclDocument*)NclDocumentParser::getObject(
			    "return", "document");

		//return document->removeDocument()
		return true;
	}

	IPrivateBaseContext* NclDocumentConverter::getPrivateBaseContext() {
		return privateBaseContext;
	}

	NclDocument* NclDocumentConverter::importDocument(string docLocation) {
		string uri;

		if (!isAbsolutePath(docLocation)) {
			if (docLocation.find_first_of("/") == std::string::npos) {
				uri = getAbsolutePath(docLocation) + "/" + docLocation;

			} else {
				uri = getAbsolutePath(docLocation) + docLocation.substr(
						docLocation.find_last_of("/"),
						docLocation.length() - docLocation.find_last_of("/"));
			}

		} else {
			uri = docLocation;
		}

		return (NclDocument*)(privateBaseContext->addVisibleDocument(
				uri, deviceLayout));
	}

	void* NclDocumentConverter::parseEntity(
		    string entityLocation, NclDocument* document, void* parent) {

		void* entity;

		parseEntityVar = true;
		parentObject = parent;
		addObject("return", "document", document);
		entity = parse(entityLocation);
		parseEntityVar = false;
		return entity;
	}

	void* NclDocumentConverter::parseRootElement(DOMElement* rootElement) {
		string elementName;
		void* object;

		if (parseEntityVar) {
			elementName = XMLString::transcode(rootElement->getTagName());
			if (elementName == "region") {
				return getLayoutParser()->parseRegion(
						rootElement, parentObject);

			} else if (elementName == "regionBase") {
				return getLayoutParser()->parseRegionBase(
					    rootElement, parentObject);

			} else if (elementName == "transition") {
				return getTransitionParser()->parseTransition(
					    rootElement, parentObject);

			} else if (elementName == "transitionBase") {
				return getTransitionParser()->parseTransitionBase(
					    rootElement, parentObject);

			} else if (elementName == "rule") {
				return getPresentationControlParser()->parseRule(
					    rootElement, parentObject);

			} else if (elementName == "compositeRule") {
				return getPresentationControlParser()->parseRule(
					    rootElement, parentObject);

			} else if (elementName == "ruleBase") {
				return getPresentationControlParser()->parseRuleBase(
					    rootElement, parentObject);

			} else if (elementName == "causalConnector") {
				return getConnectorsParser()->parseCausalConnector(
					    rootElement, parentObject);

			} else if (elementName == "connectorBase") {
				return getConnectorsParser()->parseConnectorBase(
					    rootElement, parentObject);

			} else if (elementName == "descriptor") {
				return getPresentationSpecificationParser()->parseDescriptor(
					    rootElement, parentObject);

			} else if (elementName == "descriptorSwitch") {
				return getPresentationControlParser()->parseDescriptorSwitch(
					    rootElement, parentObject);

			} else if (elementName == "descriptorBase") {
				return getPresentationSpecificationParser()->
					    parseDescriptorBase(rootElement, parentObject);

			} else if (elementName == "importBase") {
				// return the root element
				return getImportParser()->parseImportBase(
					    rootElement, parentObject);

			} else if (elementName == "importedDocumentBase") {
				// return the root element
				return getImportParser()->parseImportedDocumentBase(
					    rootElement, parentObject);

			} else if (elementName == "importNCL") {
				// return the root element
				return getImportParser()->parseImportNCL(
					    rootElement, parentObject);

			} else if (elementName == "media") {
				return getComponentsParser()->parseMedia(
					    rootElement, parentObject);

			} else if (elementName == "context") {
				object = getComponentsParser()->parseContext(
					    rootElement, parentObject);

				getComponentsParser()->posCompileContext(rootElement, object);
				return object;

			} else if (elementName == "switch") {
				object = getPresentationControlParser()->parseSwitch(
					    rootElement, parentObject);

				getPresentationControlParser()->posCompileSwitch(
					    rootElement, object);

				return object;

			} else if (elementName == "link") {
				return getLinkingParser()->parseLink(
					    rootElement, parentObject);

			} else if (elementName == "area") {
				return getInterfacesParser()->parseArea(
					    rootElement, parentObject);

			} else if (elementName == "property") {
				return getInterfacesParser()->parseProperty(
					    rootElement, parentObject);

			} else if (elementName == "port") {
				return getInterfacesParser()->parsePort(
					    rootElement, parentObject);

			} else if (elementName == "switchPort") {
				return getInterfacesParser()->parseSwitchPort(
					    rootElement, parentObject);

			} else {
				return NULL;
			}

		} else {
			return NclDocumentParser::parseRootElement(rootElement);
		}

		return NULL;
	}

/*
	void *NclDocumentConverter::parseRootElement(DOMElement *rootElement){
		if ( XMLString::compareIString( rootElement->getTagName(),
			    XMLString::transcode("ncl") ) == 0) {

			return getStructureParser()->parseNcl(rootElement, NULL);
		}

		wclog << "Wrong Document" << endl;
		return NULL;
	}*/
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::converter::IDocumentConverter*
		createNclDocumentConverter(
				IPrivateBaseContext* pbc, IDeviceLayout* deviceLayout) {

	return new ::br::pucrio::telemidia::converter::ncl::NclDocumentConverter(
			pbc, deviceLayout);
}

extern "C" void destroyNclDocumentConverter(
		::br::pucrio::telemidia::converter::IDocumentConverter* doc) {

	return delete doc;
}

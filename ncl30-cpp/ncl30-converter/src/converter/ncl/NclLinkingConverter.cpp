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

#include "../../../include/ncl/NclLinkingConverter.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace converter {
namespace ncl {
	NclLinkingConverter::NclLinkingConverter(
		    DocumentParser *documentParser,
		    NclConnectorsConverter *connCompiler) :
		    	    NclLinkingParser(documentParser) {


	}

	void NclLinkingConverter::addBindToLink(
		    void *parentObject, void *childObject) {

		// nothing to do, since to be created the bind needs to be associated
		// with
		// its link
	}

	void NclLinkingConverter::addBindParamToBind(
		    void *parentObject, void *childObject) {

		((Bind*) parentObject)->addParameter((Parameter*) childObject);
	}

	void NclLinkingConverter::addLinkParamToLink(
		    void *parentObject, void *childObject) {

		((Link*) parentObject)->addParameter((Parameter*) childObject);
	}

	void *NclLinkingConverter::createBind(
		    DOMElement *parentElement, void *objGrandParent) {

		string component, roleId;
		Role* role;
		Node* anchorNode;
		NodeEntity* anchorNodeEntity;
		InterfacePoint* interfacePoint;
		NclDocument* document;
		GenericDescriptor* descriptor;

		// papel
		role = connector->getRole(XMLString::transcode(
			    parentElement->getAttribute(XMLString::transcode("role"))));

		// no' ancora do bind
		component = XMLString::transcode(parentElement->getAttribute(
				XMLString::transcode("component")));

		if (composite->getId() == component) {
			anchorNode = (Node*)composite;

		} else {
			anchorNode = (Node*)(composite->getNode(component));
		}

		// TODO - verificar erro na autoria
		if (anchorNode == NULL) {
			cout << "NclLinkingConverter::createBind Warning!";
			cout << " anchorNode == NULL for component '" << component;
			cout << "', return NULL" << endl;
			return NULL;
		}

		anchorNodeEntity = (NodeEntity*)(anchorNode->getDataEntity());

		// ponto de interface do bind
		// testa se o bind define uma interface
		if (parentElement->hasAttribute(XMLString::transcode("interface"))) {
			if (anchorNodeEntity == NULL) {
				interfacePoint = NULL;

			} else {
				interfacePoint = anchorNodeEntity->getAnchor(
						XMLString::transcode(parentElement->getAttribute(
								XMLString::transcode("interface"))));
			}

			if (interfacePoint == NULL) {
				if (anchorNodeEntity != NULL &&
						anchorNodeEntity->instanceOf("CompositeNode")) {

					interfacePoint = ((CompositeNode*)anchorNodeEntity)->
							getPort(XMLString::transcode(parentElement->
									getAttribute(XMLString::transcode(
											"interface"))));
				} else {
					interfacePoint = anchorNode->getAnchor(
							XMLString::transcode(parentElement->
									getAttribute(XMLString::transcode(
											"interface"))));
				}

				if (interfacePoint == NULL) {
					cout << "NclLinkingConverter::createBind Warning!";
					cout << " interfacePoint == NULL, return NULL for '";
					cout << anchorNodeEntity->getId() << "'";
					if (role != NULL) {
						cout << " for role '" << role->getLabel() << "'";
					}
					cout << " in connector '" << connector->getId() << "'";
					cout << endl;
					return NULL;
				}
			}

		} else if (anchorNodeEntity != NULL) {
			// se nao houver interface, faz bind para a ancora lambda
			interfacePoint = anchorNodeEntity->getAnchor(0);

		} else {
			// se nao houver interface, faz bind para a ancora lambda
			interfacePoint = anchorNode->getAnchor(0);
		}

		// atribui o bind ao elo (link)
		if (parentElement->hasAttribute(XMLString::transcode("descriptor"))) {
			document = (NclDocument*) getDocumentParser()->getObject(
				    "return", "document");

			descriptor = document->getDescriptor(XMLString::transcode(
				     parentElement->getAttribute(XMLString::transcode(
				     	    "descriptor"))));
		} else {
			descriptor = NULL;
		}

		if (role == NULL) {
			// &got
			if (parentElement->hasAttribute(XMLString::transcode("role"))) {
				ConditionExpression* condition;
				CompoundCondition* compoundCondition;
				AssessmentStatement* statement;
				AttributeAssessment* assessment;
				ValueAssessment* otherAssessment;

				roleId = XMLString::transcode(parentElement->getAttribute(
						XMLString::transcode("role")));

				cout << "NclLinkingConverter::createBind FOUND GOT '";
				cout << roleId << "'" << endl;
				assessment = new AttributeAssessment(roleId);
				assessment->setEventType(EventUtil::EVT_ATTRIBUTION);
				assessment->setAttributeType(EventUtil::ATT_NODE_PROPERTY);
				assessment->setMinCon(0);
				assessment->setMaxCon(Role::UNBOUNDED);

				otherAssessment = new ValueAssessment(roleId);

				statement = new AssessmentStatement(Comparator::CMP_NE);
				statement->setMainAssessment(assessment);
				statement->setOtherAssessment(otherAssessment);

				condition = ((CausalConnector*)
						connector)->getConditionExpression();

				if (condition->instanceOf("CompoundCondition")) {
					((CompoundCondition*)condition)->addConditionExpression(
							statement);

				} else {
					compoundCondition = new CompoundCondition(
							condition, statement, CompoundCondition::OP_OR);

					((CausalConnector*)connector)->setConditionExpression(
							(ConditionExpression*)compoundCondition);
				}
				role = (Role*)assessment;

			} else {
				return NULL;
			}
		}

		return ((Link*) objGrandParent)->bind(
			    anchorNode, interfacePoint, descriptor, role->getLabel());
	}

	void *NclLinkingConverter::createLink(
		    DOMElement *parentElement, void *objGrandParent) {

		NclDocument *document;
		Link *link;

		// obtendo o conector do link
		document = (NclDocument*) getDocumentParser()->getObject(
			    "return", "document");

		connector = document->getConnector(XMLString::transcode(
			   parentElement->getAttribute(XMLString::transcode(
			   	    "xconnector"))));

		if (connector == NULL) {
			// connector not found
			return NULL;
		}

		// criando o link
		if (connector->instanceOf("CausalConnector")) {
			link = new CausalLink(getId(parentElement), connector);

		} else {
			link = NULL;
		}

		composite = (CompositeNode*) objGrandParent;
		return link;
	}

	void *NclLinkingConverter::createBindParam(
		    DOMElement *parentElement, void *objGrandParent) {

		Parameter *parameter;
		parameter = new Parameter(
			    XMLString::transcode(parentElement->getAttribute(
			    	    XMLString::transcode("name"))),

			    XMLString::transcode(parentElement->getAttribute(
			    	    XMLString::transcode("value"))));

		return parameter;
	}

	void *NclLinkingConverter::createLinkParam(
		    DOMElement *parentElement, void *objGrandParent) {

		Parameter *parameter;
		parameter = new Parameter(
			    XMLString::transcode(parentElement->getAttribute(
			    	     XMLString::transcode("name"))),

			    XMLString::transcode(parentElement->getAttribute(
			    	     XMLString::transcode("value"))));

		return parameter;
	}

	string NclLinkingConverter::getId(DOMElement *element) {
		string strRet = "";
		if (element->hasAttribute(XMLString::transcode("id"))) {
			strRet = XMLString::transcode(element->getAttribute(
				    XMLString::transcode("id")));
		} else {
			strRet = "";//"NclLinkingConverterId" + idCount++;
		}
		wclog << strRet.c_str() << endl;
		return strRet;

	}
}
}
}
}
}

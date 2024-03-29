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

#include "../../include/components/ContextNode.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ncl {
namespace components {
	ContextNode::ContextNode(string id):
		    CompositeNode(id) {

		descriptorCollection = new map<string, GenericDescriptor*>;
		linkSet = new vector<Link*>;
		nodes = new vector<Node*>;
		typeSet.insert("ContextNode");
		typeSet.insert("DocumentNode");
		typeSet.insert("LinkComposition");
	}

	ContextNode::~ContextNode() {
		if (descriptorCollection != NULL) {
			delete descriptorCollection;
			descriptorCollection = NULL;
		}

		if (linkSet != NULL) {
			delete linkSet;
			linkSet = NULL;
		}

		if (nodes != NULL) {
			delete nodes;
			nodes = NULL;
		}
	}

	bool ContextNode::addLink(Link* link) {
		if (link == NULL)
			return false;

		linkSet->push_back(link);
		link->setParentComposition(this);
		return true;
	}

	bool ContextNode::addNode(Node* node) throw(IllegalNodeTypeException*) {
		if (!node->instanceOf("DocumentNode")) {
			throw(new IllegalNodeTypeException());
		}

		if (node == NULL || this->getNode(node->getId()) != NULL) {
			return false;
		}

		nodes->push_back(node);
		node->setParentComposition(this);
		return true;
	}

	void ContextNode::clearLinks() {
		vector<Link*>::iterator it;
		Link *link;

		for(it=linkSet->begin(); it!=linkSet->end(); ++it) {
			link = (Link*) (*it);
			link->setParentComposition(NULL);
		}
		linkSet->clear();
	}

	bool ContextNode::containsLink(Link* link) {
		vector<Link*>::iterator i;

		for (i = linkSet->begin(); i != linkSet->end(); ++i) {
			if ((*i) == link) {
				return true;
			}
		}
		return false;
	}

	vector<Link*>* ContextNode::getLinks() {
		wclog << "ContextNode::getLinks ";
		if (this->linkSet == NULL || this->linkSet->size() == 0) {
			wclog << "linkSet is NULL or his size is 0" << endl;
			return NULL;
		}

		wclog << "returning linkSet with size = " << linkSet->size() << endl;
		return this->linkSet;
	}

	Link* ContextNode::getLink(string linkId) {
		vector<Link*>::iterator i;

		i = linkSet->begin();
		while (i != linkSet->end()) {
			if ((*i)->getId() != "" && (*i)->getId() == linkId) {
				return *i;
			}
			++i;
		}
		return NULL;
	}

	GenericDescriptor* ContextNode::getNodeDescriptor(Node *node) {
		if (descriptorCollection->count(node->getId()) != 0) {
			return (*descriptorCollection)[node->getId()];
		}

		return NULL;
	}

	int ContextNode::getNumLinks() {
		return (int)(linkSet->size());
	}

	bool ContextNode::removeLink(Link *link) {
		vector<Link*>::iterator it;
		for (it = linkSet->begin(); it != linkSet->end(); ++it) {
			if ((*it)->getId() == link->getId()) {
				if (linkSet->erase(it) == linkSet->end()) {
					link->setParentComposition(NULL);
					return true;
				}
			}
		}
		return false;
	}

	bool ContextNode::setNodeDescriptor(
		    string nodeId, GenericDescriptor* descriptor) {

		Node *node;
		node = getNode(nodeId);
		if (node == NULL) {
			return false;
		}

		if (descriptorCollection->count(nodeId) != 0) {
			if (descriptor == NULL) {
				descriptorCollection->erase(
					    descriptorCollection->find(nodeId));

				return true;
			}
		} else if (descriptor == NULL) {
			return true;
		}

		(*descriptorCollection)[nodeId] = descriptor;
		return true;
	}
}
}
}
}
}

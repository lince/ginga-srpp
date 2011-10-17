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

#ifndef NclPresentationControlConverter_H
#define NclPresentationControlConverter_H

#include "ncl/reuse/ReferNode.h"
#include "ncl/reuse/ReferredNode.h"
using namespace ::br::pucrio::telemidia::ncl::reuse;

#include "ncl/descriptor/Descriptor.h"
using namespace ::br::pucrio::telemidia::ncl::descriptor;

#include "ncl/switches/DescriptorSwitch.h"
#include "ncl/switches/SwitchNode.h"
#include "ncl/switches/CompositeRule.h"
#include "ncl/switches/SimpleRule.h"
#include "ncl/switches/Rule.h"
#include "ncl/switches/RuleBase.h"
using namespace ::br::pucrio::telemidia::ncl::switches;

#include "ncl/interfaces/Port.h"
using namespace ::br::pucrio::telemidia::ncl::interfaces;

#include "ncl/util/Comparator.h"
using namespace ::br::pucrio::telemidia::ncl::util;

#include "ncl/NclDocument.h"
using namespace ::br::pucrio::telemidia::ncl;

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "../framework/ModuleParser.h"
#include "../framework/DocumentParser.h"
using namespace ::br::pucrio::telemidia::converter::framework;

#include "../framework/ncl/NclPresentationControlParser.h"
using namespace ::br::pucrio::telemidia::converter::framework::ncl;

#include "NclPresentationSpecConverter.h"
#include "NclComponentsConverter.h"

#include <xercesc/dom/DOM.hpp>
XERCES_CPP_NAMESPACE_USE

namespace br {
namespace pucrio {
namespace telemidia {
namespace converter {
namespace ncl {
  class NclPresentationControlConverter : public NclPresentationControlParser {
	// tabelas com componentes e regras de cada switch
	// para reusar logica
	// 1 copiar tabelas abaixo e inicializar
	// 2 copiar os metodos:
	// addBindRuleGroupToContentControlGroup,
	// addCompositionContentGroupToContentControlGroup
	// addMediaContentGroupToContentControlGroup
	// createBindRuleGroup
	// addContentControlGroupToContentControlGroup
	// posCreateContentControlGroup
	// addDescriptorGroupToDescriptorSwitchGroup
	// addBindRuleGroupToDescriptorSwitchGroup
	// 3 adicionar posCreateDescriptorSwitchGroup no createDescriptorSwitchGroup
	// 3 adicionar posCreateContentControlGroup no createContentControlGroup
	// 4 copiar adaptando posCompileContentControlGroup
	// 4 copiar adaptando posCompileDescriptorSwitchGroup
	private:
		map<string, map<string, NodeEntity*>*>* switchConstituents;

	public:
		NclPresentationControlConverter(DocumentParser *documentParser);
		vector<Node*> *getSwitchConstituents(SwitchNode *switchNode);
		void addCompositeRuleToCompositeRule(
			    void *parentObject, void *childObject);

		void addCompositeRuleToRuleBase(void *parentObject, void *childObject);
		void addRuleToCompositeRule(void *parentObject, void *childObject);
		void addRuleToRuleBase(void *parentObject, void *childObject);
		void addSwitchPortToSwitch(void *parentObject, void *childObject);
		void *createCompositeRule(
			    DOMElement *parentElement, void *objGrandParent);

		void *createSwitch(DOMElement *parentElement, void *objGrandParent);
		void *createRuleBase(DOMElement *parentElement, void *objGrandParent);
		void *createRule(DOMElement *parentElement, void *objGrandParent);
		void *createDescriptorSwitch(
			    DOMElement *parentElement, void *objGrandParent);

		void addDescriptorToDescriptorSwitch(
			    void *parentObject, void *childObject);

		void addImportBaseToRuleBase(void *parentObject, void *childObject);
		void addBindRuleToDescriptorSwitch(
			    void *parentObject, void *childObject);

		void addBindRuleToSwitch(void *parentObject, void *childObject);
		void addDefaultComponentToSwitch(void *parentObject, void *childObject);
		void addDefaultDescriptorToDescriptorSwitch(
			    void *parentObject, void *childObject);

		void addContextToSwitch(void *parentObject, void *childObject);
		void addMediaToSwitch(void *parentObject, void *childObject);
		void addSwitchToSwitch(void *parentObject, void *childObject);

	private:
		void addNodeToSwitch(SwitchNode *switchNode, NodeEntity *node);

	public:
		void *createBindRule(DOMElement *parentElement, void *objGrandParent);
		void *createDefaultComponent(
			    DOMElement *parentElement, void *objGrandParent);

		void *createDefaultDescriptor(
			    DOMElement *parentElement, void *objGrandParent);

		void *posCompileSwitch(DOMElement *parentElement, void *parentObject);

	private:
		short convertComparator(string comparator);
  };
}
}
}
}
}

#endif

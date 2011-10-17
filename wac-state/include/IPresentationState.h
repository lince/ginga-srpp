/*
 * IPresentationState.h
 *
 *  Created on: Jul 23, 2010
 *      Author: caioviel
 */

#ifndef IPRESENTATIONSTATE_H_
#define IPRESENTATIONSTATE_H_

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "IElementaryState.h"

namespace br{
namespace ufscar{
namespace lince{
namespace ginga{
namespace wac{
namespace state{

class IPresentationState {
public:
	~IPresentationState() {};

	/**
	 * Retorna o nome de todos os players da apresentação atual.
	 * @return Instância de vector contendo todos os nomes dos players da apresentação atual.
	 */
	virtual vector<string>* getPlayersNames()=0;

	/**
	 * Retorna o estado de um determinado Player.
	 * @param name Nome do player o qual se deseja obter o estado.
	 * @return Instância de PlayerStateWac com o estado atual do player.
	 */
	virtual IElementaryState* getElementaryState(string name)=0;

	/**
	 * Retorna o Descritor associado ao nó de mídia de um player.
	 * @param Nome do Player relacionado ao nó de mídia.
	 * @return Nome do descritor associado ao nó de mídia.
	 */
	virtual string getMediaDescriptor(string name)=0;

	/**
	 * Retorna o nome completo da apresentação.
	 * @return String contendo o nome completo da apresentação;
	 */
	virtual string getPresentationName()=0;

	/**
	 * Retorna o nome do documento NCL.
	 * @return String contendo o nome do documento NCL.
	 */
	virtual string getDocumentName()=0;

	/**
	 * Retorna o nome da base privada.
	 * @return String contendo o nome da base privada.
	 */
	virtual string getPrivateBaseName()=0;

	virtual string toString()=0;

	virtual vector<string>* getContextPropertyNames()=0;

	virtual string getContextPropertyValue(string attributeId)=0;
};


}
}
}
}
}
}


#endif /* IPRESENTATIONSTATE_H_ */

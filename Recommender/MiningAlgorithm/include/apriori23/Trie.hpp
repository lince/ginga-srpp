/***************************************************************************
Trie.hpp  -  description
-------------------
begin                : cs dec 26 2002
copyright            : (C) 2002 by Ferenc Bodon
email                : bodon@mit.bme.hu
***************************************************************************/

/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

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
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

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
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/



#ifndef Trie_HPP
#define Trie_HPP


#ifdef HAVE_RECOMMENDER
#include "recommender/common.hpp"
#else
#include "common.hpp"
#endif


#include <vector>
#include <set>

using namespace std;

class Apriori_Trie;
class Trie;

/** Struct que representa um Trie

*/
struct Edge
{
	itemtype label;
	Trie* subtrie;
};

/**
A classe trie é uma abstração para uma estrutura de dados recursivas.
Cada nó raíz representa um itemset.
*/

class Trie
{
	friend class Apriori_Trie;

public:

	Trie( const unsigned long init_counter );

	/// Decide se o itemset será ou não incluído na árvore
	const Trie* is_included( const set<itemtype>& an_itemset,
			set<itemtype>::const_iterator item_it ) const;

	/// Incrementa o contator dos itemsets
	void find_candidate( vector<itemtype>::const_iterator it_basket_upper_bound,
			const itemtype distance_from_candidate,
			vector<itemtype>::const_iterator it_basket, 
			const unsigned long counter_incr=1);

	/// Apaga as arvores que contém os itens que ocorrem poucas vezes
	void delete_infrequent( const double min_occurrence,
			const itemtype distance_from_candidate );

	/// exibe a árvore em pré-ordem
	void show_content_preorder( ) const;
	~Trie();

private:

	/// adiciona um nó vazio na árvore
	void add_empty_state( const itemtype item,
			const unsigned long init_counter=0 );

public:
	// No public members
#ifdef HAVE_LOG4CXX
	LoggerPtr logger;
#endif


private:

	/// contator que armazena o número de nós na trie (Árvore)
	unsigned long counter;

	/** edgevector armazena os nós raízes das trie
	*
	* edgevector é organizado. Em linhas gerais é um vector de tries
	* que são estruturas de dados do tipo árvores
	*/
	vector<Edge> edgevector;

	/// armazena o tamanho do maior caminho até os nós folha da árvore (trie)
	/// inicia no nó raiz
	itemtype maxpath;
};


#endif

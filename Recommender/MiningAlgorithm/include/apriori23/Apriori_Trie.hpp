/***************************************************************************
Apriori_Trie.hpp  -  description
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

#ifndef Apriori_Trie_HPP
#define Apriori_Trie_HPP


#ifdef HAVE_RECOMMENDER
#include "recommender/Trie.hpp"
#include "recommender/Input_Output_Manager.hpp"
#else
#include "Trie.hpp"
#include "Input_Output_Manager.hpp"
#endif


#include <fstream>
#include <set>
#include <vector>
#include <cstdio>
using namespace std;


/** Apriori_Trie

Apriori_Trie cria a estrutura de  uma árvore concebida para fornecer métodos eficientes
para o algoritmo apriori.
*/
class Apriori_Trie
{
public:

	Apriori_Trie( const unsigned long counter_of_emptyset );

	/// Insere os itens mais frequentes
	void insert_frequent_items(const vector<unsigned long>& counters );

	/// Gera os canditados a regras
	void candidate_generation( const itemtype& frequent_size );

	/// Incrementa o contador dos canditados.
	void find_candidate( const vector<itemtype>& basket,
			const itemtype candidate_size, 
			const unsigned long counter=1 );

	/// Deleta os itemsets menos frequentes
	void delete_infrequent( const double min_occurrence,
			const itemtype candidate_size );

	/// Gera as regras de associação
	void association( const double min_conf,
			Input_Output_Manager& input_output_manager ) const;

	/// Retorna o tamanho do maior caminho até o nó folha da arvore (trie.hpp)
	itemtype longest_path() const;

	/// Escreve os itens mais frequentes no arquivo
	void write_content_to_file( Input_Output_Manager& input_output_manager ) const;

	/// apresenta o conteúdo da árvore gerada em pré-ordem
	void show_content_preorder( ) const;

	~Apriori_Trie();

protected:

	/// Decide se todos os nós na arvore são frequentes
	bool is_all_subset_frequent( const set<itemtype>& maybe_candidate ) const;

	/// Gera os candidatos com tamanho 2. Ex: Quem assiste A assiste B.
	void candidate_generation_two();

	/// Gera os candidatos com 2 ou mais. Ex: Quem assiste A assiste B,C,D.
	void candidate_generation_assist( Trie* Trie,
			const itemtype distance_from_generator,
			set<itemtype>& maybe_candidate );

	/// Incrementa o contador dos candidatos encontrados.
	void find_candidate_two( const vector<itemtype>& basket,
			const unsigned long counter=1 );

	/// Deleta os itens menos frequentes de tamanho 2.
	void delete_infrequent_two( const double min_occurrence );


	/// Encontra regras de associação.
	void assoc_rule_find( const double min_conf, set<itemtype>& condition_part,
			set<itemtype>& consequence_part,
			const unsigned long union_support,
			Input_Output_Manager& input_output_manager ) const;

	/// Geração parcial das regras de associação
	void assoc_rule_assist( const double min_conf, const Trie* Trie,
			set<itemtype>& consequence_part,
			Input_Output_Manager& input_output_manager ) const;

	/// Escreve as regras de associação encontradas no disco
	void write_content_to_file_assist( Input_Output_Manager& input_output_manager,
			const Trie* actual_state,
			const itemtype distance_from_frequent,
			set<itemtype>& frequent_itemset ) const;
private:
	// No private methods
#ifdef HAVE_LOG4CXX
	LoggerPtr logger;
#endif

public:
	// No public members

protected:
	/// Trie armazena os candidatos e os itens mais frequentes
	Trie main_trie;

	/**  temp_counter_array stores armazena as ocorrência de pares *///
	vector< vector<unsigned long> > temp_counter_array;
};

#endif

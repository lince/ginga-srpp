/***************************************************************************
apriori.h  -  description
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


#ifndef APRIORI_H
#define APRIORI_H

#ifdef HAVE_RECOMMENDER
#include "recommender/Apriori_Trie.hpp"
#else
#include "Apriori_Trie.hpp"
#endif


#include <map>



/** Essa classe implementa o algoritmo APRIORI.

O objetivo do algoritmo apriori é encontrar os conjuntos de dados frequentes L
no banco de dados. Itens com pouca frequencia devem ser deletados permanecendo
apenas aqueles que ocorrem mais vezes.
*/

class Apriori {
public:
       /**
        * Construtor.
        * @param basket_file arquivo com o conjunto de dados para mineração
        * @param output_file_name arquivo de saída com as regras de associação
        * @param store_input <b> true </b> indica para ler todo arquivo e armazenar as informações em memória
        *
        */
       Apriori( ifstream& basket_file, const char* output_file_name,
			const bool store_input );

	/// Esse método implementa o algoritmo Apriori
	void APRIORI_alg( const double min_supp, const double min_conf,
			const bool quiet, const unsigned long size_threshold );
	~Apriori();
private:


	void support( const itemtype& candidate_size );
#ifdef HAVE_LOG4CXX
	LoggerPtr logger;
#endif

protected:
	// No protected class data members

private:
	/// Abstração de uma árvore recursiva que armazena os item mais frequentes e portanto candidatos.
	Apriori_Trie*                           apriori_trie;
	/// Input_output_manager é responsável por gerenciar as operações de
	/// leitura e escrita.
	Input_Output_Manager                    input_output_manager;
	/// Armazenar as informações do banco de dados em memória, se o parâmetro store_input=true;
	map<vector<itemtype>, unsigned long>    reduced_baskets;
	/// se store_input = true, então as informações provinientes do
	/// arquivo de entrada serão armazenadas na memória
	bool                                    store_input;
};

#endif

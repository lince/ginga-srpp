/***************************************************************************
Input_Output_Manager.hpp  -  description
-------------------
begin                : cs march 10 2004
copyright            : (C) 2004 by Ferenc Bodon
email                : bodon@cs.bme.hu
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

#ifndef Input_Output_Manager_HPP
#define Input_Output_Manager_HPP

#ifdef HAVE_RECOMMENDER
#include "recommender/common.hpp"
#else
#include "common.hpp"
#endif


#include <fstream>
#include <vector>
#include <set>
using namespace std;


/** Essa classe é responsável pelas operações de entrada e saída (I/O Operations)
*/

class Input_Output_Manager:public ofstream
{
public:

	Input_Output_Manager( ifstream& basket_file,
			const char* output_file_name );

	/// Realizar a leitura de uma linha do arquivo de entrada (uma transação).
	int read_in_a_line( set<itemtype>& basket );

	/// Determina a frequencia de cada item.
	unsigned long find_frequent_items(
			const double min_supp, vector<unsigned long>& support_of_items );

	/// Criar um arquivo temporário com os itemsets mais frequentes
	void basket_recode( const set<itemtype>& original_basket,
			vector<itemtype>& new_basket );

	/// Escreve um itemset para um arquivo
	void write_out_basket( const set<itemtype>& itemset );

	/// Escreve o itemset e seu contador de frequencia para um arquivo
	void write_out_basket_and_counter( const set<itemtype>& itemset,
			const unsigned long counter );

	///
	void rewind();

	~Input_Output_Manager( );

private:
	/// Arquivo que contém as transações. Arquivo de entrada do algoritmo
	ifstream& basket_file;

	/** Vector que representa a frequência de cada item.
	*
	*/
	vector<itemtype> new_code;

	/** O inverso de  new_code vector.
	*/

	vector<itemtype> new_code_inverse;
#ifdef HAVE_LOG4CXX
	LoggerPtr logger;
#endif
};


#endif

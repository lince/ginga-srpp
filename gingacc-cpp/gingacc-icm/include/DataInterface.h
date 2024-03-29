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

#ifndef _DATA_INTERFACE_H_
#define _DATA_INTERFACE_H_

#include <string>
#include <vector>
#include <fstream>

#ifdef HAVE_RECOMMENDER
#include "recommender/utils.h"
#else
#include "../../../include/lince/utils/utils.h"
//#include "utils.h"  // <=--------- DEBUG!!!
//#include <RuleAdapter.h>  // <=--------- DEBUG!!!
#endif
using namespace ::br::ufscar::lince::ginga::recommender::util;

#ifdef HAVE_LOG4CXX
#include <log4cxx/logger.h>
using namespace log4cxx;
#endif

using namespace std;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace recommender {
namespace icm {

/** 
 * This class provides the data to be used in simulation of SI data broadcast.
 * The data to be "broadcasted" comprise the service provider name, service name (from SDT table), genre, and subgenre (from EIT table)
 * @author Marco Cristo & Angelo Bittar
 */
class TVData {

private:
	/**! Time in seconds since the begining of the simulation. */
	int seconds;

	/**! Name of the service proveder. */
	string providerName;

	/**! Name of the service. */
	string serviceName;

	/**! Service genre. */
	string genre;

	/**! Service sub-genre. */
	string subgenre;

public:
	/** 
	 * Starts TVData object state
	 */
	TVData(int sec, string pn, string sn, string g, string sg) {
		seconds = sec;
		providerName = pn;
		serviceName = sn;
		genre = g;
		subgenre = sg;
	}
	;

	/** 
	 * Returns program start time (in seconds, counting from the start of the simulation)
	 * @return int program start time (in seconds, counting from the start of the simulation)
	 */
	int getTimeStamp() {
		return seconds;
	}
	;

	/** 
	 * Returns provider name
	 * @return string provider name
	 */
	string getSProviderName() {
		return providerName;
	}
	;

	/** 
	 * Returns service name
	 * @return string service name
	 */
	string getServiceName() {
		return serviceName;
	}
	;

	/** 
	 * Returns service genre
	 * @return string service genre
	 */
	string getGenre() {
		return genre;
	}
	;

	/** 
	 * Returns service subgenre
	 * @return string service subgenre
	 */
	string getSubGenre() {
		return subgenre;
	}
	;
};

/** Class DataInterface
 * This class provides simulates the broadcast of TV meta-data to be used to infer implicit profiles.
 * In particular, this class simulates the broadcast of some data from SI tables.
 * The actual class should provide access to the SI data through the TS decoding services provided
 * by Ginga or by the Recommender API
 * Note that it also could provided data related to user interactions with the set-top-box, gathered by
 * the recommender
 * @author Marco Cristo & Angelo Bittar
 */
class DataInterface {

private:
	vector<TVData> *DataItens;
	time_t starttime;
	int startpoint;

public:
	/** 
	 * Load data schedule from a file where each line has the format:
	 * data1,data2,data3,...
	 * Each line represents a service.
	 * data1 is the offset time (in seconds) for the start of the service.
	 * Eg: 7,globo,corujao,filme,filme.
	 * Globo service "corujao" (genre: filme, subgenre: filme) will start 7 seconds after the beginning of the simulation.
	 */
	DataInterface();

	/** 
	 * Destroys data schedule from memory
	 */
	~DataInterface();

	/** 
	 * Returns object with data of interest, according with request time (current time) and data schedule (loaded previously)
	 * @return TVData* pointer to object with data of interest
	 */
	TVData *getData();
};

}
}
}
}
}
}

#endif /*_DATA_INTERFACE_H_*/

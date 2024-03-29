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

#ifndef _IMPLICIT_CONTEXT_MANAGER_H_
#define _IMPLICIT_CONTEXT_MANAGER_H_

#include <string>

#ifdef HAVE_RECOMMENDER
#include "recommender/utils.h"
#include "recommender/DataInterface.h"
#include "recommender/ImplicitProfiles.h"
#include <../../gingancl-cpp/include/adaptation/context/RuleAdapter.h>
#else
#include "../../../include/lince/utils/utils.h"
#include "../../../include/lince/icm/DataInterface.h"
#include "../../../include/lince/icm/ImplicitProfiles.h"
#include <../../../gingancl-cpp/include/adaptation/context/RuleAdapter.h>
//#include "utils.h"  // <=--------- DEBUG!!!
//#include <RuleAdapter.h>  // <=--------- DEBUG!!!
#endif
using namespace br::pucrio::telemidia::ginga::ncl::adaptation::context;
using namespace ::br::ufscar::lince::ginga::recommender::util;

#ifdef HAVE_LOG4CXX
#include <log4cxx/logger.h>
using namespace log4cxx;
#endif

#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>  /* define socket */
//#include <netinet/in.h>  /* define internet socket */
//#include <netdb.h>       /* define internet socket */
//#include <arpa/inet.h>
#include <sstream>

using namespace std;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace recommender {
namespace icm {

/** 
 * Wrapper for context manager that provides an interface for implicit users
 * @author Marco Cristo & Angelo Bittar
 */
class ImplicitContextManager {

private:
	/**! Pointer to presentation context. */
	PresentationContext *pctx;

	/**! Pointer to access interface to TV metadata */
	DataInterface *sii;

	/**! Pointer to implicit user profiles */
	ImplicitProfiles *iprofiles;

	/**! Current user id */
	int curImplicitUser; // current implicit user

	/**! Previous data observed by guessImplicitUser(). Corresponds to service name information. */
	string prevData;


	/**! List of profile candidates */
	vector<pair<vector<int> , int> > candidates;

	/**! Previous subgenre observed by guessImplicitUser(). */
	string prevSubGenre;

	/**! Previous timestamp observed by guessImplicitUser(). */
	int prevTimeStamp;

	/**! Previous user id observed by guessImplicitUser(). */
	int prevUserId;

	/**! Address of ImplicitContextManager instance. */
	static ImplicitContextManager* _instance;

	/**! Minimum probability to profile being considered similar to another profile. */
	static const float MIN_ACCEPTABLE_SIMILARITY = 0.3;

	/**! Minimum level of certainty to profile being considered new.  */
	static const float HIGH_LEVEL_OF_CERTAINTY = 0.95;

	/**! User id of default Ginga user */
	static const int DEFAULT_USER_ID = 0;

	//void notifyAboutNewImplicitUser();

	/**
	 *  Returns value of cosine between two vectors
	 *  @param v1 first vector.
	 *  @param v2 second vector.
	 *  @return float value of cossine.
	 */
	float cosine(vector<float> *v1, vector<float> *v2);

	/**
	 *  Gather stats.
	 *  @param si pointer to TV metadata.
	 */
	void gatherStats(TVData *si);

	/**
	 *  Compare base profiles with candidates.
	 *  @param bestc reference of best candidate.
	 *  @param bestp reference of best user and implicituser.
	 *  @param sim similarity value for best candidate.
	 */
	void compareProfiles(int &bestc, pair<int, int> &bestp, float &sim);

	/**
	 *  Update best profile.
	 *  @param bestc the best candidate.
	 *  @param bestp the best implicitUser.
	 */
	void updateBestProfile(int bestc, pair<int, int> bestp);

	/**
	 *  Select new profile.
	 *  @return int the selected profile.
	 */
	int selectNewProfile(void);

	/*
	 * Show vector contents - Debug purpose.
	 * @param *v vector to be shown.
	 * @return float vector in string format.
	 * */
	string showv(vector<float> *v) ;

	/*
	 * Show vector contents - Debug purpose.
	 * @param *v.
	 * @return string.
	 * */
	string showv(vector<int> *v) ;
	
	//Codigo de depuração - Ângelo Bitar
	bool newData;

public:
	/** 
	 * Get instance of (explicit) presentation context and corresponding context manager.
	 * Load users and profiles
	 * Set current implicit user profile.
	 * Clean up profile base
	 * Starts class DataInterface which simulates TV metadata broadcast (test purpose only -- actual DataInterface should only provide access to TV metadata).
	 */
	ImplicitContextManager();

	/** 
	 *  Destructor for implicit context manager
	 */
	~ImplicitContextManager();

	/** 
	 *  Check if PresentationContext is already instantiated
	 *  @return static bool value indicating if context instance has already been created.
	 */
	static bool canInstantiate();

	/** 
	 * Returns instance of implicit context manager
	 * @return static ImplicitContextManager* pointer to object implicit context manager
	 */
	static ImplicitContextManager* getInstance();

	/** 
	 * Returns current user id
	 * @return int current user id
	 */
	int getCurrentUserId();

	/** 
	 * Returns current implicit user id
	 * @return int current implicit user id
	 */
	int getCurrentImplicitUserId();

	/** 
	 * Returns current profile id for giver user id
	 * @param userid user id.
	 * @return int current profile id
	 */
	int getCurrentProfileId(int userid);

	/** 
	 * Returns true if current user id was set
	 * @param userid user id.
	 * @return bool value indicating if opeation was performed
	 */
	bool setCurrentUserId(int userid);

	/** 
	 * Returns true if current implicit user id was set
	 * @param userid user id.
	 * @return bool value indicating if opeation was performed
	 */
	bool setCurrentImplicitUserId(int userid);

	/** 
	 * Returns true if current profile id was set
	 * @param userid user id.
	 * @param profileid profile id.
	 * @return bool value indicating if opeation was performed
	 */
	bool setCurrentProfileId(int userid, int profid);

	/** 
	 * Returns a profile vector of a user
	 * @param userid user id
	 * @param profid profile id
	 * @return vector<int>* pointer for a feature vector which corresponds to the user profile
	 */
	vector<int>* getProfile(int userid, int profid);

	/** 
	 * Create a new implicit profile 
	 * @param uid user id
	 */
	void createProfile(int uid);

	/** 
	 *  Guess implicit user based on its profiles and additional evidence
	 */
	void guessImplicitUser();
	
	
	/**
	 * 
	 */
	bool remove(int userid);

	//Codigo de depuração - Ângelo Bitar
	string convInt2String(int number);
	string convFloat2String(float number);
	bool isNewData(){
		return newData;
	};
	void setNewData(bool data) {
		newData = data;
	};
	void candidatesClear(void);
};

}
}
}
}
}
}

#endif /*_IMPLICIT_CONTEXT_MANAGER_H_*/

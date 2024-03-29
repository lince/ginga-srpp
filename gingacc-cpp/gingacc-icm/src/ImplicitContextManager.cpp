/* Copyright (C) Marco Cristo & Angelo Filipe 2010
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of tprevUserIdhe License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_RECOMMENDER
#include "recommender/ImplictContextManager.h"
#else
#include "ImplicitContextManager.h"
#endif

//using namespace br::ufscar::lince::ginga::recommender::localagent;
using namespace br::ufscar::lince::ginga::recommender::icm;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace recommender {
namespace icm {

//Create a new Profile Candidate

ImplicitContextManager* ImplicitContextManager::_instance = NULL;

// check if PresentationContext is already instantiated
bool ImplicitContextManager::canInstantiate() {
	return PresentationContext::instantiated();
}
;

// get an implicit context instance
ImplicitContextManager* ImplicitContextManager::getInstance() {
	if (_instance == NULL && PresentationContext::instantiated()) {
		_instance = new ImplicitContextManager();
	}
	return _instance;
}

// constructor for ImplicitContext
ImplicitContextManager::ImplicitContextManager() {
	//Codigo de Depuracao Bitar
	newData = true;
	
	// cout << "+++ calling ImplicitContextManager()" << endl;

	// get instance of (explicit) presentation context
	pctx = PresentationContext::getInstance();

	// start profiles
	iprofiles = new ImplicitProfiles(pctx);

	// set current implicit user profile
	curImplicitUser = pctx->getContextManager()->getCurrentUserId();

	// load simulation data
	sii = new DataInterface();
	prevTimeStamp = time(NULL);
	prevUserId = getCurrentUserId();
	//prevProfileId = getCurrentProfileId(prevUserId);
	prevData = "";

	// notify about new user -- this will change in future (Raiza, Angelo e Marco)
	// notifyAboutNewImplicitUser();
}

// destructor for implicit context manager
ImplicitContextManager::~ImplicitContextManager() {
	delete iprofiles;
	delete sii;
}
;

// get profile vector
vector<int>* ImplicitContextManager::getProfile(int userid, int profid) {
	return iprofiles->get(userid, profid);
}
;

// Create a new implicit profile 
void ImplicitContextManager::createProfile(int uid) {
	// cout << "ImplicitContextManager::createProfile " << uid << endl;
	iprofiles->createProfile(uid);
};


// return current user id
int ImplicitContextManager::getCurrentUserId() {
	return pctx->getContextManager()->getCurrentUserId();
}

// return current implicit user id
int ImplicitContextManager::getCurrentImplicitUserId() {
	return curImplicitUser;
}

// get current profile id
int ImplicitContextManager::getCurrentProfileId(int userid) {
	return iprofiles->getCurrentId(userid);
}
;

// set current user id
bool ImplicitContextManager::setCurrentUserId(int userid) {
	pctx->getContextManager()->setCurrentUserId(userid);
	return true;
}
;

// set current implicit user id
bool ImplicitContextManager::setCurrentImplicitUserId(int userid) {
	curImplicitUser = userid; // current implicit user
	return true;
}
;

// set current profile of a given user
bool ImplicitContextManager::setCurrentProfileId(int userid, int profid) {
	return iprofiles->setCurrentId(userid, profid);
}
;

bool ImplicitContextManager::remove(int userid) {
	return iprofiles->remove(userid, 0);
};

/*
// notify registered listeners about new implicit user
void ImplicitContextManager::notifyAboutNewImplicitUser() {
	// by now, only send SOCKET message to Raiza :)))
	const int SERVER_PORT = 4444; // define a server port number 
	int sd;
	struct sockaddr_in server_addr = { AF_INET, htons(SERVER_PORT) };
	char buf[1] = "";

	// create a socket 
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cout << "ImplicitContextManager::notifyAboutNewImplicitUser: client socket failed" << endl;
		return;
	}

	// connect a socket 
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	if (connect(sd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
		cout << "ImplicitContextManager::notifyAboutNewImplicitUser: connect to server failed" << endl;
		return;
	}

	// send message to server 
	cout << "ImplicitContextManager::notifyAboutNewImplicitUser: User has changed or TV on." << flush << endl;
	send(sd, buf, sizeof(buf), 0);

	close(sd);
}
;
*/

// cosine between two vectors
float ImplicitContextManager::cosine(vector<float> *v1, vector<float> *v2) {
	float dot, sqv1, sqv2;
	dot = sqv1 = sqv2 = 0.0;
	for (int i = 0; i < v1->size(); i++) {
		dot += (*v1)[i] * (*v2)[i];
//		cout << "*** " << (*v1)[i] << " * " << (*v2)[i] << " = " << dot << endl;
		sqv1 += (*v1)[i] * (*v1)[i];
		sqv2 += (*v2)[i] * (*v2)[i];
	};
//	cout << dot << " " << sqv1 << " " << sqv2 << endl;
	if (dot == 0)
		return 0.0;
	return dot / (sqrt(sqv1) * sqrt(sqv2));
}
;

// coleta estats de programacao
void ImplicitContextManager::gatherStats(TVData *si) {
	//Codigo de depuração - trocar o stable time para 1
	const int stableTime = 6;
	// update old candidates
	int now = time(NULL);
	int elapsedTime = now - prevTimeStamp;
	for (int i = 0; i < candidates.size(); i ++) {
		int t = elapsedTime > stableTime? elapsedTime - stableTime: elapsedTime;
		candidates[i].first[iprofiles->getAxis(prevSubGenre)] += t;
		candidates[i].second += t;
		// cout << showv(&candidates[i].first) << " " << candidates[i].second << endl;
	};

	// get subgenre
	string subGenre = si->getSubGenre();
	if (subGenre != prevSubGenre) {
		// create new candidate
		prevSubGenre = subGenre;
		// se cara se logou agora
		// start_point = candidates;
		vector<int> countSgen;
		countSgen.assign(iprofiles->getSize(), 0);
		countSgen[iprofiles->getAxis(subGenre)] += stableTime;
		candidates.push_back(make_pair(countSgen, stableTime));
		for (int i = 0; i < candidates.size() - 1; i ++) {
			candidates[i].first[iprofiles->getAxis(subGenre)] += stableTime;
			candidates[i].second += stableTime;
		}
		// cout << showv(&candidates[candidates.size() - 1].first)  << " " << candidates[candidates.size() - 1].second << endl;
	}
	
	//Codigo de depuração - Ângelo Bitar
		iprofiles->setValues(0, "debug.candidate", convInt2String(candidates.size()));
	
	// cout << "+++ ImplicitContextManager::gatherStats #cands =" << candidates.size() << endl;

	// save timestamp
	prevTimeStamp = now;
};
string ImplicitContextManager::showv(vector<float> *v) {
	ostringstream ss;
	ss << " <";
	for (int i = 0; i < v->size(); i++)
		ss << (*v)[i] << " ";
	ss << ">";
	return ss.str();
};
string ImplicitContextManager::showv(vector<int> *v) {
	ostringstream ss;
	ss << " <";
	for (int i = 0; i < v->size(); i++)
		ss << (*v)[i] << " ";
	ss << ">";
	return ss.str();
};
//Codigo de depuração - Ângelo Bitar
string ImplicitContextManager::convInt2String(int number){
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
};
//Codigo de depuração - Ângelo Bitar
string ImplicitContextManager::convFloat2String(float number){
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
};
//Codigo de depuração - Ângelo Bitar
void ImplicitContextManager::candidatesClear(void){
	for (int i = 0; i < candidates.size(); i ++) {
		stringstream ss;
		ss << "debug.cprof" << i;
		iprofiles->delVars(0, ss.str());
	}
	
	candidates.clear();
	iprofiles->setValues(0, "debug.candidate", "0");		
};


void ImplicitContextManager::compareProfiles(int &bestc, pair<int, int> &bestp, float &max) {
	int i, j;
	//Codigo de depuração - Ângelo Bitar
	//vector <pair<similarityGrade, pair<pair<userID, profID>, candidateID > >
	vector<pair<float, pair<pair<int, int>, int> > > bestGrade;

	// normalize base profiles
	vector<pair<vector<float>, pair<int, int> > > bprofiles;
	tIProfiles *ip = iprofiles->get();
	tIProfiles::iterator it = ip->begin();
	while (it != ip->end()) {
		tIProfileMap* uprofs = it->second;
		tIProfileMap::iterator itu = uprofs->begin();
		while (itu != uprofs->end()) {
			vector<int> *v = itu->second.second;
			int tot = 0;
			for (j = 0; j < v->size(); j++) {
				tot += (*v)[j];
			}
			vector<float> pv;
			for (j = 0; j < v->size(); j++) {
				pv.push_back(tot == 0? 0.0: ((float) (*v)[j]) / tot);
			}
   			bprofiles.push_back(make_pair(pv, make_pair(it->first, itu->first)));
   			//Codigo de depuração - Ângelo Bitar
   			bestGrade.push_back(make_pair(0, make_pair(make_pair(it->first, itu->first), 0)));
			itu++;
		}
		it++;
	}

	// normalize candidates
	//Codigo de depuração - Ângelo Bitar (descomentar a linha de baixo)
	vector<pair<vector<float>, int> > ncandidates;
	//vector<vector<float> > ncandidates;
	for (i = 0; i < candidates.size(); i ++) {
		//Codigo de depuração - Ângelo Bitar
		stringstream sc;
		vector<float> pv;
		vector<int> &cv = candidates[i].first;
		for (j = 0; j < cv.size(); j++){
			pv.push_back(candidates[i].second == 0? 0.0: ((float) cv[j]) / candidates[i].second);
			//Codigo de depuração - Ângelo Bitar
			if (j != 0){
				sc << ",";
			}
			sc << cv[j];
		}
		//Codigo de depuração - Ângelo Bitar (descomentar a linha de baixo)
		stringstream ss;
		ss << "debug.cprof" << i;
		iprofiles->setValues(0, ss.str(), sc.str());
		ncandidates.push_back(make_pair(pv,i));	
		//ncandidates.push_back(pv);
	}
	
	// comparison
	float cos;
	max = 0;
	bestc = 0;
	bestp = make_pair(0, 0);
	for (i = 0; i < ncandidates.size(); i ++)
		for (j = 0; j < bprofiles.size(); j ++) {
			//Codigo de depuração - Ângelo Bitar (descomentar a linha de baixo)
			cos =  cosine(&ncandidates[i].first, &bprofiles[j].first);
			//cos =  cosine(&ncandidates[i], &bprofiles[j].first);
			// cout << "c" << i << " x p(" << bprofiles[j].second.first << "," << bprofiles[j].second.second << ") = " << cos << "" << showv(&ncandidates[i]) << " " << showv(&bprofiles[j].first) << endl;
			//Codigo de depuração - Ângelo Bitar
			if (cos > bestGrade[j].first){ 
				bestGrade[j].first = cos;
				bestGrade[j].second.second = ncandidates[i].second;
			}
			if (cos > max) {
				max = cos;
				bestc = i;
				bestp = bprofiles[j].second;
			}
		}
	//Codigo de depuração - Ângelo Bitar
	for (j = 0; j < bprofiles.size(); j ++) {
		stringstream ss;
		ss << "debug.prob" << bestGrade[j].second.first.second;
		stringstream si;
		si << bestGrade[j].first << ":" << bestGrade[j].second.second;
		iprofiles->setValues(bestGrade[j].second.first.first, ss.str(), si.str());
	}	
}

// Update Best Profile
void ImplicitContextManager::updateBestProfile(int bestc, pair <int, int> bestp )
{
	vector<int> *v = iprofiles->get(bestp.first, bestp.second);
	for (int i = 0; i < v->size(); i ++)
		(*v)[i] += candidates[bestc].first[i];
	iprofiles->modify(bestp.first, bestp.second);
}

// Select new profile
int ImplicitContextManager::selectNewProfile(void)
{
	// apply some critery to candidates to select a profile as a new profile
	// in this version, we only returns the oldest candidate (index 0)
	return 0;
}

// guess implicit user based on its profiles and additional evidence
void ImplicitContextManager::guessImplicitUser() {
	TVData *si = sii->getData();

	/*
	if (prevData == "") {
		int uid = getCurrentUserId();
		int pid = getCurrentProfileId(uid);
		vector<int> *cv = iprofiles->get(uid, pid);
		if (cv != NULL) {
			vector<int> *pfs = new vector<int> (*cv);
			for (int j = 0; j < pfs->size(); j++)
				(*pfs)[j] += j;
			iprofiles->insert(uid, pfs, pid);
			iprofiles->show();
		}
	}
	*/

	// if user has logged on, candidates are eliminated and flags are reseted
	//cout << "ImplicitContextManager::guessImplicitUser: before get" << endl;
	int userid = getCurrentUserId();
	//cout << "ImplicitContextManager::guessImplicitUser: AFTER get" << endl;
	if (userid != prevUserId ) {
		setNewData(true);
		prevUserId = userid;
		if (userid != DEFAULT_USER_ID) {
			//candidates.clear();
			//Codigo de depuração - Ângelo Bitar (Descomentar a linha de cima)
			candidatesClear();
		//cout << "ImplicitContextManager::guessImplicitUser: before setImplict" << endl;
			setCurrentImplicitUserId(userid);
			cout << "ImplicitContextManager::guessImplicitUser: 1) setCurrentProfileId(" << userid << ", 0)." << endl;
			setCurrentProfileId(userid, 0);
			if (si != NULL) {
				prevData = si->getServiceName();
				prevSubGenre = si->getSubGenre();
			};
			prevTimeStamp = time(NULL);
			return;
		}		
	}

	if (si != NULL) {
		string curData = si->getServiceName();
		//cout << "ImplicitContextManager::guessImplicitUser: before setValues" << endl;
		//Codigo de depuração - Ângelo Bitar
		iprofiles->setValues(0, "debug.channel", si->getSProviderName());
		iprofiles->setValues(0, "debug.service", si->getServiceName());
		iprofiles->setValues(0, "debug.genre", si->getGenre());
		iprofiles->setValues(0, "debug.subgenre", si->getSubGenre());
		
		if (curData != prevData) {
			setNewData(true);
			cout << "ImplicitContextManager::guessImplicitUser: get TV metadata = "
					// << si->getTimeStamp() << ", " 
					<< si->getServiceName()
					<< ", " << si->getSProviderName() << ", " << si->getGenre()
					<< ", " << si->getSubGenre() << endl;

			// gather stats
			//cout << "ImplicitContextManager::guessImplicitUser: before gahterStats" << endl;
			gatherStats(si);
			//cout << "ImplicitContextManager::guessImplicitUser: AFTER gahterStats" << endl;
			// compare profiles
			int bestc;
			pair<int, int> bestp;
			float bestsim;
			
			if (candidates.size() == 1 &&
				userid != DEFAULT_USER_ID) {
				//Codigo de depuração - Ângelo Bitar
				compareProfiles(bestc, bestp, bestsim);
				// if user has just logged in (only 1 candidate and id != 0)
				bestc = 0;
				bestp = make_pair(userid, 0);
				bestsim = 1.0;
				
			} else
				compareProfiles(bestc, bestp, bestsim);
			if (bestsim > MIN_ACCEPTABLE_SIMILARITY) {
				// profile identified
				// update profile
				updateBestProfile(bestc, bestp);
				cout << "ImplicitContextManager::guessImplicitUser: Profile candidate " << bestc << " was identified as user " << bestp.first << ", profile " << bestp.second << "." << endl;
				// set current implicit user and profile
				setCurrentImplicitUserId(bestp.first);
				cout << "ImplicitContextManager::guessImplicitUser: 2) setCurrentProfileId(" << bestp.first << ", " << bestp.second << ")." << endl;
				setCurrentProfileId(bestp.first, bestp.second);
				// set 'bestp.first' as explicit user id since we are sure she/he is logged on
				// if (bestsim > HIGH_LEVEL_OF_CERTAINTY) {
				//	setCurrentUserId(bestp.first);
				// we have to decide what to do with candidates!
				//}

			} else {
				// profile not identified
				int profid;
				// select one out of the candidates to be inserted into the profile database
				int selectedCandidate = selectNewProfile();
				// insert the candidate
			    /* iprofiles->insert(DEFAULT_USER_ID, &candidates[selectedCandidate].first, profid);
				// set current implicit user and profile
				setCurrentImplicitUserId(DEFAULT_USER_ID);
				setCurrentProfileId(DEFAULT_USER_ID, profid); */
			}
			
			prevData = curData;
		}
	}
}



}
}
}
}
}
}

/*
int main(void) {
	ImplicitContextManager *icm = new ImplicitContextManager();

	while (1) {
		icm->guessImplicitUser();
		usleep(1000000); // 1000000 = 1 seg
	}
	delete icm;
}
;*/


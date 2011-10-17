#ifndef _LOCAL_AGENT_H_
#define _LOCAL_AGENT_H_

#include <ginga/recommender/agentlistener/AgentListener.h>
#include <ginga/recommender/database/Database.h>

#include "system/io/interface/input/CodeMap.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

#include "tuner/ITuner.h"
#include "tuner/providers/IChannel.h"
using namespace ::br::pucrio::telemidia::ginga::core::tuning;

#include "thread/Thread.h"
using namespace ::br::ufscar::lince::ginga::recommender;

#include "recommender/utils/Utils.h"
using namespace ::br::ufscar::lince::ginga::recommender::util;

#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;

#include "wac/state/IStateManager.h"
#include "wac/state/IPresentationState.h"
using namespace ::br::ufscar::lince::ginga::wac::state;

#include <ginga/player/PlayerState.h>
using namespace ::br::ufscar::lince::ginga::core::player;

#include "Program.h"
#include "Volume.h"
#include "Channel.h"
#include "Iteraction.h"
#include "Key.h"
#include "Meta.h"
#include "NclStateMachine.h"
#include "Document.h"
#include "Context.h"
#include "Media.h"
#include "User.h"

//------------Iza inseriu:-----------------

/*#include "Device.h"
#include "DeviceResource.h"
#include "Information.h"
#include "IteractionBase.h"
#include "Resource.h"
#include "SocialNetwork.h"
#include "UserSN.h"
#include "WatchTV.h"
#include "WatchTVUsers.h"*/

//----------------Fim Iza------------------

#include <vector>
#include <map>
#include <sstream>
#include <set>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                    class LocalAgent: public Thread {
                    public:

						/**
	                     * Construtor vazio
		                 */
                        LocalAgent();

                        /**
                         * Destrutor.
                         */
                        ~LocalAgent();
                        
						/**
	                     * Retorna a instancia única da classe
		                 */
                        static LocalAgent* getInstance();

                        /**
                         * Retorna o banco de dados onde são armazendas as informações coletadas.
                         * @return banco de dados onde são armazendas as informações coletadas.
                         */
                        Database* getDatabase();

                        /**
                         * Pára a execução do LocalAgent.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool stop();

                        /**
                         * Adiciona um AgentListener ao LocalAgent.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool addAgentListener(AgentListener* listener);

                        /**
                         * Remove um AgentListener do LocalAgent.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool removeAgentListener(AgentListener* listener);
                        
                        void run();
                        
                        bool getEstado();
                        
                        void setEstado(bool estado);
                        
                        string getValue(string varAmbiente);
                        
                        void setPressedKey(int pressedKey);
                        
                        int getPressedKey();
                        
                        bool addIteraction(Iteraction* iteraction);
                        
                        bool removeIteraction(Iteraction* iteraction);
                        
                        string getKeyName(int keyCode);
                        
                        void setTuner(ITuner* tuner);

			void setPrimaryKey();

			void setNewUser(string ID, string userGenre, string locationZip, string age);
						
                    private:
                        vector<AgentListener*>* listeners;
                        vector<Iteraction*> iters;
                        Database* db;
                        ITuner* tuner;
                        int pressedKey;
                        bool estado; // "true": LocalAgent ativado | "false": LocalAgent desativado
                        static LocalAgent* _instance;
                        IComponentManager* cm;
                        IStateManager* stateManager;
                        IPresentationState* ps;
                        Document* document;
                        NclStateMachine* nclStateMachine;
                        Context* context;
                        Media* media;
			User* user;
                    };
                }
            }
        }
    }
}

#endif


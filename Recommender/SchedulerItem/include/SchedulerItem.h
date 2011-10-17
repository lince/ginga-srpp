#ifndef _SCHEDULER_ITEM_H_
#define _SCHEDULER_ITEM_H_

#include <vector>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                        class SchedulerItem {
                        public:

                            /**
                             * Inicializa a aplicação com os parâmetros especificados.
                             * @param params parâmetros para a aplicação.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool init(vector<char*>* params) = 0;

                            /**
                             * Coloca a aplicação em execução.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool start() = 0;

                            /**
                             * Pára a aplicação.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool stop() = 0;

                            /**
                             * Coloca a aplicação em modo pause.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool pause() = 0;

                        };

                }
            }
        }
    }
}

#endif


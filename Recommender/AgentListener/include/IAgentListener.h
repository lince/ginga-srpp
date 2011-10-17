#include "AgentListener.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                	class IAgentListener : public AgentListener {
                    public:

                        /**
                         * Método acionado quando o volume é incrementado.
                         */
                        virtual void volUp() = 0;

                        /**
                         * Método acionado quando o volume é decrementado.
                         */
                        virtual void volDown() = 0;

                        /**
                         * Método acionado quando o canal é incrementado.
                         */
                        virtual void channelUp() = 0;

                        /**
                         * Método acionado quando o canal é decrementado.
                         */
                        virtual void channelDown() = 0;

                        /**
                         * Método acionado quando ocorre uma mudança de canal.
                         * @param info informações do canal anterior e do novo canal sintonizado.
                         */
                        virtual void channelChange(const char* info) = 0;

                        /**
                         * Método acionado quando ocorre uma interação com uma aplicação NCL.
                         * @param info informações da interação realizada.
                         */
                        virtual void nclChange(const char* info) = 0;
                	}
                }
			}
		}
	}
}

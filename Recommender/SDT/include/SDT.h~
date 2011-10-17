#ifndef _SDT_H_
#define _SDT_H_

#include <string>
#include <stdio.h>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    /**
                     * Criar um abstração para a tabela sdt responsável em informar o nome
                     * do provedor de serviços e o serviço associado
                     *
                     * @author Paulo Muniz de Ávila
                     */
                    class sdt {
                    private:
                        char serviceProviderName[128];
                        char serviceName[128];

                    public:

                        /**
                         * Recuperar o nome do serviço, no sistema analógico o programa
                         * @return char * com o nome do serviço
                         */
                        char* getServiceName();


                        /**
                         * Recuperar o nome do provedor de serviços, no sistema analógico
                         * a emissora
                         * @return char * com o nome do provedor de serviços
                         */
                        char* getServiceProviderName();


                        /**
                         * Armazenar no objeto da classe Sdt o nome do serviço
                         * @param serviceName char * com o nome do serviço
                         */
                        void setServiceName(const char *serviceName);

                        /**
                         * Armanezar no objeto da classe Sdt o provedor de Serviço
                         * @param serviceProviderName char * com o nome do provedor de serviço
                         */
                        void setServiceProviderName(const char *serviceProviderName);

                    };
                }
            }
        }
    }
}

#endif


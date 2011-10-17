#include "../include/SDT.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                    /**
                     * Recuperar o nome do serviço, no sistema analógico o programa
                     * @return char * com o nome do serviço
                     */
                    char* sdt::getServiceName() {
                        return serviceName;
                    }

                    /**
                     * Recuperar o nome do provedor de serviços, no sistema analógico
                     * a emissora
                     * @return char * com o nome do provedor de serviços
                     */
                    char* sdt::getServiceProviderName() {
                        return serviceProviderName;
                    }

                    /**
                     * Armazenar no objeto da classe Sdt o nome do serviço
                     * @param serviceName char * com o nome do serviço
                     */
                    void sdt::setServiceName(const char *serviceName) {
                        strcpy(this->serviceName, serviceName);
                        strcat(this->serviceName, "\0");
                    }

                    /**
                     * Armanezar no objeto da classe Sdt o provedor de Serviço
                     * @param serviceProviderName char * com o nome do provedor de serviço
                     */
                    void sdt::setServiceProviderName(const char *serviceProviderName) {
                        strcpy(this->serviceProviderName, serviceProviderName);
                        strcat(this->serviceProviderName, "\0");
                    }
                }
            }
        }
    }
}



#include "../include/EIT.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
                    /**
                     * Recuperar a informação com o gênero do serviço
                     * Ver Norma NBR 15603-2 para maiores informações
                     * @return um char * com o gênero do serviço
                     */
                    char* eit::getDescriptor_conteudo_genero() {
                        return descriptor_conteudo_genero;
                    }

                    /**
                     * Recuperar as informações com o sub-gênero do serviço
                     * Ver Norma NBR 15603-2 para maiores informações
                     * @return um char * com o sub-gênero do serviço
                     */
                    char* eit::getDescriptor_conteudo_subgenero() {
                        return descriptor_conteudo_subgenero;
                    }

                    /**
                     * Recuperar as informações de sinopse do serviço
                     * @return Um ponteiro de 128 bytes com a sinopse
                     */
                    char* eit::getDescriptor_event_short_sinopse() {
                        return descriptor_event_short_sinopse;
                    }

                    /**
                     * Recuperar o horário de término do programa
                     * @return char * com o horário informado pelo provedor de serviço
                     */
                    char* eit::getEnd_time() {
                        return end_time;
                    }

                    /**
                     * Recuperar o horário de início do programa
                     * @return char * com o horário informado pelo provedor de serviço
                     */
                    char* eit::getStart_time() {
                        return start_time;
                    }

                    /**
                     * Armazenar o gênero no objeto da classe Eit
                     * @param descriptor_conteudo_genero char * com o código do gênero
                     */
                    void eit::setDescriptor_conteudo_genero(const char *descriptor_conteudo_genero) {
                        strcpy(this->descriptor_conteudo_genero, descriptor_conteudo_genero);
                        strcat(this->descriptor_conteudo_genero, "\0");
                    }

                    /**
                     * Armazenar o sub-gênero no objeto da classe Eit
                     * @param descriptor_conteudo_subgenero char * com o código do gênero
                     */
                    void eit::setDescriptor_conteudo_subgenero(const char *descriptor_conteudo_subgenero) {
                        strcpy(this->descriptor_conteudo_subgenero, descriptor_conteudo_subgenero);
                        strcat(this->descriptor_conteudo_subgenero, "\0");
                    }

                    /**
                     * Armazenar uma sinopse no objeto da classe Eit
                     * @param descriptor_event_short_sinopse um char * com 128 bytes descrevendo a sinopse
                     */
                    void eit::setDescriptor_event_short_sinopse(const char *descriptor_event_short_sinopse) {
                        strcpy(this->descriptor_event_short_sinopse, descriptor_event_short_sinopse);
                        strcat(this->descriptor_event_short_sinopse, "\0");
                    }

                    /**
                     * Armazenar o horário de término do serviço no objeto da classe Eit
                     * @param end_time um char * com o término do serviço
                     */
                    void eit::setEnd_time(const char *end_time) {
                        strcpy(this->end_time, end_time);
                        strcat(this->end_time, "\0");
                    }

                    /**
                     * Armazenar o horário de início do serviço no objeto da classe Eit
                     * @param start_time um char * com  o horário de início do programa
                     */
                    void eit::setStart_time(const char *start_time) {
                        strcpy(this->start_time, start_time);
                        strcat(this->start_time, "\0");
                    }
                }
            }
        }

    }
}


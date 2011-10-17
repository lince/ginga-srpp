#ifndef _EIT_H_
#define _EIT_H_

#include <string>
#include <stdio.h>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    /**
                     * Fornece uma abstração para a tabela Eit. A tabela Eit
                     * é responsável em prover informações sobre os serviços
                     * disponibilizados pelos provedores de serviços
                     *
                     * @author Paulo Muniz de Ávila
                     */

                    class eit {
                    private:
                        /// inicio do programa - máxmo de 12 bytes
                        char start_time[24];
                        /// término do programa - máximo de 12 bytes
                        char end_time[24];
                        /// gênero do programa - máximo de 4 bytes
                        char descriptor_conteudo_genero[5];
                        /// sub-gênero do programa - máximo de 4 bytes
                        char descriptor_conteudo_subgenero[5];
                        /// sinopse do programa máximo de 128 bytes
                        char descriptor_event_short_sinopse[129];


                        /*
                         * criar o epg para ser gravado no disco:
                         * SELECT n.*,c.classgen,c.classubgen,c.descritor FROM ibope.new_epg n, ibope.categ
                         *  orias c where c.nome=n.nomePrograma
                         *
                         */

                    public:
                        /**
                         * Recuperar a informação com o gênero do serviço
                         * Ver Norma NBR 15603-2 para maiores informações
                         * @return um char * com o gênero do serviço
                         */
                        char* getDescriptor_conteudo_genero();


                        /**
                         * Recuperar as informações com o sub-gênero do serviço
                         * Ver Norma NBR 15603-2 para maiores informações
                         * @return um char * com o sub-gênero do serviço
                         */
                        char* getDescriptor_conteudo_subgenero();


                        /**
                         * Recuperar as informações de sinopse do serviço
                         * @return Um ponteiro de 128 bytes com a sinopse
                         */
                        char* getDescriptor_event_short_sinopse();


                        /**
                         * Recuperar o horário de término do programa
                         * @return char * com o horário informado pelo provedor de serviço
                         */
                        char* getEnd_time();


                        /**
                         * Recuperar o horário de início do programa
                         * @return char * com o horário informado pelo provedor de serviço
                         */
                        char* getStart_time();


                        /**
                         * Armazenar o gênero no objeto da classe Eit
                         * @param descriptor_conteudo_genero char * com o código do gênero
                         */
                        void setDescriptor_conteudo_genero(const char *descriptor_conteudo_genero);


                        /**
                         * Armazenar o sub-gênero no objeto da classe Eit
                         * @param descriptor_conteudo_subgenero char * com o código do gênero
                         */
                        void setDescriptor_conteudo_subgenero(const char *descriptor_conteudo_subgenero);


                        /**
                         * Armazenar uma sinopse no objeto da classe Eit
                         * @param descriptor_event_short_sinopse um char * com 128 bytes descrevendo a sinopse
                         */
                        void setDescriptor_event_short_sinopse(const char *descriptor_event_short_sinopse);


                        /**
                         * Armazenar o horário de término do serviço no objeto da classe Eit
                         * @param end_time um char * com o término do serviço
                         */
                        void setEnd_time(const char *end_time);


                        /**
                         * Armazenar o horário de início do serviço no objeto da classe Eit
                         * @param start_time um char * com  o horário de início do programa
                         */
                        void setStart_time(const char *start_time);


                    };
                }
            }
        }
    }
}

#endif


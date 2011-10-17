#ifndef _MININGALGORITHM_H_
#define _MININGALGORITHM_H_

#include <vector>

#include <ginga/recommender/database/Database.h>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                        class MiningAlgorithm {
                        public:

                            /**
                             * Inicializa o algoritmo com os parâmetros especificados.
                             * @param params parâmetros para o algoritmo.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool setParams(vector<char*> params) = 0;

                            /**
                             * Prepara os dados para serem processados pelo algoritmo.
                             * @param data dados a serem pre-processados.
                             * @param delimiter delimitador dos dados.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool prepare(const char* data, const char* delimiter) = 0;

                            /**
                             * Prepara os dados para serem processados pelo algoritmo.
                             * @param db banco de dados que contém a tabela a ser pre-processada.
                             * @param table tabela com os dados a serem pre-processados.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool prepare(Database* db, const char* table) = 0;

                            /**
                             * Executa o algoritmo.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool start() = 0;

                            /**
                             * Pára a execução do algoritmo.
                             * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool stop() = 0;

                            /**
                             * Define a saída dos dados processados.
                             * @param data variável para armazenamento dos dados processados.
                             * @param size tamanho (em bytes) da variável para armazenamento dos dados.
                             * @param delimiter delimitador dos dados.
                             */
                            virtual void setOutput(char* data, long size, const char* delimiter) = 0;

                            /**
                             * Define a saída dos dados processados.
                             * @param db banco de dados que contém a tabela onde serão armazenados os dados processados.
                             * @param table tabela onde serão armazenados os dados processados.
                             */
                            virtual void setOutput(Database* db, const char* table) = 0;

                            /**
                             * Define a saída dos dados processados.
                             * @param file caminho do arquivo para armazenamento dos dados processados.
                             */
                            virtual void setOutput(const char* file) = 0;
                        };

                }
            }
        }
    }
}

#endif


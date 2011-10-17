#ifndef _DATAMINING_H_
#define _DATAMINING_H_

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <ginga/recommender/miningalgorithm/MiningAlgorithm.h>
#include <ginga/recommender/miningalgorithm/IMiningAlgorithmApriori.h>
#include <ginga/recommender/database/Database.h>

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    class DataMining {
                    public:

                        /**
                         * Construtor.
                         * Devolve uma instância de DataMining.
                         */
                        DataMining();

                        /**
                         * Construtor.
                         * Devolve uma instância de DataMining.
                         * @param algorithm algoritmo que será usado para o processamento dos dados.
                         */
                        DataMining(const char* algorithm);

                        /**
                         * Construtor.
                         * Devolve uma instância de DataMining.
                         * @param lib nome da biblioteca que contém o algoritmo a ser executado.
                         * @param constructor construtor do algoritmo que devolve uma instância para a interface MiningAlgorithm.
                         * @param destructor destrutor do algoritmo.
                         */
                        DataMining(const char* lib, const char* constructor, const char* destructor);

                        /**
                         * Destrutor.
                         */
                        virtual ~DataMining();

                        /**
                         * Define o algoritmo que será usado para o processamento dos dados.
                         * @param algorithm algoritmo que será usado para o processamento dos dados.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool setAlgorithm(const char* algorithm);

                        /**
                         * Retorna o algoritmo que será usado para o processamento dos dados.
                         * @return algoritmo que será usado para o processamento dos dados.
                         */
                        const char* getAlgorithm();

                        /**
                         * Retorna todos os algoritmos disponíveis para processamento dos dados.
                         * @return todos os algoritmos disponíveis para processamento dos dados.
                         */
                        vector<char*>* getAlgorithms();

                        /**
                         * Inicializa o algoritmo com os parâmetros especificados.
                         * @param params parâmetros para o algoritmo.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool setParams(vector<char*> params);

                        /**
                         * Prepara os dados para serem processados pelo algoritmo.
                         * @param data dados a serem pre-processados.
                         * @param delimiter delimitador dos dados.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool prepare(const char* data, const char* delimiter);

                        /**
                         * Prepara os dados para serem processados pelo algoritmo.
                         * @param db banco de dados que contém a tabela a ser pre-processada.
                         * @param table tabela com os dados a serem pre-processados.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool prepare(Database* db, const char* table);

                        /**
                         * Executa o algoritmo.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool start();

                        /**
                         * Pára a execução do algoritmo.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool stop();

                        /**
                         * Define a saída dos dados processados.
                         * @param data variável para armazenamento dos dados processados.
                         * @param size tamanho (em bytes) da variável para armazenamento dos dados.
                         * @param delimiter delimitador dos dados.
                         */
                        void setOutput(char* data, long size, const char* delimiter);

                        /**
                         * Define a saída dos dados processados.
                         * @param db banco de dados que contém a tabela onde serão armazenados os dados processados.
                         * @param table tabela onde serão armazenados os dados processados.
                         */
                        void setOutput(Database* db, const char* table);

                        /**
                         * Define a saída dos dados processados.
                         * @param file caminho do arquivo para armazenamento dos dados processados.
                         */
                        void setOutput(const char* file);

                    private:
                        /**
                         * Inicializa o mapa com os nomes dos algoritmos de mineração de dados disponíveis.
                         */
                        void setAvaibleMiningAlgorithms();

                        enum MiningAlgoritmKeys {
                            Apriori = 1
                        };

                        map<char*, int> avaibleMiningAlgorithms;
                        string currentAlgorithm;

                    public:
                        MiningAlgorithm* miningAlgorithm;

                    private:
                        void* (*algorithmConstructor)(void);
                        void* algorithmPointer;
                        void (*algorithmDestructor)(void*);
                    };

                }
            }
        }
    }
}

#endif


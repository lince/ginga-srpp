#include <../include/DataMining.h>

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    /**
                     * Construtor.
                     * Devolve uma instância de DataMining.
                     */
                    DataMining::DataMining() {
                        miningAlgorithm = NULL;
                        algorithmConstructor = NULL;
                        algorithmPointer = NULL;
                        algorithmDestructor = NULL;

                        setAvaibleMiningAlgorithms();
                    }

                    /**
                     * Construtor.
                     * Devolve uma instância de DataMining.
                     * @param algorithm algoritmo que será usado para o processamento dos dados.
                     */
                    DataMining::DataMining(const char* algorithm) {
                        miningAlgorithm = NULL;
                        algorithmConstructor = NULL;
                        algorithmPointer = NULL;
                        algorithmDestructor = NULL;

                        setAvaibleMiningAlgorithms();
                    }

                    /**
                     * Construtor.
                     * Devolve uma instância de DataMining.
                     * @param lib nome da biblioteca que contém o algoritmo a ser executado.
                     * @param constructor construtor do algoritmo que devolve uma instância para a interface MiningAlgorithm.
                     * @param destructor destrutor do algoritmo.
                     */
                    DataMining::DataMining(const char* lib, const char* constructor, const char* destructor) {
                        setAvaibleMiningAlgorithms();

                        void* library = dlopen(lib, RTLD_LAZY);
                        if (library == NULL) {
                            cerr << "Could not open the library." << endl;
                        } else {
                            algorithmConstructor = (void*(*)(void)) dlsym(library, constructor);
                            algorithmPointer = algorithmConstructor();
                            miningAlgorithm = reinterpret_cast<MiningAlgorithm*> (algorithmPointer);

                            algorithmDestructor = (void(*)(void*)) dlsym(library, destructor);
                        }
                    }

                    /**
                     * Destrutor.
                     */
                    DataMining::~DataMining() {
                        algorithmDestructor(algorithmPointer);
                        miningAlgorithm = NULL;
                        algorithmConstructor = NULL;
                        algorithmPointer = NULL;
                        algorithmDestructor = NULL;
                    }

                    /**
                     * Define o algoritmo que será usado para o processamento dos dados.
                     * @param algorithm algoritmo que será usado para o processamento dos dados.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
#include <string.h>

                    bool DataMining::setAlgorithm(const char* algorithm) {
                        currentAlgorithm = algorithm;

                        if (currentAlgorithm == "Apriori") {
                            miningAlgorithm = new IMiningAlgorithmApriori();
                            return true;
                        } else
                            return false;

#if 0
                        switch (avaibleMiningAlgorithms.find((char*) algorithm)->second) {
                            case Apriori:
                                miningAlgorithm = new IMiningAlgorithmApriori();
                                return true;

                            default:
                                miningAlgorithm = NULL;
                        }

                        return false;
#endif
                    }

                    /**
                     * Retorna o algoritmo que será usado para o processamento dos dados.
                     * @return algoritmo que será usado para o processamento dos dados.
                     */
                    const char* DataMining::getAlgorithm() {
                        return currentAlgorithm.c_str();
                    }

                    /**
                     * Retorna todos os algoritmos disponíveis para processamento dos dados.
                     * @return todos os algoritmos disponíveis para processamento dos dados.
                     */
                    vector<char*>* DataMining::getAlgorithms() {
                        vector<char*>* algorithms = new vector<char*>;

                        if (avaibleMiningAlgorithms.size() == 0)
                            return NULL;

                        for (map<char *, int>::iterator it = avaibleMiningAlgorithms.begin(); it != avaibleMiningAlgorithms.end(); it++) {
                            cout << (*it).first << ": " << (*it).second << endl;
                            algorithms->push_back(((*it).first));
                        }

                        return algorithms;
                    }

                    /**
                     * Inicializa o algoritmo com os parâmetros especificados.
                     * @param params parâmetros para o algoritmo.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool DataMining::setParams(vector<char*> params) {
                        return (miningAlgorithm->setParams(params));
                    }

                    /**
                     * Prepara os dados para serem processados pelo algoritmo.
                     * @param data dados a serem pre-processados.
                     * @param delimiter delimitador dos dados.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool DataMining::prepare(const char* data, const char* delimiter) {
                        return (miningAlgorithm->prepare(data, delimiter));
                    }

                    /**
                     * Prepara os dados para serem processados pelo algoritmo.
                     * @param db banco de dados que contém a tabela a ser pre-processada.
                     * @param table tabela com os dados a serem pre-processados.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool DataMining::prepare(Database* db, const char* table) {
                        return (miningAlgorithm->prepare(db, table));
                    }

                    /**
                     * Executa o algoritmo.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool DataMining::start() {
                        return (miningAlgorithm->start());
                    }

                    /**
                     * Pára a execução do algoritmo.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool DataMining::stop() {
                        return (miningAlgorithm->stop());
                    }

                    /**
                     * Define a saída dos dados processados.
                     * @param data variável para armazenamento dos dados processados.
                     * @param size tamanho (em bytes) da variável para armazenamento dos dados.
                     * @param delimiter delimitador dos dados.
                     */
                    void DataMining::setOutput(char* data, long size, const char* delimiter) {
                        return (miningAlgorithm->setOutput(data, size, delimiter));
                    }

                    /**
                     * Define a saída dos dados processados.
                     * @param db banco de dados que contém a tabela onde serão armazenados os dados processados.
                     * @param table tabela onde serão armazenados os dados processados.
                     */
                    void DataMining::setOutput(Database* db, const char* table) {
                        miningAlgorithm->setOutput(db, table);
                    }

                    /**
                     * Define a saída dos dados processados.
                     * @param file caminho do arquivo para armazenamento dos dados processados.
                     */
                    void DataMining::setOutput(const char* file) {
                        miningAlgorithm->setOutput(file);
                    }

                    /**
                     * Inicializa o mapa com os nomes dos algoritmos de mineração de dados disponíveis.
                     */
                    void DataMining::setAvaibleMiningAlgorithms() {
                        currentAlgorithm = "";

                        avaibleMiningAlgorithms["Apriori"] = Apriori;
                    }

                }
            }
        }
    }
}



#include "../include/IMiningAlgorithmApriori.h"

#include <iostream>
#include <algorithm>
#define DEBUG 0

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    /**
                     * Construtor.
                     */
                    IMiningAlgorithmApriori::IMiningAlgorithmApriori() {
						inDB = NULL;

						outDataBuffer = NULL;
						outDataSize = NULL;
						outDataDelimiter = NULL;

						outDB = NULL;
						outTableName = NULL;

						outFileName = NULL;

                        userInfo = NULL;
                    }

                    /**
                     * Destrutor.
                     */
                    IMiningAlgorithmApriori::~IMiningAlgorithmApriori() {
#if 0
//XXX não deve ser utilizado caso se use a referência.
						if (inDB != NULL) {
							delete inDB;
							inDB = NULL;
						}
#endif

#if 0
//XXX não deve ser utilizado caso se use a referência.
						if (outDataBuffer != NULL) {
							delete outDataBuffer;
							outDataBuffer = NULL;
						}
#endif
						if (outDataSize != NULL) {
							delete outDataSize;
							outDataSize = NULL;
						}
						if (outDataDelimiter != NULL) {
							delete outDataDelimiter;
							outDataDelimiter = NULL;
						}

#if 0
//XXX não deve ser utilizado caso se use a referência.
						if (outDB != NULL) {
							delete outDB;
							outDB = NULL;
						}
#endif
						if (outTableName != NULL) {
							delete outTableName;
							outTableName = NULL;
						}

						if (outFileName != NULL) {
							delete outFileName;
							outFileName = NULL;
						}
                    }

                    /**
                     * Inicializa o algoritmo com os parâmetros especificados.
                     * @param params parâmetros para o algoritmo:
                     *        params[0]: min_sup parametros do algoritmo. Utilizado para desconsiderar poucas interações do usuário.
                     *        params[1]: min_confi Confiabilidade dos dados. Quanto maior esse parâmetro, menos regras são produzidas.
                     *        params[2]: banco de dados tabela contendo a tabela SI (uma abstração para as tabelas EIT e SDT).
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool IMiningAlgorithmApriori::setParams(vector<char*> params) {
                        if (params.size() != 3)
                            return false;

                        string param = params[0];
                        minSup = Utilitario::stod(param);
                        param = params[1];
                        minConfi = Utilitario::stod(param);
                        siCacheFileName = params[2];

                        return true;
                    }

					/**
					 * Prepara os dados para serem processados pelo algoritmo.
					 * @param data dados a serem pre-processados.
					 * @param delimiter delimitador dos dados.
					 * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
					*/
                    bool IMiningAlgorithmApriori::prepare(const char* data, const char* delimiter) {
                        if (data == NULL || delimiter == NULL)
                            return false;

                        inData = new string(data);

                        inDataDelimiter = new string(delimiter);

						vector<string> inDataProcess;
						Utilitario::Tokenize(*inData, inDataProcess, *inDataDelimiter);

						vector<string> inDataProcessFields;
						inDataProcessFields.push_back("CODE");
						inDataProcessFields.push_back("KEY");
						inDataProcessFields.push_back("DOC_ID");
						inDataProcessFields.push_back("FILENAME");
						inDataProcessFields.push_back("DATA");
						inDataProcessFields.push_back("HOUR");
						inDataProcessFields.push_back("WEEKDAY");
						inDataProcessFields.push_back("TIME");
    
                        userInfo = new vector<User *>;
                        userInfo = mountObject(inDataProcess, inDataProcessFields);

						if (userInfo == NULL)
							return false;

                        return true;
                    }

                    /**
                     * Prepara os dados para serem processados pelo algoritmo.
                     * @param db banco de dados que contém a tabela a ser pre-processada.
                     * @param table tabela com os dados a serem pre-processados.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool IMiningAlgorithmApriori::prepare(Database* db, const char* table) {
                        if (db == NULL || table == NULL)
                            return false;

                        inDB = db;
                        inTableName = new string(table);

                        //O método getUserInformation retorna as informações relacionais do banco aberto para objetos da classe User.
                        userInfo = getUserInformation();

						if (userInfo == NULL)
							return false;

                        return true;
                    }

                    /**
                     * Executa o algoritmo.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool IMiningAlgorithmApriori::start() {
                        //Vetores para armazenar a freqüência que o usuário assistiu a cada programa.
                        //Os dados nele armazenados serão utilizados para a mineração de dados pelo algoritmo Apriori.
                        vector<string> Pouco_Madrugada;
                        vector<string> Medio_Madrugada;
                        vector<string> Alto_Madrugada;

                        vector<string> Pouco_Manha;
                        vector<string> Medio_Manha;
                        vector<string> Alto_Manha;

                        vector<string> Pouco_Tarde;
                        vector<string> Medio_Tarde;
                        vector<string> Alto_Tarde;

                        vector<string> Pouco_Noite;
                        vector<string> Medio_Noite;
                        vector<string> Alto_Noite;

                        if (userInfo == NULL)
                            return false;

#if DEBUG
                        //O método prepareDataUser prepara os objetos da classe User em strings para serem submetidas aos algoritmos de mineração de dados.
                        vector<string> retPrepareDataUser = prepareDataUser(userInfo);
                        cout << "prepareDataUser() executado com sucesso.\ntest1: size = " << retPrepareDataUser.size() << endl;

                        //Persiste as informações preparadas pelo método prepareDataUser em um arquivo.
                        //saveUserInformation(retPrepareDataUser, "epg.txt");
                        //cout << "saveUserInformation() executado com sucesso." << endl;
#endif

                        //A tabela SI é uma abstração para as tabelas EIT e SDT.
                        vector<si*>* cacheInformation;
                        //Recupera as informações das tabelas EIT e SDT presentes no cache aberto, atribuindo-as ao vetor passado.
                        cacheInformation = restoreCacheInformation(siCacheFileName.c_str());

						if (cacheInformation == NULL) {
#if DEBUG
							cerr << "Error loading cache." << endl;
#endif
							return false;
						}

#if DEBUG
                        cout << "restoreCacheInformation() executado com sucesso.\ncacheInformation: size = " << cacheInformation->size() << endl;

                        cout << "Preparando o tempo gasto assistindo cada programa\nIsto demora dependendo dos requisitos do sistema\n " << endl;
#endif

                        //Laço que utiliza os objetos da classe User lidos para as informações dos programas assistidos.
                        //O laço converte as informações lidas para um futuro arquivo de texto que será utililizado para a mineração de dados.
                        vector<string> ret;
                        int icontador = 0;
                        for (vector<User *>::iterator it = userInfo->begin(); it != userInfo->end(); ++it) {
#if DEBUG
                            //Imprime o tempo que o programa foi assistido.
                            cout << (*it)->getTimeDuration() << endl;
#endif

                            //Calcula o percentual de tempo que o usuário assistiu ao programa atual.
                            //getPercentualByTime return : 0x0 0x0 MADRUGADA 0 Entrando no for UtilsFilter
                            //DataCache --> 4546
                            //Campos de ret:
                            //  (1) gênero
                            //  (2) gênero
                            //  (3) período
                            //  (4) quantidade
                            ret = getPercentualByTime((*it)->getFilename(), (*it)->getTimeDuration(), (*it)->getFormatDate(), cacheInformation);

                            ////Prepara o dado obtido para o formato da mineração de dados.
                            vector<string>::iterator itstring = ret.begin();
                            string genero = Utilitario::convertIntToString(++icontador);
                            ////Adiciona o delimitador.
                            genero += ",";
                            //Adiciona o gênero (1).
                            genero += *itstring;
                            genero += "\n";

                            //Avança para o próximo elemento da string com o gênero lida.
                            ++itstring;
                            //O valor continua sendo do gênero (2).
                            string y = *itstring;

                            //Converte o contador para string.
                            genero += (Utilitario::convertIntToString(icontador));
                            //Adiciona o delimitador.
                            genero += ",";
                            //Adiciona o gênero do programa (2).
                            genero += y;
                            genero += "\n";

                            //Adiciona o perídodo (3).
                            string periodo = *++itstring;
                            //Adiciona a quantidade (4).
                            string quant = *++itstring;

#if DEBUG
                            cout << "Genero = " << genero << endl;
                            cout << "quant = " << quant << endl;
                            cout << "periodo = " << periodo << endl;
#endif

                            //Armazena o gênero, filtrando os dados de acordo com o período e a quantidade de tempo que ficou sintonizado pelo usuário.
                            if (quant.compare("POUCO") == 0) {
                                if (periodo.compare("MADRUGADA") == 0) {
                                    Pouco_Madrugada.push_back(genero);
                                } else if (periodo.compare("MANHA") == 0) {
                                    Pouco_Manha.push_back(genero);
                                } else if (periodo.compare("TARDE") == 0) {
                                    Pouco_Tarde.push_back(genero);
                                } else if (periodo.compare("NOITE") == 0) {
                                    Pouco_Noite.push_back(genero);
                                }
                            } else if (quant.compare("MEDIO") == 0) {
                                if (periodo.compare("MADRUGADA") == 0) {
                                    Medio_Madrugada.push_back(genero);
                                } else if (periodo.compare("MANHA") == 0) {
                                    Medio_Manha.push_back(genero);
                                } else if (periodo.compare("TARDE") == 0) {
                                    Medio_Tarde.push_back(genero);
                                } else if (periodo.compare("NOITE") == 0) {
                                    Medio_Noite.push_back(genero);
                                }
                            } else if (quant.compare("ALTO") == 0) {
                                if (periodo.compare("MADRUGADA") == 0) {
                                    Alto_Madrugada.push_back(genero);
                                } else if (periodo.compare("MANHA") == 0) {
                                    Alto_Manha.push_back(genero);
                                } else if (periodo.compare("TARDE") == 0) {
                                    Alto_Tarde.push_back(genero);
                                } else if (periodo.compare("NOITE") == 0) {
                                    Alto_Noite.push_back(genero);
                                }
                            }
                        }

                        //Pesiste os dados filtrados em arquivos temporários, por meio do método saveUserInformation.
                        saveUserInformation(Pouco_Madrugada, "pouco_madrugada.tmp");
                        saveUserInformation(Pouco_Manha, "pouco_manha.tmp");
                        saveUserInformation(Pouco_Tarde, "pouco_tarde.tmp");
                        saveUserInformation(Pouco_Noite, "pouco_noite.tmp");

                        saveUserInformation(Medio_Madrugada, "medio_madrugada.tmp");
                        saveUserInformation(Medio_Manha, "medio_manha.tmp");
                        saveUserInformation(Medio_Tarde, "medio_tarde.tmp");
                        saveUserInformation(Medio_Noite, "medio_noite.tmp");

                        saveUserInformation(Alto_Madrugada, "alto_madrugada.tmp");
                        saveUserInformation(Alto_Manha, "alto_manha.tmp");
                        saveUserInformation(Alto_Tarde, "alto_tarde.tmp");
                        saveUserInformation(Alto_Noite, "alto_noite.tmp");

                        //Process Mining
                        //Mineração de dados.

                        //Atributos para seleciona o nome do arquivo de entrada.
                        string quant[3] = {"pouco", "medio", "alto"};
                        string concatena = "_";
                        string periodo[4] = {"madrugada", "manha", "tarde", "noite"};

                        //Executa o algoritmo Apriori.
                        //Os dados são minerados e codificados em novos arquivos tempoários.
                        for (int j = 0; j < 4; j++) {
                            for (int i = 0; i < 3; i++) {
                                string entfile = quant[i] + concatena + periodo[j] + ".tmp";
                                string saifile = quant[i] + concatena + periodo[j] + ".out";

                                //Execute - argumentos:
                                //1) arquivo com as interações do usuário (os recém-criados, neste caso)
                                //2) min_sup
                                //3) min_conf
                                execute(entfile, minSup, minConfi);

                                //Encode - argumentos:
                                //1) tabela usada para o mapeamento string --> numérico
                                //2) arquivo gerado pelo algoritmo apriori (numérico)
                                //3) fileOut arquivo de saída mapeado numérico --> string
                                encode("code_table.txt", "coded_output.txt", (char *) saifile.c_str());

                                entfile = "rm " + entfile;
                                system(entfile.c_str());
                            }
                        }

                        //Junta todos os arquivos criados em um arquivo de saída.
                        bool usingTmpFile = false;
                        if (outFileName == NULL) {
                            outFileName = new string("miningAlgorithmAprioriOutput.tmp");
                            usingTmpFile = true;
                        }

                        //Caso setOutput (para arquivo) tenha sido executado, o arquivo de saída será o do método; caso contrário, será um arquivo temporário.
                        string catCmd = "cat *.out > " + *outFileName;
                        system(catCmd.c_str());

                        if (outDataBuffer != NULL || outDB != NULL) {
                            //Caso setOutput (para variável) tenha sido executado, é feita a atribuição da saída para a variável.
                            if (outDataBuffer != NULL) {
                                ifstream miningAlgorithmOutput(outFileName->c_str());

                                if (!miningAlgorithmOutput.is_open())
                                    return false;

                                char* tmpOutDataBuffer = new char[*outDataSize];
                                miningAlgorithmOutput.getline(tmpOutDataBuffer, *outDataSize);

                                string switchDelimiter(tmpOutDataBuffer);
                                int position = switchDelimiter.find(" ");

                                while (position < switchDelimiter.size()) {
                                    switchDelimiter.replace(position, 1, *outDataDelimiter);
                                    position = switchDelimiter.find(" ", (position + 1));
                                }

                                //Apaga o último delimitador.
                                switchDelimiter.replace((switchDelimiter.size() - outDataDelimiter->size()), outDataDelimiter->size(), "");
                                strcpy(outDataBuffer, switchDelimiter.c_str());

                                delete tmpOutDataBuffer;

                                miningAlgorithmOutput.close();
                            }

                            //Caso setOutput (para banco de dados) tenha sido executado, é persistida a saída no banco.
                            if (outDB != NULL) {
                                ifstream miningAlgorithmOutput(outFileName->c_str());

                                if (!miningAlgorithmOutput.is_open())
                                    return false;

                                while (!miningAlgorithmOutput.eof()) {
                                    string value;

                                    miningAlgorithmOutput >> value;
                                    value = "insert into " + *outTableName + " VALUES (\"" + value + "\")";
                                    if (outDB->query(value.c_str()));
                                }

                                miningAlgorithmOutput.close();
                            }
                        }

                        if (userInfo != NULL) {
                            delete userInfo;
							userInfo = NULL;
						}

						//Os arquivos .lua devem ser gerados antes de os arquivos .out serem apagados.
                        system("rm *.out");
                        system("rm code_table.txt coded_output.txt");

                        if (usingTmpFile) {
                            *outFileName = "rm " + *outFileName;
                            system(outFileName->c_str());
                        }

                        return true;
                    }

                    /**
                     * Pára a execução do algoritmo.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool IMiningAlgorithmApriori::stop() {
                        return true;
                    }

                    /**
                     * Define a saída dos dados processados.
                     * @param data variável para armazenamento dos dados processados.
                     * @param size tamanho (em bytes) da variável para armazenamento dos dados.
                     * @param delimiter delimitador dos dados.
                     */
                    void IMiningAlgorithmApriori::setOutput(char* data, long size, const char* delimiter) {
                        outDataBuffer = data;
                        outDataSize = new long();
                        *outDataSize = size;
                        outDataDelimiter = new string(delimiter);
                    }

                    /**
                     * Define a saída dos dados processados.
                     * @param db banco de dados que contém a tabela onde serão armazenados os dados processados.
                     * @param table tabela onde serão armazenados os dados processados.
                     */
                    void IMiningAlgorithmApriori::setOutput(Database* db, const char* table) {
                        outDB = db;
                        outTableName = new string(table);
                    }

                    /**
                     * Define a saída dos dados processados.
                     * @param file caminho do arquivo para armazenamento dos dados processados.
                     */
                    void IMiningAlgorithmApriori::setOutput(const char* file) {
                        outFileName = new string(file);
                    }

                    /**
                     * Recupera as informações das tabelas EIT e SDT
                     * @param nomeCache o nome do cache que contém as informações das tabelas EIT & SDT
                     * @return um ponteiro para um vector contendo os registros recuperados.
                     */
                    vector<si*> * IMiningAlgorithmApriori::restoreCacheInformation(string nomeCache) {

                        vector<si*> *restauraCache = new vector<si *>;
                        si tablesi;
                        ifstream rdfile(nomeCache.c_str(), ios::in);

                        if (rdfile.bad()) {
                            cerr << "Error reading EIT and SDT tables cache." << endl;
                            //return new vector<si*>;
							return NULL;
                        }


                        // Pega o tamanho do arquivo
                        rdfile.seekg(0, ios::end);
                        int length = rdfile.tellg();
                        rdfile.seekg(0, ios::beg);

#if DEBUG
                        cout << "si sizeof() = " << sizeof (si) << endl;
#endif
                        const int calculaRegistro = length / sizeof (si);
#if DEBUG
                        cout << "registros == " << calculaRegistro << endl;
#endif
                        si * c [calculaRegistro];
                        int iElementpos = 0;

                        while ((rdfile) && (!rdfile.eof())) {
                            rdfile.read((char *) (&tablesi), sizeof (si));


                            c[iElementpos] = new si();
                            *c[iElementpos] = tablesi;

                            restauraCache->push_back(c[iElementpos]);
                            iElementpos++;

                        }

#if DEBUG
                        cout << "Elements == " << iElementpos << endl;
#endif
                        rdfile.close();
                        return restauraCache;
                    }

                    /**
                     * Converte informações relacionais do banco de dados em objetos
                     * da classe User.
                     * @return um ponteiro de vector <User*>
                     */
                    vector<User*>* IMiningAlgorithmApriori::getUserInformation() {
                        vector<string> rtnConsulta, rtnHead;

                        Database *userDB = inDB;

                        if (!userDB->createDatabase()) {
                            userDB->closeDatabase();
                            if (userDB != NULL) {
                                delete userDB;
                                userDB = NULL;
                            }
                            exit(1);
                        }

                        //userDB->query("SELECT CODE,KEY,DOC_ID,FILENAME,DATA,HOUR,WEEKDAY,TIME FROM HISTORICO", &rtnConsulta, &rtnHead);
						string selectQuery = "SELECT * FROM " + *inTableName;
						userDB->query(selectQuery.c_str(), &rtnConsulta, &rtnHead);

                        vector<User*> *usuarios;

                        usuarios = new vector<User *>;
                        usuarios = mountObject(rtnHead, rtnConsulta);

                        userDB->closeDatabase();

                        return usuarios;
                    }

                    /**
                     * Monta objetos da classe User a partir das tuplas passadas por referência.
                     * @param retornaConsulta : dados contidos nas tuplas.
                     * @param retornaHead : nome das tabelas.
                     * @return vetor com os usuários.
                     */
                    vector<User *> * IMiningAlgorithmApriori::mountObject(vector<string> rtnConsulta, vector<string> rtnHead) {
                        vector<User *> * usersVector;
                        usersVector = new vector<User *>;
                        vector<string> tokens;

                        if (rtnHead.size() == 0)
                            return NULL;

                        const int calculaNumeroLaco = (rtnConsulta.size() / rtnHead.size());

                        if (calculaNumeroLaco == 0)
                            return NULL;

                        int iElementPosition = 0;
                        User * c[calculaNumeroLaco];
                        vector<string>::iterator it;
                        vector<string>::iterator itdata;
                        vector<string>::iterator ithora;

                        while (iElementPosition < calculaNumeroLaco) {
                            if (iElementPosition == 0) {
                                it = rtnConsulta.begin();
                            } else {
                                ++it;
                            }

                            c[iElementPosition] = new User();

                            c[iElementPosition]->setCode(Utilitario::convertStringToInt(*it));
                            ++it;

                            c[iElementPosition]->setKey(*it);
                            ++it;

                            c[iElementPosition]->setDocid(*it);
                            ++it;

                            c[iElementPosition]->setFilename(*it);
                            ++it;

                            string frmData = *it;
                            Utilitario::Tokenize(frmData, tokens, "-");
                            itdata = tokens.begin();
                            c[iElementPosition]->setYear(*(itdata));
                            c[iElementPosition]->setMonth(*(++itdata));
                            c[iElementPosition]->setDay(*(++itdata));

                            tokens.clear();
                            frmData.clear();

                            ++it;
                            frmData = *it;
                            Utilitario::Tokenize(frmData, tokens, ":");
                            ithora = tokens.begin();
                            c[iElementPosition]->setHour(*(ithora));
                            c[iElementPosition]->setMinute(*(++ithora));
                            c[iElementPosition]->setSecond(*(++ithora));

                            ++it;
                            c[iElementPosition]->setWeekDay(*it);

                            ++it;
                            int duracao = Utilitario::convertStringToInt(*it);
                            c[iElementPosition]->setTimeDuration((double) duracao);

                            usersVector->push_back(c[iElementPosition]);
                            iElementPosition++;
                            itdata->clear();
                            ithora->clear();
                            tokens.clear();
                        }

                        return usersVector;
                    }

                    /**
                     * Realiza a persistência das informações processadas pelo
                     * método prepareDataUser
                     * @param user recebe uma string com os dados a serem persistidos na
                     * @param file recebe uma string que informa o path e nome do arquivo a ser
                     *        criado em disco.
                     * em disco.
                     */
                    void IMiningAlgorithmApriori::saveUserInformation(vector<string> user, string arquivo) {
                        FILE *p;
                        char *str = (char *) arquivo.c_str();
                        int i;
                        string formattedOutput;
                        /* Lê um nome para o arquivo a ser aberto: */

                        if (!(p = fopen(str, "w"))) /* Caso ocorra algum erro na abertura do arquivo..*/ { /* o programa aborta automaticamente */
                            printf("Erro! Impossível abrir o arquivo!\n");
                            exit(1);
                        }
                        /* Se não houve erro, imprime no arquivo e o fecha ...*/

                        for (vector<string>::iterator it = user.begin(); it != user.end(); ++it) {

                            formattedOutput += (*it);

                        }

                        int z = formattedOutput.size();

                        for (i = 0; i < z; i++)
                            putc(formattedOutput[i], p);

                        fclose(p);

                    }

                    /**
                     * Converte objetos da classe User em strings para serem
                     * submetidas aos algoritmos de mineração
                     * @param user um ponteiro para um vector<User *> *user
                     * @return <b> string </b> com os objetos da classe User convertidos no padrão basket_file do
                     * algoritmo apriori.
                     */
                    vector<string> IMiningAlgorithmApriori::prepareDataUser(vector<User *> *user) {
                        vector<string> tableUser;
                        string *temp = new string();
                        User *tmp;

                        if (user == NULL) {
                            tableUser.push_back(*temp);
                            return tableUser;
                        }

                        //faz loop enquanto existir registros e converte para string
                        for (vector<User *>::iterator it = user->begin(); it != user->end(); ++it) {

                            //recebe o objeto com as informações coletas
                            tmp = *it;

                            //converte para string
                            *temp = tmp->getFilename() + ";" + tmp->getFormatDate() + ";" + tmp->getFormatTime();
                            tableUser.push_back(*temp);
                            temp->clear();
                        }
                        delete temp;
                        return tableUser;
                    }

                    /**
                     * Calcular o período do dia em que um programa foi sintonizado.
                     * Exemplo: Programa sintonizado das 09:30hs até 11:00hs.
                     * retorna: manhã.
                     * @param dataColectedByUser string contendo as informações do programa, horário, data.
                     * @return string informando em qual horário o programa foi sintonizado
                     */
                    string IMiningAlgorithmApriori::getValueByTime(string dataColectedByUser) {
                        //FIXME não implementado pelo Paulo.
                        return "Test";
                    }

                    /**
                     * Retorna uma string representando o tempo que usuário
                     * permaneceu conectado em determinado programa.
                     * @param duration double que representa o percentual de tempo em
                     * que o usuário ficou conectado em um determinado programa.
                     *
                     * @return <b> string </b> que pode ser: pouco, medio, alto
                     */
                    string IMiningAlgorithmApriori::verifyDuration(double duration) {

                        if (duration > DESPREZIVEL && duration <= POUCO) {
                            return "POUCO";
                        } else if (duration > POUCO && duration <= MEDIO) {
                            return "MEDIO";
                        } else if (duration > MEDIO && duration <= ALTO) {
                            return "ALTO";
                        }

                        return "DESPREZIVEL";
                    }

                    /**
                     * Quando não existirem dados suficiente para o algoritmo apriori processar
                     * é necessário preencher o arquivo de saída lua com informações randômicas.
                     * @param min informa o menor valor gerado
                     * @param max informa o máximo valor gerado
                     * @return <b> int </b> onde esse valor é maior igual ao minimo e menor igual ao máximo.
                     */
                    int IMiningAlgorithmApriori::getRandom(int min, int max) {

                        int iSecret;

                        /* initialize random seed: */
                        srand(time(NULL));

                        /* generate secret number: */
                        iSecret = rand() % max + 1;


                        return iSecret;

                    }

                    /**
                     * Calcular o percentual de tempo que o usuário sintonizou
                     * determinando programa.
                     * @param dataColectedByUser string contendo as informações do programa, horário, data.
                     * @param dataCacheByTable vector <si*> * um ponteiro para os dados restaurados da tabela EIT & SDT.
                     * @return string informando: pouco, medio,alto
                     */
                    string IMiningAlgorithmApriori::getPercentualByTime(const string dataColectedByUser, vector <si*> * dataCacheByTable) {

                        try {
                            /* recupera as informações do banco de dados coletado do usuário */
                            vector<string> temporario; /// recebe os tokens
                            vector<string> tmp_inicio; /// separa a data da hora
                            vector<string> tmp_final; /// separa a data da hora
                            vector<string> dtdTempI; /// recebe a data tokenizada.
                            vector<string> tmeTempI; /// recebe a hora tokenizada.
                            vector<string> dtdTempF; /// recebe a data tokenizada.
                            vector<string> tmeTempF; /// recebe a hora tokenizada.

                            struct tm dt_inicio_user;
                            struct tm dt_fim_user;

                            string compareTableSiUserData;
                            string horaInicialCache;
                            string horaFinalCache;

                            double tempoUsuarioPrograma = 0;
                            double tempoCachePrograma = 0;
                            double percentual = 0;

                            Utilitario::Tokenize(dataColectedByUser, temporario, ";");
                            Utilitario::Tokenize(temporario[1], tmp_inicio, " ");
                            Utilitario::Tokenize(temporario[2], tmp_final, " ");

                            /* hora de inicio */
                            Utilitario::Tokenize(tmp_inicio[0], dtdTempI, "-");
                            Utilitario::Tokenize(tmp_inicio[1], tmeTempI, ":");

                            /* hora final */
                            Utilitario::Tokenize(tmp_final[0], dtdTempF, "-");
                            Utilitario::Tokenize(tmp_final[1], tmeTempF, ":");


                            /* Nesse ponto já temos a datainicio, datafinal tokenizadas
                             * Agora vamos implementar  as struct tm com essas
                             * informações
                             */

                            dt_inicio_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempI[0]), Utilitario::convertStringToInt(dtdTempI[1]),
                                    Utilitario::convertStringToInt(dtdTempI[2]), Utilitario::convertStringToInt(tmeTempI[0]), Utilitario::convertStringToInt(tmeTempI[1]),
                                    Utilitario::convertStringToInt(tmeTempI[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            dt_fim_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempF[0]), Utilitario::convertStringToInt(dtdTempF[1]),
                                    Utilitario::convertStringToInt(dtdTempF[2]), Utilitario::convertStringToInt(tmeTempF[0]), Utilitario::convertStringToInt(tmeTempF[1]),
                                    Utilitario::convertStringToInt(tmeTempF[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            /* calcula o tempo que o usuário assitiu o programa */
                            tempoUsuarioPrograma = Utilitario::diferenceDates(&dt_inicio_user, &dt_fim_user);

                            /*
                             * Realizar a busca no vector com a cache para determinar o
                             * tempo total do programa e criar a struct tm com essas
                             * informações
                             */

                            for (vector<si *>::iterator it = dataCacheByTable->begin(); it != dataCacheByTable->end(); it++) {
                                compareTableSiUserData = static_cast<string> ((*it)->getServiceName());
                                if (compareTableSiUserData.compare(temporario[0]) == 0) { //verifica se encontrou o programa na EIT & SDT
                                    horaInicialCache = (*it)->getStart_time();
                                    horaFinalCache = (*it)->getEnd_time();
                                    break;
                                }
                            }

                            /* Executar a função para calcular o tempo que o usuário
                             * assistiu o programa e a duração total do programa
                             * e em seguida calcular o percentual
                             */

                            /* limpa os vector para reaproveitar a instancia */
                            temporario.clear();
                            tmp_inicio.clear();
                            tmp_final.clear();
                            dtdTempI.clear();
                            tmeTempI.clear();
                            dtdTempF.clear();
                            tmeTempF.clear();

                            /* hora de inicio */
                            Utilitario::Tokenize(horaInicialCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempI, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempI, ":");

                            /* hora final */
                            temporario.clear();
                            Utilitario::Tokenize(horaFinalCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempF, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempF, ":");

                            /* Nesse ponto já temos a datainicio, datafinal tokenizadas
                             * Agora vamos implementar  as struct tm com essas
                             * informações
                             */

                            dt_inicio_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempI[0]), Utilitario::convertStringToInt(dtdTempI[1]),
                                    Utilitario::convertStringToInt(dtdTempI[2]), Utilitario::convertStringToInt(tmeTempI[0]), Utilitario::convertStringToInt(tmeTempI[1]),
                                    Utilitario::convertStringToInt(tmeTempI[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            dt_fim_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempF[0]), Utilitario::convertStringToInt(dtdTempF[1]),
                                    Utilitario::convertStringToInt(dtdTempF[2]), Utilitario::convertStringToInt(tmeTempF[0]), Utilitario::convertStringToInt(tmeTempF[1]),
                                    Utilitario::convertStringToInt(tmeTempF[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            /* calcula o tempo que o usuário assitiu o programa */
                            tempoCachePrograma = Utilitario::diferenceDates(&dt_inicio_user, &dt_fim_user);

                            percentual = (tempoUsuarioPrograma / tempoCachePrograma) * 100;

                            return (verifyDuration(percentual));
                        } catch (exception e) {

                        }
                    }

                    /**
                     * Calcular o percentual de tempo que o usuário sintonizou determinando programa.
                     * @param nomePrograma string contendo as informações do programa para comparação com as informações da EIT no cache.
                     * @param tempoUsuario tempoUsuario double contendo o tempo em que o usuário ficou sintonizado no programa
                     * @param dataInicio string contendo a data atual para recuperar os programas da tabela EIT & SDT
                     * @param dataCacheByTable vector si* um ponteiro para os dados restaurados da tabela EIT & SDT.
                     * @return vector contendo o tempo que o usuário assitiu cada programa.
                     */
                    vector <string>IMiningAlgorithmApriori::getPercentualByTime(const string nomePrograma, double tempoUsuario, string dataInicio, vector <si*> * dataCacheByTable) {

                        /* recupera as informações do banco de dados coletado do usuário */

                        try {

                            vector<string> temporario; /// recebe os tokens
                            vector<string> tmp_inicio; /// separa a data da hora
                            vector<string> tmp_final; /// separa a data da hora
                            vector<string> dtdTempI; /// recebe a data tokenizada.
                            vector<string> tmeTempI; /// recebe a hora tokenizada.
                            vector<string> dtdTempF; /// recebe a data tokenizada.
                            vector<string> tmeTempF; /// recebe a hora tokenizada.
                            vector<string> retorno; /// armazena o retorno da funçao.

                            struct tm dt_inicio_user;
                            struct tm dt_fim_user;

                            string compareTableSiUserData;
                            vector <string> compareTableSiUserDataHora;
                            vector <string> dataInicioTokenizer;
                            string horaInicialCache;
                            string horaFinalCache;

                            double tempoUsuarioPrograma = tempoUsuario * 60;
                            double tempoCachePrograma = 0;
                            double percentual = 0;

                            Utilitario::Tokenize(dataInicio, dataInicioTokenizer, " ");
                            dataInicio = dataInicioTokenizer[0];

                            /*
                             * Realizar a busca no vector com a cache para determinar o
                             * tempo total do programa e criar a struct tm com essas
                             * informações
                             */

#if DEBUG
                            cout << "Entrando no for UtilsFilter" << endl;
                            cout << "DataCache --> " << dataCacheByTable->size() << endl;
#endif

                            for (vector<si *>::iterator it = dataCacheByTable->begin(); it != dataCacheByTable->end(); it++) {
                                compareTableSiUserData = static_cast<string> ((*it)->getServiceName());
                                string tmp = static_cast<string> ((*it)->getStart_time());
                                Utilitario::Tokenize(tmp, compareTableSiUserDataHora, " ");

                                if ((compareTableSiUserData.compare(nomePrograma) == 0)) { //verifica se encontrou o programa na EIT & SDT
                                    horaInicialCache = (*it)->getStart_time();
                                    horaFinalCache = (*it)->getEnd_time();
                                    string genero = (*it)->getDescriptor_conteudo_genero();
                                    string subgenero = (*it)->getDescriptor_conteudo_subgenero();
                                    string sinopse = (*it)->getDescriptor_event_short_sinopse();
#if DEBUG
                                    // cout << "Sinopse lida " << sinopse << endl;
                                    // cout << "genero " << genero << endl;
                                    // cout << "sub-genero " << subgenero << endl;
#endif
                                    retorno.push_back(genero);
                                    retorno.push_back(subgenero);
                                    break;
                                } else {
                                    vector<string> t;
                                    t.push_back("0x0");
                                    t.push_back("0x0");
                                    t.push_back("MADRUGADA");
                                    t.push_back("0");
                                    return t;
                                }
                            }

#if DEBUG
                            cout << "Saindo do loop for UtilsFilter +++ " << endl;
#endif
                            /* Executar a função para calcular o tempo que o usuário
                             * assistiu o programa e a duração total do programa
                             * e em seguida calcular o percentual
                             */

                            /* limpa os vector para reaproveitar a instancia */
                            temporario.clear();
                            tmp_inicio.clear();
                            tmp_final.clear();
                            dtdTempI.clear();
                            tmeTempI.clear();
                            dtdTempF.clear();
                            tmeTempF.clear();

#if DEBUG
                            cout << "Limpando Registro ... " << endl;
#endif

                            /* hora de inicio */
                            Utilitario::Tokenize(horaInicialCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempI, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempI, ":");

                            /* hora final */
                            temporario.clear();
                            Utilitario::Tokenize(horaFinalCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempF, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempF, ":");

                            /* Nesse ponto já temos a datainicio, datafinal tokenizadas
                             * Agora vamos implementar  as struct tm com essas
                             * informações
                             */

                            dt_inicio_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempI[0]), Utilitario::convertStringToInt(dtdTempI[1]),
                                    Utilitario::convertStringToInt(dtdTempI[2]), Utilitario::convertStringToInt(tmeTempI[0]), Utilitario::convertStringToInt(tmeTempI[1]),
                                    Utilitario::convertStringToInt(tmeTempI[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            dt_fim_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempF[0]), Utilitario::convertStringToInt(dtdTempF[1]),
                                    Utilitario::convertStringToInt(dtdTempF[2]), Utilitario::convertStringToInt(tmeTempF[0]), Utilitario::convertStringToInt(tmeTempF[1]),
                                    Utilitario::convertStringToInt(tmeTempF[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            struct tm checkFim = dt_inicio_user;
                            struct tm checkInicio = dt_inicio_user;
                            checkFim.tm_hour = 12;
                            checkFim.tm_min = 0;
                            checkFim.tm_sec = 0;

                            checkInicio.tm_hour = 6;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 0;

                            /* calcula o tempo que o usuário assitiu o programa */
                            tempoCachePrograma = Utilitario::diferenceDates(&dt_inicio_user, &dt_fim_user);

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                retorno.push_back("MANHA");
                            }

                            checkFim.tm_hour = 18;
                            checkFim.tm_min = 0;
                            checkFim.tm_sec = 0;

                            checkInicio.tm_hour = 12;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 1;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                retorno.push_back("TARDE");
                            }

                            checkFim.tm_hour = 23;
                            checkFim.tm_min = 59;
                            checkFim.tm_sec = 59;

                            checkInicio.tm_hour = 18;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 1;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                retorno.push_back("NOITE");
                            }

                            checkFim.tm_hour = 05;
                            checkFim.tm_min = 59;
                            checkFim.tm_sec = 59;

                            checkInicio.tm_hour = 0;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 0;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                retorno.push_back("MADRUGADA");
                            }

                            if (tempoUsuario > tempoCachePrograma)
                                tempoCachePrograma = tempoUsuario;

                            if (tempoCachePrograma > 0)
                                percentual = (tempoUsuarioPrograma / tempoCachePrograma) * 100;
                            else
                                percentual = 0;

#if DEBUG
                            cout << "percentual = " << percentual << endl;
#endif
                            retorno.push_back(verifyDuration(percentual));
                            return retorno;
                        } catch (exception e) {
                            vector<string> t;
                            t.push_back("MADRUGADA");
                            t.push_back("0");
                            return t;
                        }
                    }

                    /**
                     * O algoritmo apriori produz a saída toda em formato numérico. Esse método mapea a saída do algoritmo
                     * numérico para strings.
                     * @param fileCodeTable tabela usada para o mapeamento string --> numérico
                     * @param fileOutMining arquivo gerado pelo algoritmo apriori (numérico)
                     * @param fileOut arquivo de saída mapeado numérico --> string
                     */
                    void IMiningAlgorithmApriori::encode(char * fileCodeTable, char * fileInMining, char *fileOut) {
                        code_file_process(fileCodeTable);
                        code_file_process_out_Algorithm(fileInMining);
                        code_file_process_out(fileOut);
                        decode();
                    }

                    void IMiningAlgorithmApriori::code_file_process(char* code_file_name) {
                        itemtype item_code;
                        string tempkar;

                        ifstream kodfile(code_file_name);
                        if (!kodfile) {
                            cerr << endl << "Codefile does not exists." << endl;
                            return;
                        }
                        while (kodfile) {
                            kodfile >> item_code;
                            kodfile.get(); //reading in the colon
                            getline(kodfile, tempkar);
                            codearray[item_code] = tempkar;
                        }

                        kodfile.close();
                    }

                    void IMiningAlgorithmApriori::code_file_process_out_Algorithm(char* outAlgorithm) {

                        char s[80];
                        int t;
                        imprimir = "";

                        if ((code_file_algorithm = fopen(outAlgorithm, "r")) == NULL) {
                            imprimir += "Cannot open file";
                            exit(1);
                        }
                    }

                    void IMiningAlgorithmApriori::code_file_process_out(char* outAlgorithm) {

                        char s[80];
                        int t;

                        code_file_out.open(outAlgorithm, ios::out);

                        if (code_file_out.fail()) {
                            imprimir += "Cannot open file\n";
                        }

                    }

                    /**
                     * Adiciona o item decodificado à string que será impressa ao final da execução.
                     * @param item elemento que será escrito.
                     */
                    void IMiningAlgorithmApriori::write_decoded_item(itemtype item) {
                        if (codearray[item] != "") {
                            imprimir += codearray[item];
                        } else
                            imprimir += item;
                    }

                    ///  Decodes the standard input and write the result to the standard output.

                    /**
                     * Decodifica o arquivo que contém os dados da interação do usuário com o middleware.
                     */
                    void IMiningAlgorithmApriori::decode() {
                        double conf;
                        itemtype itemset_size = 0,
                                item_index;
                        unsigned long temp_stor;

                        fscanf(code_file_algorithm, "Frequent 0-itemsets:\n");
                        fscanf(code_file_algorithm, "itemset (occurrence)\n");
                        fscanf(code_file_algorithm, "{} (%lu)\n", &temp_stor);

                        while (fscanf(code_file_algorithm, "Frequent %lu-itemsets:\n", &itemset_size) == 1) {

                            fscanf(code_file_algorithm, "itemset (occurrence)\n");
                            while (fscanf(code_file_algorithm, "%lu ", &temp_stor) == 1) {
                                if (itemset_size > 1) {
                                    write_decoded_item(temp_stor);
                                    imprimir += " ";
                                }
                                for (item_index = 1; item_index < itemset_size; item_index++) {
                                    fscanf(code_file_algorithm, "%lu ", &temp_stor);
                                    if (itemset_size > 1) {
                                        write_decoded_item(temp_stor);
                                        imprimir += " ";
                                    }
                                }
                                fscanf(code_file_algorithm, "(%lu)\n", &temp_stor);
                                if (itemset_size > 1) {
                                    imprimir += "\n";
                                }
                            }
                        }

                        fscanf(code_file_algorithm, "\nAssociation rules:\ncondition ==> consequence (confidence, occurrence)\n");

                        while (fscanf(code_file_algorithm, "%lu ", &temp_stor) == 1) {
                            write_decoded_item(temp_stor);

                            imprimir += " ";

                            fscanf(code_file_algorithm, "==>");
                            imprimir += "=";
                            while (fscanf(code_file_algorithm, " %lu", &temp_stor) == 1) {
                                imprimir += " ";
                                write_decoded_item(temp_stor);
                            }
                            fscanf(code_file_algorithm, " (%lf, %lu)\n", &conf, &temp_stor);

                            imprimir += " (";
                            imprimir += temp_stor;
                            imprimir += ")\n";
                        }

                        code_file_out << imprimir;

                        code_file_out.close();
                    }

                    /**
                     * Executa o algoritmo apriori
                     * @param file arquivo que contém as interações do usuário (comportamento do usuário)
                     * @param min_sup parametros do algoritmo. Utilizado para desconsiderar poucas interações do usuário.
                     * @param min_confi Confiabilidade dos dados. Quanto maior esse parâmetro, menos regras são produzidas.
                     */
                    void IMiningAlgorithmApriori::execute(string file, double min_sup, double min_confi) {
                        /**
                         * run_me script by Paulo Muniz de Ávila
                         * ./convert_baskets coded_baskets.txt>converted_baskets.txt
                         * rm coded_baskets.txt
                         * ../apriori converted_baskets.txt coded_output.txt $3 $4
                         * rm converted_baskets.txt
                         * echo Decoding the items.
                         * cat ./coded_output.txt|./decode_items code_table.txt
                         * rm coded_output.txt
                         * rm code_table.txt
                         */

                        string outputFile = "coded_output.txt";
                        bool store_input = false;

                        double min_supp = min_sup, min_conf = min_confi;
                        bool quiet = false;
                        unsigned long size_threshold = 0;

                        /* Codificando os itens extraidos da tabela */
                        string comando = "./code_basket  " + file + " > coded_baskets.txt";
                        system(comando.c_str());
                        system("./convert_baskets coded_baskets.txt>converted_baskets.txt");
                        system("rm coded_baskets.txt");

                        /* Executa o algoritmo Apriori e produz as regras
                         * de associação
                         */

                        ifstream basket_file;
                        basket_file.open("converted_baskets.txt");
                        if (!basket_file) {

                            cerr << "\nError! The basket file can not be read!" << flush;
                            return;
                        }

                        Apriori apriori(basket_file, outputFile.c_str(), store_input);
                        apriori.APRIORI_alg(min_supp, min_conf, quiet, size_threshold);
                        basket_file.close();

                        system("rm converted_baskets.txt");
                    }

                    /*
                    0x0|0X0F|Jornalismo
                    0x0|0x00|Jornalismo
                    0x0|0x01|Jornalismo
                    0x0|0x02|Jornalismo
                    0x1|0x00|Esporte
                    0x2|0X0F|Outros
                    0x2|0x00|Educativo
                    0x3|0x00|Novela
                    0x5|0x00|Reality show
                    0x5|0x00|Serie/seriado
                    0x6|0X0F|Variedade
                    0x6|0x00|Variedade
                    0x6|0x01|Variedade
                    0x6|0x02|Variedade
                    0x6|0x03|Variedade
                    0x6|0x04|Variedade
                    0x6|0x05|Variedade
                    0x7|0x00|Reality show
                    0x8|0X0F|Informacao
                    0x8|0X0F|Variedade
                    0x8|0x00|Informacao
                    0x8|0x01|Informacao
                    0x8|0x02|Informacao
                    0x8|0x03|Informacao
                    0x8|0x04|Informacao
                    0x9|0x00|Humoristico
                    0xA|0x00|Infantil
                    0xB|0x00|Er¢tico
                    0xC|0X0F|Filme
                    0xC|0x00|Filme
                    0xD|0X0F|Sorteio, televendas, premiacao
                    0xD|0x00|Sorteio, televendas, premiacao
                    0xD|0x01|Sorteio, televendas, premiacao
                    0xD|0x02|Sorteio, televendas, premiacao
                    0xE|0x00|Debate/entrevista
                    0xE|0x01|Debate/entrevista
                    0xF|0X0F|Outros
                    0xF|0x00|Outros
                    0xF|0x01|Outros
                    0xF|0x02|Outros
                    0xF|0x03|Outros */

                    /**
                     * Prepara o banco para receber as novas recomendacoes
                     */
                    void IMiningAlgorithmApriori::prepareToSaveRecommender() {
                        /*
                                DataRecommender dt;
                            dt.executeSql("mining.db", "delete from recommender");
                         */

                        Database* mining = new Database("", "", "mining.db");

                        bool result = mining->createDatabase();

                        if (!result) {
                            cerr << "Nao foi possivel abrir o banco!" << std::endl;
                            mining->closeDatabase();
                            exit(1);
                        } else {
#if DEBUG
                            std::cout << "Banco de dados opened !" << std::endl;
#endif

                            mining->query("delete from recommender");
                        }

                        mining->closeDatabase();
                        //delete mining;
                    }

                    /**
                     * Salva a recomendação no arquivo de formato lua.
                     * Exemplo de arquivo salvo:
                     * NOITE{
                     * Genero Cod = "0x0",
                     * SubGenero Cod = "0x02",
                     * Inicio = "2008-03-12 23:32:00",
                     * Fim = "2008-03-13 00:29:00",
                     * Programa = "REPORTER RECORD SG",
                     * Emissora = "006",
                     * }
                     *
                     * @param arquivo nome do arquivo para salvar as recomendações
                     * @param user recomendações ofertadas para o usuário
                     * @param horario que pode ser: madrugada, manha, tarde e noite
                     */
                    void IMiningAlgorithmApriori::saveRecommender(string arquivo, vector<string> *user, string horario) {
                        FILE *p;
                        char c;
                        char *str = (char *) arquivo.c_str();
                        int i;
                        vector<string>::iterator it = user->begin();
                        string formattedOutput;

                        /* Lê um nome para o arquivo a ser aberto: */

                        if (!(p = fopen(str, "w"))) /* Caso ocorra algum erro na abertura do arquivo..*/ {
                            printf("Erro! Impossível abrir o arquivo!\n");
                            return;
                        }

                        int tam = user->size() / 6;

#if DEBUG
                        cout << "user-size() = " << tam << endl;
#endif

                        bool isPossibleSaveData = false;

                        //DataRecommender miningDatabase;
                        Database* miningDatabase = new Database("", "", "mining.db");
                        miningDatabase->createDatabase();

                        vector <string> *tmpRecommender = new vector<string>;

                        string tmpgen;
                        string tmpa;
                        string tmpb;
                        string inicio;
                        string fim;
                        string programa;
                        string emissora;
                        string sinopse;

                        formattedOutput += horario;
                        formattedOutput += "={}\n";

                        for (int i = 0; i < tam - 1; i++) {

                            tmpa = (*it);
                            it++;
                            tmpb = (*it);
                            it++;
                            tmpgen = getGenProgram(tmpa, tmpb);
                            formattedOutput += horario + "[";
                            formattedOutput += Utilitario::convertIntToString(i) + "]={";
                            formattedOutput += "\n   Genero = \"" + tmpgen;
                            formattedOutput += "\",\n   Inicio = \"" + (*it);
                            inicio = (*it);
                            it++;
                            formattedOutput += "\",\n   Fim = \"" + (*it);
                            fim = (*it);
                            it++;
                            formattedOutput += "\",\n   Programa = \"" + (*it);
                            programa = (*it);
                            it++;
                            formattedOutput += "\",\n   Emissora = \"" + (*it);
                            emissora = (*it);
                            it++;
                            formattedOutput += "\",\n   Sinopse = \"" + (*it);
                            sinopse = (*it);
                            it++;
                            formattedOutput += "\"\n}\n";

                            /* prepara o vetor com as recomendacoes */

                            tmpRecommender->push_back(tmpgen);
                            tmpRecommender->push_back(inicio);
                            tmpRecommender->push_back(fim);
                            tmpRecommender->push_back(programa);
                            tmpRecommender->push_back(emissora);
                            tmpRecommender->push_back(sinopse);
                            tmpRecommender->push_back(horario);

                            /* inserir na base de dados a recomendaçao */

                            //miningDatabase.insere(tmpRecommender);
                            char sql[1024];
                            int rc;
                            std::vector<string>::iterator it = tmpRecommender->begin();
                            string gen = *it;
                            ++it;
                            string ini = *it;
                            ++it;
                            string fim = *it;
                            ++it;
                            string pro = *it;
                            ++it;
                            string emi = *it;
                            ++it;
                            string sin = *it;
                            ++it;
                            string hor = *it;
                            ++it;

                            sprintf(sql, "INSERT INTO RECOMMENDER (genero,inicio,fim,programa,emissora,sinopse,horario) VALUES ('%s','%s','%s','%s','%s','%s','%s')", gen.c_str(), ini.c_str(), fim.c_str(), pro.c_str(), emi.c_str(), sin.c_str(), hor.c_str());

                            string execute = sql;
                            miningDatabase->query(sql);

#if DEBUG
                            std::cout << "Inseriu um registro " << sql << " " << std::endl;
#endif

                            tmpRecommender->clear();
                        }

                        int z = formattedOutput.size();

                        for (i = 0; i < z; i++)
                            putc(formattedOutput[i], p);

                        //delete miningDatabase;
                        fclose(p);
                    }

                    /**
                     * retorna o Genero do Programa a partir do codigo
                     * @param gen string com o genero
                     * @return o genero, por exemplo, Jornalismo
                     **/
                    string IMiningAlgorithmApriori::getGenProgram(string gen, string subgen) {
                        if (gen.compare("0x0") == 0)
                            return "Jornalismo";

                        if (gen.compare("0x1") == 0)
                            return "Esporte";

                        //0x2|0X0F|Outros
                        //0x2|0x00|Educativo
                        if (gen.compare("0X2") == 0) {
                            if (subgen.compare("0X0F") == 0)
                                return "Outros";
                            else if (subgen.compare("0x00") == 0)
                                return "Educativo";
                        }

                        if (gen.compare("0x03") == 0)
                            return "Novela";

                        //0x5|0x00|Reality show
                        //0x5|0x00|Serie/seriado

                        if (gen.compare("0x5") == 0) {
                            return "Seriado";
                        }

                        //0x6|0X0F|Variedade
                        //0x6|0x00|Variedade
                        //0x6|0x01|Variedade
                        //0x6|0x02|Variedade
                        //0x6|0x03|Variedade
                        //0x6|0x04|Variedade
                        //0x6|0x05|Variedade

                        if (gen.compare("0x6") == 0) {
                            return "Variedade";
                        }

                        //0x8|0X0F|Informacao
                        //0x8|0X0F|Variedade
                        //0x8|0x00|Informacao
                        //0x8|0x01|Informacao
                        //0x8|0x02|Informacao
                        //0x8|0x03|Informacao
                        //0x8|0x04|Informacao

                        if (gen.compare("0x8") == 0) {
                            return "Informacao/Variedade";
                        }

                        //0x9|0x00|Humoristico
                        if (gen.compare("0x9") == 0) {
                            return "Humoristico";
                        }

                        //0xA|0x00|Infantil

                        if (gen.compare("0xA") == 0)
                            return "Infantil";

                        // 0xB|0x00|Er¢tico

                        if (gen.compare("0xB") == 0)
                            return "Erotico";

                        // 0xC|0X0F|Filme
                        // 0xC|0x00|Filme

                        if (gen.compare("0xc") == 0)
                            return "Filme";

                        //0xD|0X0F|Sorteio, televendas, premiacao

                        if (gen.compare("0xD") == 0)
                            return "Sorteio/Premiacao";

                        if (gen.compare("0xE") == 0)
                            return "Debate";

                        //se chegou ate aqui sem retornar
                        //outros
                        return "Outros";
                    }

                    vector<string> IMiningAlgorithmApriori::getLine() {
                        return line;
                    }

                    void IMiningAlgorithmApriori::setLine(vector<string> line) {
                        this->line = line;
                    }

                    /**
                     * Evita que ocorram duplicações nas recomedações
                     */
                    void IMiningAlgorithmApriori::controlDuplicate() {
                        count = 0;
                    }

                    /**
                     * Leitura de arquivo texto do disco
                     * @param arquivo nome do arquivo a ser lido
                     * @return número de bytes lidos do arquivo
                     */
                    int IMiningAlgorithmApriori::ReadFile(char *arquivo) {
                        vector<string> temporario = getLine();

                        char buffer[256];

                        ifstream is;
                        is.open(arquivo, ios::in);

                        while (is.getline(buffer, 256)) {
                            temporario.push_back(buffer);
                        }

                        setLine(temporario);
                        is.close();
                        return 1;

                    }

                    /**
                     * Busca na tabela EIT o programa recomendado
                     * @param genero string que informa o genero do programa
                     * @param subgenero string que informa o sub-genero do programa
                     * @param per_dia string que informa o dia para procurar na tabela EIT
                     * @param programacao a tabela EIT
                     * @return
                     */
                    vector<string> IMiningAlgorithmApriori::searchProgrambyId(string genero, string subgenero, string per_dia, vector<string> *programacao) {

                        vector<string> temporario; /// recebe os tokens
                        vector<string> tmp_inicio; /// separa a data da hora
                        vector<string> tmp_final; /// separa a data da hora
                        vector<string> dtdTempI; /// recebe a data tokenizada.
                        vector<string> tmeTempI; /// recebe a hora tokenizada.
                        vector<string> dtdTempF; /// recebe a data tokenizada.
                        vector<string> tmeTempF; /// recebe a hora tokenizada.
                        vector<string> retorno; /// armazena o retorno da funçao.
                        vector <string>::iterator p;
                        vector <int> tmp;
                        bool checkpoint = false;

                        struct tm dt_inicio_user;
                        struct tm dt_fim_user;

                        string compareTableSiUserData;
                        vector <string> compareTableSiUserDataHora;
                        vector <string> dataInicioTokenizer;
                        string horaInicialCache;
                        string horaFinalCache;
                        string dataInicio;
                        string generoLido;
                        string subgeneroLido;
                        string servico;
                        string provedor;
                        string hoje;
                        string periodo;
                        string sinopse;

                        vector <si*> *cacheInformation;
#if DEBUG
                        //cout << "Carregando cache" << endl;
#endif
                        cacheInformation = restoreCacheInformation(siCacheFileName.c_str()); //recupera os dados da tabela EIT & SDT armazenados na cache

						if (cacheInformation == NULL) {
#if DEBUG
							cerr << "Error loading cache." << endl;
#endif
							exit(1);
						}

#if DEBUG
                        //cout << "Cache carregada " << endl;
#endif
                        time_t rawtime;
                        struct tm * timeinfo;

                        time(&rawtime);
                        timeinfo = localtime(&rawtime);

                        int day = (timeinfo->tm_mday);
                        string month = Utilitario::convertIntToString(timeinfo->tm_mon + 1);

                        int indice;
                        checkpoint = false;
                        indice = 0;

                        for (vector <si*>::iterator it = cacheInformation->begin(); it != cacheInformation->end(); it++) {
                            indice++;
                            generoLido = (*it)->getDescriptor_conteudo_genero();
                            subgeneroLido = (*it)->getDescriptor_conteudo_subgenero();
                            horaInicialCache = (*it)->getStart_time();
                            horaFinalCache = (*it)->getEnd_time();

#if DEBUG
                            //cout << "Laco for genero lido " << generoLido << " " << subgeneroLido << endl;
#endif

                            sinopse = (*it)->getDescriptor_event_short_sinopse();

                            /* descobrir em qual periodo do dia o programa listado está sendo apresentado */
                            /* limpa os vector para reaproveitar a instancia */
                            temporario.clear();
                            tmp_inicio.clear();
                            tmp_final.clear();
                            dtdTempI.clear();
                            tmeTempI.clear();
                            dtdTempF.clear();
                            tmeTempF.clear();

                            /* hora de inicio */
                            Utilitario::Tokenize(horaInicialCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempI, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempI, ":");

                            /* hora final */
                            temporario.clear();
                            Utilitario::Tokenize(horaFinalCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempF, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempF, ":");

                            /* Nesse ponto já temos a datainicio, datafinal tokenizadas
                             * Agora vamos implementar  as struct tm com essas
                             * informações
                             */

                            dt_inicio_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempI[0]), Utilitario::convertStringToInt(dtdTempI[1]),
                                    Utilitario::convertStringToInt(dtdTempI[2]), Utilitario::convertStringToInt(tmeTempI[0]), Utilitario::convertStringToInt(tmeTempI[1]),
                                    Utilitario::convertStringToInt(tmeTempI[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            dt_fim_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempF[0]), Utilitario::convertStringToInt(dtdTempF[1]),
                                    Utilitario::convertStringToInt(dtdTempF[2]), Utilitario::convertStringToInt(tmeTempF[0]), Utilitario::convertStringToInt(tmeTempF[1]),
                                    Utilitario::convertStringToInt(tmeTempF[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                            struct tm checkFim = dt_inicio_user;
                            struct tm checkInicio = dt_inicio_user;
                            checkFim.tm_hour = 12;
                            checkFim.tm_min = 0;
                            checkFim.tm_sec = 0;

                            checkInicio.tm_hour = 6;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 0;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                periodo = "MANHA";
                            }

                            checkFim.tm_hour = 18;
                            checkFim.tm_min = 0;
                            checkFim.tm_sec = 0;

                            checkInicio.tm_hour = 12;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 1;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                periodo = "TARDE";
                            }

                            checkFim.tm_hour = 23;
                            checkFim.tm_min = 59;
                            checkFim.tm_sec = 59;

                            checkInicio.tm_hour = 18;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 1;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                periodo = "NOITE";
                            }

                            checkFim.tm_hour = 05;
                            checkFim.tm_min = 59;
                            checkFim.tm_sec = 59;

                            checkInicio.tm_hour = 0;
                            checkInicio.tm_min = 0;
                            checkInicio.tm_sec = 0;

                            /* verifica em qual horário do dia estamos */
                            if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                                periodo = "MADRUGADA";
                            }

                            /* hora de inicio */
                            Utilitario::Tokenize(horaInicialCache, temporario, " ");
                            Utilitario::Tokenize(temporario[0], dtdTempI, "-");
                            Utilitario::Tokenize(temporario[1], tmeTempI, ":");

                            int dayAtual = Utilitario::convertStringToInt(dtdTempI[2]);
                            string monthAtual = tmeTempI[1];

                            if ((per_dia.compare(periodo.c_str()) == 0) && (dayAtual == day)) {

                                tmp.push_back(indice);
                            }

                            if ((generoLido.compare(genero.c_str()) == 0) && (subgeneroLido.compare(subgenero.c_str()) == 0) && (per_dia.compare(periodo.c_str()) == 0)) {
                                servico = (*it)->getServiceName();
                                provedor = (*it)->getServiceProviderName();
                                sinopse = (*it)->getDescriptor_event_short_sinopse();

                                /* verifica se não existe duplicidade na lista de recomendacao */
                                p = find(programacao->begin(), programacao->end(), servico);

                                if ((day == dayAtual) && (p == programacao->end())) {
                                    checkpoint = true;
                                    programacao->push_back(generoLido);
                                    programacao->push_back(subgeneroLido);
                                    programacao->push_back(horaInicialCache);
                                    programacao->push_back(horaFinalCache);
                                    programacao->push_back(servico);
                                    programacao->push_back(provedor);
                                    programacao->push_back(sinopse);
                                    break;
                                }
                            }
                        }

                        //se false, é necessário preencher com algum elemento aleatório
                        if (checkpoint == false) {
                            checkpoint = false;

                            if (tmp.size() > 0) {
                                int aleat = getRandom(1, tmp.size());

                                int indice = tmp[aleat];
                                vector <si*>::iterator it = cacheInformation->begin();

                                for (int cont = 0; it != cacheInformation->end() && cont < indice; cont++)
                                    it++;

                                /* verifica se não existe duplicidade no aleatório */
                                p = find(programacao->begin(), programacao->end(), (*it)->getServiceName());

                                while (p != programacao->end()) {
                                    int aleat = getRandom(1, tmp.size());
                                    int indice = tmp[aleat];
                                    it = cacheInformation->begin();

                                    for (int cont = 0; (it != cacheInformation->end()) && (cont < indice); cont++)
                                        it++;

                                    p = find(programacao->begin(), programacao->end(), (*it)->getServiceName());
                                }


                                programacao->push_back((*it)->getDescriptor_conteudo_genero());
                                programacao->push_back((*it)->getDescriptor_conteudo_subgenero());
                                programacao->push_back((*it)->getStart_time());
                                programacao->push_back((*it)->getEnd_time());
                                programacao->push_back((*it)->getServiceName());
                                programacao->push_back((*it)->getServiceProviderName());
                                sinopse = (*it)->getDescriptor_event_short_sinopse();
                                programacao->push_back(sinopse);

                            }
                        }

                        /* limpa os vector para reaproveitar a instancia */
                        temporario.clear();
                        tmp_inicio.clear();
                        tmp_final.clear();
                        dtdTempI.clear();
                        tmeTempI.clear();
                        dtdTempF.clear();
                        tmeTempF.clear();

                        /* hora de inicio */
                        Utilitario::Tokenize(horaInicialCache, temporario, " ");
                        Utilitario::Tokenize(temporario[0], dtdTempI, "-");
                        Utilitario::Tokenize(temporario[1], tmeTempI, ":");

                        /* hora final */
                        temporario.clear();
                        Utilitario::Tokenize(horaFinalCache, temporario, " ");
                        Utilitario::Tokenize(temporario[0], dtdTempF, "-");
                        Utilitario::Tokenize(temporario[1], tmeTempF, ":");

                        /* Nesse ponto já temos a datainicio, datafinal tokenizadas
                         * Agora vamos implementar  as struct tm com essas
                         * informações
                         */

                        dt_inicio_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempI[0]), Utilitario::convertStringToInt(dtdTempI[1]),
                                Utilitario::convertStringToInt(dtdTempI[2]), Utilitario::convertStringToInt(tmeTempI[0]), Utilitario::convertStringToInt(tmeTempI[1]),
                                Utilitario::convertStringToInt(tmeTempI[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                        dt_fim_user = Utilitario::convertToStructTm(Utilitario::convertStringToInt(dtdTempF[0]), Utilitario::convertStringToInt(dtdTempF[1]),
                                Utilitario::convertStringToInt(dtdTempF[2]), Utilitario::convertStringToInt(tmeTempF[0]), Utilitario::convertStringToInt(tmeTempF[1]),
                                Utilitario::convertStringToInt(tmeTempF[2]), Utilitario::getWeekDayFromDate(dt_inicio_user));

                        struct tm checkFim = dt_inicio_user;
                        struct tm checkInicio = dt_inicio_user;
                        checkFim.tm_hour = 12;
                        checkFim.tm_min = 0;
                        checkFim.tm_sec = 0;

                        checkInicio.tm_hour = 6;
                        checkInicio.tm_min = 0;
                        checkInicio.tm_sec = 0;

                        /* verifica em qual horário do dia estamos */
                        if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                            retorno.push_back("MANHA");
                        }

                        checkFim.tm_hour = 18;
                        checkFim.tm_min = 0;
                        checkFim.tm_sec = 0;

                        checkInicio.tm_hour = 12;
                        checkInicio.tm_min = 0;
                        checkInicio.tm_sec = 1;

                        /* verifica em qual horário do dia estamos */
                        if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                            retorno.push_back("TARDE");
                        }

                        checkFim.tm_hour = 23;
                        checkFim.tm_min = 59;
                        checkFim.tm_sec = 59;

                        checkInicio.tm_hour = 18;
                        checkInicio.tm_min = 0;
                        checkInicio.tm_sec = 1;

                        /* verifica em qual horário do dia estamos */
                        if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                            retorno.push_back("NOITE");
                        }

                        checkFim.tm_hour = 05;
                        checkFim.tm_min = 59;
                        checkFim.tm_sec = 59;

                        checkInicio.tm_hour = 0;
                        checkInicio.tm_min = 0;
                        checkInicio.tm_sec = 0;

                        /* verifica em qual horário do dia estamos */
                        if ((Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user)) && (Utilitario::checkDates(checkInicio, checkFim, dt_inicio_user))) {
                            retorno.push_back("MADRUGADA");
                        }
                        return retorno;
                    }
                }
            }
        }
    }
}


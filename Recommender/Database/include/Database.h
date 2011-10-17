#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"

#include "sqlite3.h"

//#ifdef HAVE_RECOMMENDER
#include "recommender/utils/Utils.h"
#include "recommender/systemresource/SystemResource.h"
//#else
//#include "../../Utils/include/Utils.h"
//#include "../../SystemResource/include/SystemResource.h"
//#endif

using namespace br::ufscar::lince::ginga::recommender::util;

#ifdef HAVE_LOG4CXX
#include <log4cxx/logger.h>
using namespace log4cxx;
#endif

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    class Database {
                    private:
                        string user;
                        string password;
                        string database;
                        long maxSize;
                        ///Abstração para o banco de dados sqlite
                        sqlite3 *db;

#ifdef HAVE_LOG4CXX
                        LoggerPtr logger;
#endif

                    public:

                        /**
                         * Construtor.
                         * Devolve uma instância de Database.
                         * @param user nome de usuário do banco de dados.
                         * @param password senha do banco de dados.
                         * @param database nome do banco de dados para conexão ou criação.
                         */
                        Database(const char* User, const char* Password, const char* Database);

                        /**
                         * Destrutor.
                         */
                        ~Database();

                        /**
                         * Cria o banco de dados com o nome, usuário e senha especificados no construtor.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool createDatabase();

                        /**
                         * Apaga o banco de dados com o nome, usuário e senha especificados no construtor.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool deleteDatabase();

                        /**
                         * Fecha a conexão com o banco de dados especificado no construtor.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool closeDatabase();

                        /**
                         * Configura o tamanho máximo que o banco de dados poderá ocupar no disco.
                         * Deve ser acionado antes da chamada do método createDatabase().
                         * @param maxSize tamanho máximo (em bytes) que o banco de dados poderá ocupar no disco.
                         */
                        void setMaxSize(long MaxSize);

                        /**
                         * Retorna o tamanho máximo que o banco de dados pode ocupar no disco.
                         * @return tamanho máximo (em bytes) que o banco de dados pode ocupar no disco.
                         */
                        long getMaxSize();

                        /**
                         * Retorna o tamanho atual do banco de dados no disco.
                         * @return tamanho atual (em bytes) do banco de dados no disco.
                         */
                        long getCurrentSize();

                        /**
                         * Executa um comando SQL.
                         * @param sql comando SQL.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool query(const char* sql);

                        /**
                         * Executa um comando SQL.
                         * @param sql comando SQL.
                         * @param head variável para retorno do cabeçalho da consulta SQL.
                         * @param data variável para retorno dos dados da consulta SQL.
                         * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                         */
                        bool query(const char* sql, vector<string>* head, vector<string>* data);
                        //bool query(const char* sql, vector<string>* head, vector<vector<string>*>* data);
                    };

                }
            }
        }
    }
}

#endif /* _DATABASE_H_*/


#include "../include/Database.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    /**
                     * Construtor.
                     * Devolve uma instância de Database.
                     * @param user nome de usuário do banco de dados.
                     * @param password senha do banco de dados.
                     * @param database nome do banco de dados para conexão ou criação.
                     */
                    Database::Database(const char* User, const char* Password, const char* Database) {
                        user = User;
                        password = Password;
                        database = Database;
                        maxSize = 0;
                        db = NULL;
                    }

                    /**
                     * Destrutor.
                     */
                    Database::~Database() {
                        closeDatabase();
                        db = NULL;
                    }

                    /**
                     * Cria o banco de dados com o nome, usuário e senha especificados no construtor.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Database::createDatabase() {
                        return (sqlite3_open(database.c_str(), &db) == SQLITE_OK);
                    }

                    /**
                     * Apaga o banco de dados com o nome, usuário e senha especificados no construtor.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Database::deleteDatabase() {
                        closeDatabase();
                        remove(database.c_str());
                    }

                    /**
                     * Fecha a conexão com o banco de dados especificado no construtor.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Database::closeDatabase() {
                       if (db != NULL) {
                           sqlite3_close(db);	
                           db = NULL;
						   return true;
                       }

					   return false;
                    }

                    /**
                     * Configura o tamanho máximo que o banco de dados poderá ocupar no disco.
                     * Deve ser acionado antes da chamada do método createDatabase().
                     * @param maxSize tamanho máximo (em bytes) que o banco de dados poderá ocupar no disco.
                     */
                    void Database::setMaxSize(long MaxSize) {
                        maxSize = MaxSize;
                    }

                    /**
                     * Retorna o tamanho máximo que o banco de dados pode ocupar no disco.
                     * @return tamanho máximo (em bytes) que o banco de dados pode ocupar no disco.
                     */
                    long Database::getMaxSize() {
                        return maxSize;
                    }

                    /**
                     * Retorna o tamanho atual do banco de dados no disco.
                     * @return tamanho atual (em bytes) do banco de dados no disco.
                     */
                    long Database::getCurrentSize() {
                        struct stat file_stat;
                        int status, fildes;

                        fildes = open(database.c_str(), O_RDWR);
                        status = fstat(fildes, &file_stat);

                        return file_stat.st_size;
                    }

                    /**
                     * Executa um comando SQL.
                     * @param sql comando SQL.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Database::query(const char* sql) {
                        char *zErrMsg;
                        char **result;
                        int rc;
                        int nrow, ncol;
                        int db_open;
                        // std::vector<std::string> head, data;

                        rc = sqlite3_get_table(
                                db, /* Um database aberto */
                                sql, /* SQL para executar */
                                &result, /* resultado escrito para char *[]   */
                                &nrow, /* Numero de linhas do resultado */
                                &ncol, /* Numero de colunas do resultado */
                                &zErrMsg /* Error msg aqui */
                                );

                        if (rc != SQLITE_OK) 
                        /*if (head.size() < 0) {
                            head.clear();
                        }
                        if (data.size() < 0) {
                            data.clear();
                        }

                        if (rc == SQLITE_OK) {
                            for (int i = 0; i < ncol; ++i)
                                head.push_back(result[i]);
                            for (int i = 0; i < ncol * nrow; ++i)
                                data.push_back(result[ncol + i]);
                        }*/

                        sqlite3_free_table(result);

                        return (rc == SQLITE_OK);
                    }

                    /**
                     * Executa um comando SQL.
                     * @param sql comando SQL.
                     * @param head variável para retorno do cabeçalho da consulta SQL.
                     * @param data variável para retorno dos dados da consulta SQL.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Database::query(const char* sql, vector<string>* head, vector<string>* data) {
                    //bool Database::query(const char* sql, vector<string>* head, vector<vector<string>*>* data) {
                        char *zErrMsg = NULL;
                        char **result;
                        int rc;
                        int nrow, ncol;
                        int db_open;

                        rc = sqlite3_get_table(
                                db, /* Um database aberto */
                                sql, /* SQL para executar */
                                &result, /* resultado escrito para char *[]   */
                                &nrow, /* Numero de linhas do resultado */
                                &ncol, /* Numero de colunas do resultado */
                                &zErrMsg /* Error msg aqui */
                                );

                        if (rc == SQLITE_OK) {
                            if (head == NULL) {
                                head = new vector<string>;
                            }
                            if (data == NULL) {
                                data = new vector<string>;
                                //data = new vector<vector<string>*>;
                            }

                            if (head->size() > 0) {		// marco: era < 0 antes
                                head->clear();
                            }
                            if (data->size() > 0) {		// marco: era < 0 antes
                                data->clear();
                            }

                            for (int i = 0; i < ncol; ++i) {
                                head->push_back(result[i]);
                            }

                            for (int i = 0; i < ncol * nrow; ++i) {
                                data->push_back(result[ncol + i]);
                            }
                            
                        } else

                        sqlite3_free_table(result);

                        return (rc == SQLITE_OK);
                    }
                }
            }
        }
    }
}


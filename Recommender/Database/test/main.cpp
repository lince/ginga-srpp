#include <iostream>
#include "../include/Database.h"

using namespace br::ufscar::lince::ginga::recommender;

using namespace std;

int
main() {
    Database* db;

    db = new Database("", "", "banco.db");
	vector<string> head;
	vector<string> data;
    db->setMaxSize(100);
	cout << "MaxSize = " << db->getMaxSize() << endl;
	if (db->createDatabase()) {
		cout << "Banco criado com sucesso." << endl;
	}
	
    if (db->query("create table mytable (tablepk INTEGER PRIMARY KEY);")) {
		cout << "Tabela criada com sucesso." << endl;
    } else {
		cout << "Erro na criação da tabela." << endl;
	}

    if (db->query("insert into mytable (tablepk) VALUES (1)")) {
		cout << "Inserção executada com sucesso." << endl;
    } else {
		cout << "Erro na inserção de dados." << endl;
	}

    if (db->query("select * from mytable", &head, &data)) {
		cout << "Query executada com sucesso." << endl;
        cout << "Saída: " << head[0] << " " << data[0] << endl;
    } else {
		cout << "Erro na execução da query" << endl;
	}

	if (db->closeDatabase()) {
		cout << "Banco fechado com sucesso." << endl;
	} else {
		cout << "Não foi possível fechar o banco." << endl;
	}


    return 0;
}


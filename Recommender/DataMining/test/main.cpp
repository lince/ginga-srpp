#include <iostream>
#include <string>
#include <algorithm>
#include <string>

//#include <ginga/recommender/miningalgorithm/DataMining.h>
#include "../include/DataMining.h"

#define PREPARE_DATA 1
#define PREPARE_DB   0

#define RECOMMENDER_ALTO  3
#define RECOMMENDER_MEDIO 1
#define RECOMMENDER_BAIXO 1

/*
#include <ginga/recommender/database/Database.h>
#include <ginga/recommender/miningalgorithm/MiningAlgorithm.h>
 */

using namespace br::ufscar::lince::ginga::recommender;
using namespace std;


//XXX necessitam de métodos da interface IMiningAlgorithmApriori.
//Os métodos abaixo geram arquivos com as recomendações. Os programas recomendados são exportados na forma de um arquivo Lua.
bool parseInputFile(string fileName, int numeroRecomendacoes, vector<string>* programacao, IMiningAlgorithmApriori* miningAlgorithmApriori) {
    cout << "Lendo " << fileName << endl;

    //ReadFile: lê o arquivo texto passado como argumento do disco.
    miningAlgorithmApriori->ReadFile((char *) fileName.c_str());

    //getLine: retorna a linha lida.
    vector<string> ler = miningAlgorithmApriori->getLine();

    //Procura o código lido e imprime o horário do programa
    string genero_sub;
    vector<string> genvector;
    //vector<string> programacao;
    vector<string> tst;
    int numberRecommender = 0;

    //Laço para varrer os elementos lidos do arquivo "alto_noite.out".
    for (vector<string>::iterator it = ler.end(); it != ler.begin() && numberRecommender < numeroRecomendacoes;) {
        it--;
        numberRecommender++;
        //Obtém o gênero do elemento lido.
        genero_sub = *it;
        cout << genero_sub << endl;
        genvector.clear();

        //Obtém os tokens contidos em genero_sub e os armazena em genvector, utilizando espaços como delimitadores.
        Utilitario::Tokenize(genero_sub, genvector, " ");

        //Os valores obtido no método anterior é submetido ao método searchProgramById, que busca na tabela EIT o programa recomendado.
        //  genvector[0] : gênero do programa;
        //  genvector[1] : subgênero do programa;
        //  "NOITE" : dia que deve ser procurado na tabela;
        //  programacao : tabela EIT.
        tst = miningAlgorithmApriori->searchProgrambyId(genvector[0], genvector[1], "NOITE", programacao);
    }

    ler.clear();
    genvector.clear();
    tst.clear();

    return true;
}

bool createLuaRecommendationFile(string periodo, string outFileName, IMiningAlgorithmApriori* miningAlgorithmApriori) {
    vector<string> programacao;

    cout << "Lendo alto" + periodo + ".out" << endl;
    parseInputFile(("alto_" + periodo + ".out"), RECOMMENDER_ALTO, &programacao, miningAlgorithmApriori);

    cout << "Lendo medio" + periodo + ".out" << endl;
    parseInputFile(("medio_" + periodo + ".out"), RECOMMENDER_MEDIO, &programacao, miningAlgorithmApriori);

    cout << "Lendo pouco" + periodo + ".out" << endl;
    parseInputFile(("pouco_" + periodo + ".out"), RECOMMENDER_BAIXO, &programacao, miningAlgorithmApriori);

    // Corrigir bug quando não existe epg
    cout << "salvando noite.lua" << endl;

    string upperPeriodo = periodo;
    transform(upperPeriodo.begin(), upperPeriodo.end(), upperPeriodo.begin(), ::toupper);

    miningAlgorithmApriori->saveRecommender(outFileName, &programacao, upperPeriodo);
}


int
main() {
    system("rm madrugada.lua  manha.lua  noite.lua  tarde.lua");
    system("rm outDB.db setOutputTextFile.txt");

    cout << "   DataMining Test" << endl;

    //Obtém uma instância da interface IMiningAlgorithmApriori.
    DataMining* dataMining = new DataMining();

    cout << "Algoritmo inicial: " << dataMining->getAlgorithm() << endl;

    vector<char*>* algoritmosDisponiveis = dataMining->getAlgorithms();

    if (algoritmosDisponiveis != NULL) {
        cout << "Algoritmos disponíveis: ";
        for (int i = 0; i < algoritmosDisponiveis->size(); i++)
            cout << (*algoritmosDisponiveis)[i] << " ";
    }
    cout << endl;

    if (dataMining->setAlgorithm("Apriori"))
        cout << "Algoritmo escolhido: " << dataMining->getAlgorithm() << endl;
    else {
        cerr << "Não foi possível utilizar o algoritmo desejado." << endl;
        exit(1);
    }

    //Teste dp método setParams.
    vector<char*> params;
    params.push_back("0.1");
    params.push_back("0");
    params.push_back("cache.db");
    if (dataMining->setParams(params)) {
        cout << "Parâmetros ajustados com sucesso." << endl;
    } else {
        cerr << "Não foi possível utilizar o algoritmo desejado." << endl;
        exit(1);
    }

    //Teste dos métodos prepare.
	//Apenas um dos dois pode ser utilizado.
#if PREPARE_DATA
    //  Para variável:
	string inDataString;
	inDataString = "6|1|003217|HOJE EM DIA|2008-03-05 09:28:00|2008-03-05 12:59:00|0|211.0|6|1|004143|BALANCO GERAL|2008-03-05 13:00:00|2008-03-05 14:59:00|0|119.0|6|1|003676|TODO MUNDO ODEIA O CHRIS VES|2008-03-05 15:00:00|2008-03-05 15:43:00|0|43.0|6|1|003776|PROGRAMA DA TARDE|2008-03-05 15:44:00|2008-03-05 18:35:00|0|171.0|6|1|003316|SP RECORD|2008-03-05 18:36:00|2008-03-05 18:44:00|0|8.0|6|1|000622|FALA BRASIL|2008-03-06 08:52:00|2008-03-06 09:03:00|0|11.0|6|1|003217|HOJE EM DIA|2008-03-06 09:04:00|2008-03-06 09:26:00|0|22.0|4|1|000005|VIDEO SHOW|2008-03-06 14:20:00|2008-03-06 14:36:00|0|16.0|4|1|000006|VALE A PENA VER DE NOVO|2008-03-06 14:37:00|2008-03-06 15:05:00|0|28.0|4|1|002389|FLASH SP TV VIVO VES|2008-03-06 15:06:00|2008-03-06 15:18:00|0|12.0|4|1|000006|VALE A PENA VER DE NOVO|2008-03-06 15:19:00|2008-03-06 15:36:00|0|17.0|4|1|000007|SESSAO DA TARDE|2008-03-06 15:37:00|2008-03-06 17:31:00|0|114.0|4|1|001933|GLOBO NOTICIA VES|2008-03-06 17:32:00|2008-03-06 17:34:00|0|2.0|4|1|000008|MALHACAO|2008-03-06 17:35:00|2008-03-06 18:05:00|0|30.0|4|1|000008|NOVELA I|2008-03-06 18:06:00|2008-03-06 18:06:00|0|0.0|6|1|003776|PROGRAMA DA TARDE|2008-03-06 18:08:00|2008-03-06 18:35:00|0|27.0|6|1|003316|SP RECORD|2008-03-06 18:36:00|2008-03-06 19:01:00|0|25.0|6|1|000622|FALA BRASIL|2008-03-07 08:32:00|2008-03-07 09:04:00|0|32.0|6|1|003217|HOJE EM DIA|2008-03-07 09:05:00|2008-03-07 09:12:00|0|7.0|6|1|003776|PROGRAMA DA TARDE|2008-03-07 15:38:00|2008-03-07 16:18:00|0|40.0|6|1|002359|SAO PAULO NO AR|2008-03-08 07:28:00|2008-03-08 07:59:00|0|31.0|6|1|000622|FALA BRASIL|2008-03-08 08:00:00|2008-03-08 09:04:00|0|64.0|6|1|003217|HOJE EM DIA|2008-03-08 09:05:00|2008-03-08 09:25:00|0|20.0|6|1|003217|HOJE EM DIA|2008-03-09 10:15:00|2008-03-09 10:29:00|0|14.0|6|1|004143|BALANCO GERAL|2008-03-09 13:41:00|2008-03-09 14:44:00|0|63.0|6|1|003676|TODO MUNDO ODEIA O CHRIS VES|2008-03-09 14:45:00|2008-03-09 15:33:00|0|48.0|6|1|003776|PROGRAMA DA TARDE|2008-03-09 15:34:00|2008-03-09 16:59:00|0|85.0|2|1|003776|PINKY DINKY DOO VES|2008-03-09 17:00:00|2008-03-09 17:00:00|0|0.0|4|1|003776|SESSAO DA TARDE|2008-03-09 17:02:00|2008-03-09 17:02:00|0|0.0|2|1|003457|PINKY DINKY DOO VES|2008-03-09 17:04:00|2008-03-09 17:11:00|0|7.0|2|1|003457|MINUTO CIENTIFICO NOT|2008-03-09 18:25:00|2008-03-09 18:25:00|0|0.0|6|1|003776|PROGRAMA DA TARDE|2008-03-09 18:27:00|2008-03-09 18:33:00|0|6.0|6|1|003316|SP RECORD|2008-03-09 18:34:00|2008-03-09 19:18:00|0|44.0|6|1|000606|DESENHOS NOT|2008-03-09 19:19:00|2008-03-09 19:35:00|0|16.0|6|1|000328|DESENHOS VES|2008-03-10 14:38:00|2008-03-10 15:43:00|0|65.0|6|1|003314|O MELHOR DO BRASIL|2008-03-10 15:44:00|2008-03-10 20:08:00|0|264.0|6|1|000004|JORNAL DA RECORD|2008-03-10 20:09:00|2008-03-10 20:54:00|0|45.0|120|1|000004|JORNAL DA RECORD|2008-03-10 20:09:00|2008-03-10 20:54:00|0|45.0|6|1|003470|TUDO A VER NOT|2008-03-10 20:55:00|2008-03-10 21:07:00|0|12.0|6|1|002300|RECORD KIDS MAT|2008-03-11 13:03:00|2008-03-11 13:20:00|0|17.0|6|1|003204|TUDO E POSSIVEL|2008-03-11 15:39:00|2008-03-11 17:18:00|0|99.0|6|1|003550|SHOW DO TOM VES|2008-03-11 17:19:00|2008-03-11 18:56:00|0|97.0|6|1|003316|SP RECORD|2008-03-12 19:11:00|2008-03-12 19:19:00|0|8.0|6|1|000606|DESENHOS NOT|2008-03-12 19:20:00|2008-03-12 20:18:00|0|58.0|6|1|003776|PROGRAMA DA TARDE|2008-03-13 15:46:00|2008-03-13 18:09:00|0|143.0|6|1|000004|JORNAL DA RECORD|2008-03-13 20:56:00|2008-03-13 21:02:00|0|6.0|120|1|000004|JORNAL DA RECORD|2008-03-13 20:56:00|2008-03-13 21:02:00|0|6.0|6|1|003470|TUDO A VER NOT|2008-03-13 21:03:00|2008-03-13 21:05:00|0|2.0|6|1|003776|PROGRAMA DA TARDE|2008-03-14 18:10:00|2008-03-14 18:33:00|0|23.0|6|1|003316|SP RECORD|2008-03-14 18:34:00|2008-03-14 19:19:00|0|45.0|6|1|000606|DESENHOS NOT|2008-03-14 19:20:00|2008-03-14 19:33:00|0|13.0|6|1|004143|BALANCO GERAL|2008-03-15 14:24:00|2008-03-15 14:26:00|0|2.0|4|1|004143|VIDEO SHOW|2008-03-15 14:27:00|2008-03-15 14:27:00|0|0.0|7|1|004143|CINEMA EM CASA|2008-03-15 14:29:00|2008-03-15 14:29:00|0|0.0|6|1|004143|BALANCO GERAL|2008-03-15 14:31:00|2008-03-15 14:31:00|0|0.0|7|1|000003|CINEMA EM CASA|2008-03-15 14:33:00|2008-03-15 14:36:00|0|3.0|4|1|000003|VIDEO SHOW|2008-03-15 14:37:00|2008-03-15 14:37:00|0|0.0|4|1|000003|VALE A PENA VER DE NOVO|2008-03-15 14:39:00|2008-03-15 14:39:00|0|0.0|6|1|000003|BALANCO GERAL|2008-03-15 14:41:00|2008-03-15 14:41:00|0|0.0|4|1|000006|VALE A PENA VER DE NOVO|2008-03-15 14:43:00|2008-03-15 14:49:00|0|6.0|6|1|003676|TODO MUNDO ODEIA O CHRIS VES|2008-03-15 14:50:00|2008-03-15 14:53:00|0|3.0|4|1|003676|VALE A PENA VER DE NOVO|2008-03-15 14:54:00|2008-03-15 14:54:00|0|0.0|6|1|003676|TODO MUNDO ODEIA O CHRIS VES|2008-03-15 14:56:00|2008-03-15 15:28:00|0|32.0|6|1|003776|PROGRAMA DA TARDE|2008-03-15 15:29:00|2008-03-15 16:48:00|0|79.0|4|1|000007|SESSAO DA TARDE|2008-03-15 17:22:00|2008-03-15 17:31:00|0|9.0|6|1|003776|PROGRAMA DA TARDE|2008-03-15 17:32:00|2008-03-15 18:34:00|0|62.0|6|1|003316|SP RECORD|2008-03-15 18:35:00|2008-03-15 19:17:00|0|42.0|6|1|000606|DESENHOS NOT|2008-03-15 19:18:00|2008-03-15 20:19:00|0|61.0|6|1|000004|JORNAL DA RECORD|2008-03-15 20:20:00|2008-03-15 20:28:00|0|8.0|120|1|000004|JORNAL DA RECORD|2008-03-15 20:20:00|2008-03-15 20:28:00|0|8.0|1|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|2|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|4|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|5|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|6|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|7|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|13|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|20|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|120|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|122|1|000210|HORARIO POLITICO NOT|2008-03-15 20:29:00|2008-03-15 20:37:00|0|8.0|6|1|000210|HOJE EM DIA|2008-03-16 11:33:00|2008-03-16 11:33:00|0|0.0|6|1|004143|BALANCO GERAL|2008-03-16 13:13:00|2008-03-16 13:20:00|0|7.0|6|1|003776|PROGRAMA DA TARDE|2008-03-16 17:02:00|2008-03-16 18:34:00|0|92.0|6|1|003316|SP RECORD|2008-03-16 18:35:00|2008-03-16 19:17:00|0|42.0|6|1|000606|DESENHOS NOT|2008-03-16 19:18:00|2008-03-16 20:19:00|0|61.0|6|1|000004|JORNAL DA RECORD|2008-03-16 20:20:00|2008-03-16 20:52:00|0|32.0|120|1|000004|JORNAL DA RECORD|2008-03-16 20:20:00|2008-03-16 20:52:00|0|32.0|6|1|001730|FALA BRASIL ESPECIAL|2008-03-17 09:34:00|2008-03-17 09:44:00|0|10.0|120|1|001730|FALA BRASIL ESPECIAL|2008-03-17 09:34:00|2008-03-17 09:44:00|0|10.0|6|1|004083|HOJE EM DIA MAT|2008-03-17 09:45:00|2008-03-17 10:42:00|0|57.0|120|1|004083|HOJE EM DIA MAT|2008-03-17 09:45:00|2008-03-17 10:42:00|0|57.0|4|1|000672|CALDEIRAO DO HUCK|2008-03-17 15:25:00|2008-03-17 15:47:00|0|22.0|5|1|000672|CAMPEONATO BRASILEIRO SERIE B|2008-03-17 15:48:00|2008-03-17 15:48:00|0|0.0|4|1|000672|CALDEIRAO DO HUCK|2008-03-17 15:50:00|2008-03-17 15:52:00|0|2.0|1|1|000900|FUTEBOL VES|2008-03-17 15:53:00|2008-03-17 17:59:00|0|126.0|4|1|000900|FUTEBOL VES|2008-03-17 15:53:00|2008-03-17 17:59:00|0|126.0|6|1|000900|FUTEBOL VES|2008-03-17 15:53:00|2008-03-17 17:59:00|0|126.0|120|1|000900|FUTEBOL VES|2008-03-17 15:53:00|2008-03-17 17:59:00|0|126.0|4|1|001933|GLOBO NOTICIA VES|2008-03-17 18:00:00|2008-03-17 18:07:00|0|7.0|4|1|000009|NOVELA I|2008-03-17 18:08:00|2008-03-17 18:27:00|0|19.0|6|1|003314|O MELHOR DO BRASIL|2008-03-17 18:28:00|2008-03-17 18:36:00|0|8.0|4|1|000009|NOVELA I|2008-03-17 18:37:00|2008-03-17 18:39:00|0|2.0|1|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|2|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|4|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|5|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|6|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|9|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|20|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|122|1|001044|RELIGIOSO MAT|2008-03-18 06:30:00|2008-03-18 06:45:00|0|15.0|7|1|001044|ATAQUE DE RISOS DM 3|2008-03-18 06:46:00|2008-03-18 06:46:00|0|0.0|1|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|2|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|4|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|5|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|6|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|9|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|20|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|122|1|000919|RELIGIOSO MAT|2008-03-18 06:48:00|2008-03-18 07:00:00|0|12.0|4|1|000919|ANTENA PAULISTA|2008-03-18 07:01:00|2008-03-18 07:01:00|0|0.0|1|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|2|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|4|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|5|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|6|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|9|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|20|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|122|1|001044|RELIGIOSO MAT|2008-03-18 07:03:00|2008-03-18 07:05:00|0|2.0|4|1|001044|ANTENA PAULISTA|2008-03-18 07:06:00|2008-03-18 07:06:00|0|0.0|7|1|001044|PESCALTERNATIVA|2008-03-18 07:08:00|2008-03-18 07:08:00|0|0.0|4|1|000525|ANTENA PAULISTA|2008-03-18 07:10:00|2008-03-18 07:18:00|0|8.0|4|1|000122|SPTV 1A EDICAO|2008-03-19 12:10:00|2008-03-19 12:45:00|0|35.0|4|1|000003|GLOBO ESPORTE|2008-03-19 12:46:00|2008-03-19 13:14:00|0|28.0|4|1|000004|JORNAL HOJE|2008-03-19 13:15:00|2008-03-19 13:45:00|0|30.0|4|1|000005|VIDEO SHOW|2008-03-19 13:46:00|2008-03-19 14:37:00|0|51.0|4|1|000006|VALE A PENA VER DE NOVO|2008-03-19 14:38:00|2008-03-19 15:41:00|0|63.0|4|1|000007|SESSAO DA TARDE|2008-03-19 15:42:00|2008-03-19 16:39:00|0|57.0";
    char* inData = new char[inDataString.size()];
	strcpy(inData, inDataString.c_str());
    char* inDataDelimiter = new char;
    strcpy(inDataDelimiter, "|");
    dataMining->prepare(inData, inDataDelimiter);
#elif PREPARE_DB
    //  Para banco de dados:
    Database* inDB = new Database("", "", "user.db");
    if (inDB->createDatabase())
		cout << "Banco de Dados de entrada de dados criado com sucesso." << endl;
/*
    if (inDB->query("CREATE TABLE HISTORICO (CODE INT, KEY VARCHAR(3), DOC_ID VARCHAR(30),FILENAME VARCHAR(256),DATA DATETEXT, HOUR DATETEXT, WEEKDAY VARCHAR (1), TIME REAL)"))
		cout << "Tabela para o BD de entrada criada com sucesso." << endl;
*/
    string inTableName = "HISTORICO";
    //string inTableRowNames = "CODE,KEY,DOC_ID,FILENAME,DATA,HOUR,WEEKDAY,TIME";
    string inTableInsertionQuery = inTableName;// + inTableRowNames;
    dataMining->prepare(inDB, inTableInsertionQuery.c_str());
#endif

    //Teste dos métodos setOutput.
    //  Para arquivo:
    dataMining->setOutput("setOutputTextFile.txt");

    //  Para variável:
    long outDataSize = 100;
    char* outData = new char[outDataSize];
    char* outDataDelimiter = new char[2];
    strcpy(outDataDelimiter, "||");
    dataMining->setOutput(outData, outDataSize, outDataDelimiter);

    //  Para banco de dados:
    Database* outDB = new Database("", "", "outDB.db");
    if (outDB->createDatabase())
		cout << "Banco de Dados para saída de dados criado com sucesso." << endl;
    if (outDB->query("create table genero (gen VARCHAR)"))
		cout << "Tabela para o DB de saída criada com sucesso." << endl;
    string outTableName = "genero";
    string outTableRowNames = " (gen)";
    string OutTableInsertionQuery = outTableName + outTableRowNames;
    dataMining->setOutput(outDB, OutTableInsertionQuery.c_str());

    //Teste do método start.
    dataMining->start();

    //Teste do método stop.
    dataMining->stop();

    //Final da execução do algoritmo.
    cout << "Retorno de setOutput (variável): " << outData << endl;

    vector<string> outDBhead;
    vector<string> outDBdata;
    string selectQuery = "select * from " + outTableName;
    cout << "Retorno de setOutput (banco de dados): ";
    if (outDB->query(selectQuery.c_str(), &outDBhead, &outDBdata))
        for (int i = 0; i < outDBdata.size(); i++)
            cout << outDBdata[i] << " ";
    cout << endl;

    //Tratamento da saída - Paulo:
    //ler arquivos gerados (módulo datamining)

    cout << "mining complete !!!" << endl << "Lendo arquivos gerados " << endl;


    // Corrigir bug quando não existe epg
    createLuaRecommendationFile("noite", "noite.lua", (IMiningAlgorithmApriori*)dataMining->miningAlgorithm);
    createLuaRecommendationFile("tarde", "tarde.lua", (IMiningAlgorithmApriori*)dataMining->miningAlgorithm);
    createLuaRecommendationFile("manha", "manha.lua", (IMiningAlgorithmApriori*)dataMining->miningAlgorithm);
    createLuaRecommendationFile("madrugada", "madrugada.lua", (IMiningAlgorithmApriori*)dataMining->miningAlgorithm);


	//Desalocação de memória.
#if PREPARE_DATA
    delete[] inData;
    delete[] inDataDelimiter;
#elif PREPARE_DB
    if (inDB->closeDatabase()) {
    };
#endif
    if (outDB->closeDatabase()) {
	};

    delete[] outData;
    delete[] outDataDelimiter;
    //delete dataMining;

    return 0;
}


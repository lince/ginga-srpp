/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/


#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
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
          namespace util {
            /**
            * Fornecer suporte de conversões e operações com datas
            * @author Paulo Muniz de Ávila
            */
            class Utilitario {
            public:
              /**
              * Tokenizar uma string em várias substrings.
              * @param str string a ser tokenizada.
              * @param tokens vector onde os tokens são armazenados.
              * @param delimiters delimitador para tokenizar. Ex: espaço ou '-'.
              * @code
              * vector<string> tokens;
              *
              * string str("Split me up! Word1 Word2 Word3.");
              *
              * Tokenize(str, tokens);
              *
              * copy(tokens.begin(), tokens.end(), ostream_iterator<string>(cout, ", "));
              *
              */
              static void Tokenize(const std::string& str,std::vector<std::string>& tokens,const std::string& delimiters);

              /**
              * Converter um valor inteiro para string.
              * @param valor int para ser convertido.
              * @return <b> string </b> convertida.
              */
              static std::string convertIntToString(int valor);

              /**
              * Converte um valor string para um tipo int.
              * @param strConvert string para ser convertido.
              * @return <b> int </b> convertido.
              */
              static int convertStringToInt(std::string strConvert);
              /**
              * Verifica se uma data e hora está dentro de um determinado intervalo
              * @param time1 o valor inicial do intervalo
              * @param time2 o valor final do intervalo
              * @param check o valor a ser verificado.
              * @return <b> 1 </b> se a data/hora do parametro check está no intervalo, <b> 0 </b> caso contrário.
              */
              static bool checkDates( struct tm time1, struct tm time2, struct tm check);

              /**
              * Criar uma struct tm utilizada por várias funções de manipulação de data/hora
              * @param year int que representa o ano
              * @param month int que representa o mês [1-12]
              * @param day int que representa o dia do mês [1-31]
              * @param hour int que representa as horas [0-24]
              * @param minute int que representa os minutos [0-60]
              * @param seconds int que representa os segundos [0-60]
              * @param weekday int que representa o dia da semana [1-7]
              * @return struct tm representando a data/hora passada por parametro.
              */
              static struct tm convertToStructTm( int year, int month, int day, int hour, int minute, int seconds, int weekday);

              /**
              * Realiza manipulação com data/hora. Ex: somar 10 minutos a data/hora atual.
              * @param incr <b>struct tm</b> que contém a data a ser manipulada.
              * @param tempo <b> int </b> para incrementar. Use valor negativo para decrementar a data/hora.
              * @param campo <b> char </b> que representa: d=dia,m=mês,y=ano,h=hora,t=minuto,n=segundos,w=dia da semana.
              * @return <b> struct tm </b> que representa a data/hora alterada.
              */
              static struct tm incrementDateTime(struct tm incr, int segundos, const char campo = 'n');

              /**
               * Retorna o dia da semana de uma data/hora.
               * @param datetime <b> struct tm </b> com a data/hora
               * @return <b> int </b> que representa o dia da semana.
               */
              static int getWeekDayFromDate(struct tm datetime);

              /**
               * Calcula o tempo de duração de um programa.
               * @param time1 <b> struct tm </b> que contém a data inicial
               * @param time2 <b> struct tm </b> que contém a data final
               * @return <b> double </b> com os segundos de duração do programa
               */
              static double diferenceDates(struct tm * time1, struct  tm  * time2);

              static int getDayNow();

              static int getMonthNow();

              static int getYearNow();

              /**
               * Converte de String para Float
               * @param s string
               * @return double
               */
              static double stod(string s);

			  /// retorna uma instância da classe para utilização dos serviços oferecidos
			  static Utilitario* getInstance();
			  /// retorna a hora local
			  static string getHora();
			  /// retorna os minutos da hora local
			  static string getMinuto();
			  /// retorna os segundos da hora local
			  static string getSegundo();
			  /// retorna o dia de acordo com a data atual do set-top box
			  static string getDia();
			  /// retorna o mês de acordo com  data atual do set-top box
			  static string getMes();
			  /// retorna o ano de acordo com a data atual do set-top box
			  static string getAno();
			  /// retorna o dia semana, sendo que domingo = 1;
			  static string getDiaSemana();
			  /// retorna o tempo que o usuário ficou em um determinado canal sem interação com o controle.
			  static double calculaTempo(struct tm* raw);
			  /// define o horário em que o set-top box foi ligado
			  static void setTimeStart(time_t ts);
			  /// retorna o horário em que o set-top box foi ligado
			  static time_t getTimeStart();
			  /// retorna uma struct tm* com a data formatada de acordo com o sistema
			  /// operacional instalado no set-top box
			  static struct tm* getObterData();
			  /// função para converter um valor do tipo interiro em uma string
			  static string IntToString(int intValue);

              ///Construtor.
			  Utilitario();
            private:
			  /// variavel do padrão singleton.
			  static Utilitario*  _instance;
			  /// define o valor inicial do ano 1900
			  static int const ANO_INICIO = 1900;

			  static time_t tempo_start;
            };
          }
        }
      }
    }
  }
}
#endif /* _UTILS_H_ */

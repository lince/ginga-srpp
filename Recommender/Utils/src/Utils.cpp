/* Copyright (C) Paulo Muniz de Ávila <avilapm@gmail.com> 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../include/Utils.h"

using namespace br::ufscar::lince::ginga::recommender::util;

namespace br {
  namespace ufscar {
    namespace lince {
      namespace ginga {
        namespace recommender {
          namespace util {
			  /// variavel do padrão singleton.
			  Utilitario*  _instance;
			  /// define o valor inicial do ano 1900
			  int const ANO_INICIO = 1900;

			  time_t tempo_start;

			Utilitario::Utilitario() {
				//LinceUtil.h

				//inicializa as variaveis//
				_instance = NULL;
				tempo_start = 0;


				//utils.cpp
				#ifdef HAVE_LOG4CXX
							  LoggerPtr logger;
							  logger = LoggerPtr(Logger::getLogger("recommender"));
							  LOG4CXX_INFO(logger, "utils::Tokenize");
				#endif
			}

            void Utilitario::Tokenize(const std::string& str,std::vector<std::string>& tokens,
                const std::string& delimiters = " ")
            {
              std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

              std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

              while (std::string::npos != pos || std::string::npos != lastPos)
                {

                tokens.push_back(str.substr(lastPos, pos - lastPos));

                lastPos = str.find_first_not_of(delimiters, pos);

                pos = str.find_first_of(delimiters, lastPos);
                }
            }



            std::string Utilitario::convertIntToString(int valor){
#ifdef HAVE_LOG4CXX
              LoggerPtr logger;
              logger = LoggerPtr(Logger::getLogger("recommender"));
              LOG4CXX_INFO(logger, "utils::convertIntToString");
#endif
              std::string s;
              std::stringstream out;
              out << valor;
              s = out.str();
              return s;
            }

            int Utilitario::convertStringToInt(std::string strConvert) {
#ifdef HAVE_LOG4CXX
              LoggerPtr logger;
              logger = LoggerPtr(Logger::getLogger("recommender"));
              LOG4CXX_INFO(logger, "utils::convertStringToInt");
#endif
              int intReturn;
              intReturn = atoi(strConvert.c_str());
              return(intReturn);
            }

            double Utilitario::diferenceDates(struct tm * time1, struct tm * time2){
              time_t start,end;
              double tempo;

              struct tm * timeinfo = time1;
              struct tm * timeinfo1 = time2;

              //converte para o tipo time_t
              start = mktime(timeinfo);
              end = mktime(timeinfo1);


              //se true, mktime falhou então retorna tempo 0
              if(end == -1 || start == -1 ) {
                return 0;
              }

              tempo =  difftime(end ,start) ;
              return (tempo);
            }

            bool Utilitario::checkDates(struct tm time1, struct tm time2, struct tm check){
#ifdef HAVE_LOG4CXX
              LoggerPtr logger;
              logger = LoggerPtr(Logger::getLogger("recommender"));
              LOG4CXX_INFO(logger, "utils::checkDates");
#endif

              time_t start,end,ch;

              //converte para o tipo time_t
              start = mktime(&time1);
              end = mktime(&time2);
              ch = mktime(&check);

              //se true, mktime falhou então retorna tempo 0
              if(end == -1 || start == -1 || ch == -1) {
                return 0;
              }

#ifdef HAVE_LOG4CXX
              logger = LoggerPtr(Logger::getLogger("recommender"));
              LOG4CXX_INFO(logger,ctime(&start));
              LOG4CXX_INFO(logger,ctime(&end));
              LOG4CXX_INFO(logger,ctime(&ch));
#endif

              if( (difftime(ch,start)>= 0) && (difftime(ch,end)<=0) )
                return true;


              return false;
            }

            struct tm Utilitario::convertToStructTm(int year, int month, int day, int hour, int minute, int seconds, int weekday){
#ifdef HAVE_LOG4CXX
              LoggerPtr logger;
              logger = LoggerPtr(Logger::getLogger("recommender"));
              LOG4CXX_INFO(logger, "utils::convertToStructTm");
#endif
              struct tm tme;
              year = year - 1900 ;
              month = month-1;
              tme.tm_year = year;
              tme.tm_mon = month;
              tme.tm_mday = day;
              tme.tm_hour = hour;
              tme.tm_min = minute;
              tme.tm_sec = seconds;
              tme.tm_wday = weekday;
              tme.tm_isdst = -1;


              return tme;
            }

            struct tm Utilitario::incrementDateTime(struct tm incr, int tempo, const char campo ){
#ifdef HAVE_LOG4CXX
              LoggerPtr logger;
              logger = LoggerPtr(Logger::getLogger("recommender"));
              LOG4CXX_INFO(logger, "utils::incrementDateTime");
#endif
              struct tm temp = incr;
              switch (campo) {
              case 'd':
                temp.tm_mday += tempo;
                break;
              case 'm':
                temp.tm_mon += tempo;
                break;
              case 'y':
                temp.tm_year += tempo ;
                break;
              case 'h':
                temp.tm_hour += tempo;
                break;
              case 't':
                temp.tm_min += tempo;
                break;
              case 'n':
                temp.tm_sec += tempo;
                break;
              case 'w':
                temp.tm_wday += tempo;
                break;

              default:
                temp.tm_sec += tempo;
                break;
              }


              time_t t = mktime(&temp);
              return *(localtime(&t));
            }

            int Utilitario::getWeekDayFromDate(struct tm datetime) {
              return datetime.tm_wday;
            }

            int Utilitario::getDayNow() {

              time_t rawtime;
              struct tm * timeinfo;

              time ( &rawtime );
              timeinfo = localtime ( &rawtime );
              return timeinfo->tm_mday;

            }

            int Utilitario::getMonthNow() {
              time_t rawtime;
              struct tm * timeinfo;

              time ( &rawtime );
              timeinfo = localtime ( &rawtime );
              return timeinfo->tm_mon + 1;
            }

            int Utilitario::getYearNow() {
              time_t rawtime;
              struct tm * timeinfo;

              time ( &rawtime );
              timeinfo = localtime ( &rawtime );
              return timeinfo->tm_year + 1900;
            }

            double Utilitario::stod(string s) {
              double f;
              istringstream ss(s);
              ss >> f;
              return f;
            }

//LinceUtil.h

//inicializa as variaveis//
Utilitario* Utilitario::_instance = NULL;
time_t Utilitario::tempo_start = 0;


Utilitario* Utilitario::getInstance() {
	if (Utilitario::_instance == NULL) {
		Utilitario::_instance = new Utilitario();
	}
	return Utilitario::_instance;
}


string Utilitario::getHora() {
	if(getObterData()->tm_isdst > 0) {
		return IntToString(getObterData()->tm_hour + 1);
	}
	return IntToString(getObterData()->tm_hour);
}


string Utilitario::getMinuto() {
	return IntToString(getObterData()->tm_min);
}


string Utilitario::getSegundo(){
	return IntToString(getObterData()->tm_sec);
}

string Utilitario::getDia(){
	return IntToString(getObterData()->tm_mday);
}


string Utilitario::getMes(){
	return IntToString(getObterData()->tm_mon + 1);
}


string Utilitario::getAno(){
	return IntToString(ANO_INICIO + getObterData()->tm_year);
}


string Utilitario::getDiaSemana(){
	return IntToString(getObterData()->tm_wday);
}




struct tm* Utilitario::getObterData() {
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return timeinfo;
}


double Utilitario::calculaTempo(struct tm* raw) {
	time_t start,end;
	double tmp = 0;

	//converte para o tipo time_t
	start = Utilitario::getTimeStart();
	end = mktime(raw);

	wclog << "start time = " << ctime(&start) << endl;
	wclog << "end time = " << ctime(&end) << endl;

	//se true, mktime falhou então retorna tempo 0
	if(end == -1 || start == -1) {
		wclog << "mktime return -1" << endl;
		return 0;
	}

	tmp = difftime(end,start);
	wclog << "Detectado interacao usuario/set-top-Box " << tmp << " segundos" << endl;

	//setar o novo valor de start//
	Utilitario::setTimeStart(end);
	return tmp;
}


/**
* Define o tempo inicial da interação do usuário
*/
void Utilitario::setTimeStart(time_t ts) {
	if(ts == 0) {
		ts = Utilitario::getTimeStart();
	}

	Utilitario::tempo_start=ts;
}


time_t Utilitario::getTimeStart() {
	if(Utilitario::tempo_start == 0)
	{
		return time(&tempo_start);
	}
	return Utilitario::tempo_start;
}

string Utilitario::IntToString(int intValue) {

	std::string s;
	std::stringstream out;
	if(intValue < 10)
		out << "0" << intValue;
	else
		out << intValue;
	s = out.str();
	return s;

}

          }
        }
      }
    }
  }
}



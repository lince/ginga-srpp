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


#ifndef _IMININGALGORITHMAPRIORIUSER_H_
#define _IMININGALGORITHMAPRIORIUSER_H_

#include <string>

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

                    /** Class User
                     *  Representa todas as informações coletadas a partir da interação do
                     *  usuário com o set-top box. Exemplo: codigo da tecla pressionada,
                     *  nome do programa sendo assistido, hora e data.
                     *  @author Paulo Muniz de Ávila
                     */
                    class User {
                    private:
                        int code;
                        string keycode;
                        string doc_id;
                        string filename;
                        string hour;
                        string minute;
                        string second;
                        string day;
                        string month;
                        string year;
                        string dayweek;
                        double time;
#ifdef HAVE_LOG4CXX
                        LoggerPtr logger;
#endif

                    public:
                        User();
                        virtual ~User();

                        /**
                         * retorna o identificador do provedor de serviços
                         * @return um int que representa o provedor de serviço
                         */
                        int getCode();

                        /**
                         * Armazena o provedor de serviços sintonizado no objeto da classe user
                         * @param setcode o código que representa o provedor de serviço
                         */
                        void setCode(int setcode);

                        /**
                         * Retorna a tecla pressionada pelo usuário
                         * @return string informando a tecla
                         * @see codemap
                         */
                        string getKey();

                        /**
                         * Armazena a tecla pressionada pelo usuário no objeto da classe User
                         * @param setKey string que representa a tecla
                         * @see codemap
                         */
                        void setKey(string setKey);

                        /**
                         * Retorna o identificador do serviço sintonizado
                         * @return string que representa o serviço (canal no sistema analógico)
                         */
                        string getDocid();

                        /**
                         * Armazena o serviço sintonizado no objeto da classe User
                         * @param setdocid string que identifica o serviço sintonizado
                         */
                        void setDocid(string setdocid);

                        /**
                         * Retorna o nome do programa sintonizado pelo usuário
                         * @return string que representa o nome do programa.
                         * <b> Tabela EIT, descritor de conteúdo </b>
                         */
                        string getFilename();

                        /**
                         * Armazena o nome do programa sintonizado no objeto da classe User
                         * @param setfilename nome do programa obtido da tabela EIT
                         */
                        void setFilename(string setfilename);

                        /**
                         * Retorna a hora atual no set-top box
                         * @return string informando a hora de 0 a 24.
                         */
                        string getHour();

                        /**
                         * Armazena a hora atual do set-top box no objeto da classe User
                         * @param sethour string representando a hora de 0 a 24.
                         */
                        void setHour(string sethour);

                        /**
                         * Retorna os minutos da hora atual do set-top box
                         * @return string informando o minuto de 0 a 59.
                         */
                        string getMinute();

                        /**
                         * Armazena os minutos atuais do set-top box no objeto da classe User
                         * @param setminute string representando os minutos de 0 a 59.
                         */
                        void setMinute(string setminute);

                        /**
                         * Retorna os segundos da hora atual do set-top box
                         * @return string representando os segundos de 0 a 59.
                         */
                        string getSecond();

                        /**
                         * Armazena os segundos atuais da hora do set-top box no objeto da classe User
                         * @param setsecond string representando os segundos de 0 a 59
                         */
                        void setSecond(string setsecond);

                        /**
                         * Retorna o dia de acordo com a data setada no set-top box
                         * @return string que representa os dias de 1 a 31.
                         */
                        string getDay();

                        /**
                         * Armazena o dia de acordo com a data atual do set-top Box no objeto da classe User
                         * @param setday string que representa o dia atual de 1 a 31
                         */
                        void setDay(string setday);

                        /**
                         * Retorna o mês atual de acordo com a data no set-top box
                         * @return string que representa o mês de 1 a 12.
                         */
                        string getMonth();

                        /**
                         * Armazena o mês de acordo com a data atual do set-top Box no objeto da classe User
                         * @param setmonth string que representa o mês de 1 a 12.
                         */
                        void setMonth(string setmonth);

                        /**
                         * Retorna o ano atual de acordo com a data no set-top box
                         * @return string que informa o ano de 1900-2027.
                         */
                        string getYear();

                        /**
                         * Armazena o ano de acordo com a data atual do set-top Box no objeto da classe User
                         * @param setyear string que representa o ano de 1900-2027
                         */
                        void setYear(string setyear);

                        /**
                         * Retorna o dia da semana. Ex: domingo = 1, segunda = 2, ...
                         * @return string que representa o dia da semana 1 a 7.
                         */
                        string getWeekDay();

                        /**
                         * Armazena o dia da semana da data atual do set-top-box no objeto da classe User
                         * @param setweekday string que representa o dia da semana de 1 a 7
                         */
                        void setWeekDay(string setweekday);

                        /**
                         * Retorna o tempo que um determinado serviço ficou sintonizado
                         * @return double que representa os segundos que um determinado serviço
                         * ficou sintonizado
                         */
                        double getTimeDuration();

                        /**
                         * Armazena o tempo que um determinado serviço ficou sintonizado no objeto
                         * da classe User
                         * @param settimeduration double representando o tempo em segundos
                         */
                        void setTimeDuration(double settimeduration);


                        /**
                         * Retorna uma string com a data formatada como dd-mm-yyyy
                         * @return string com  data preparada para inserção no banco sqlite
                         */
                        string getFormatDate();

                        /**
                         * Retorna uma string com a hora formatada como hh:mm:ss
                         * @return string com a hora preparada para inserção no banco sqlite
                         */
                        string getFormatTime();

                    };
                }
            }
        }
    }
}

#endif /* _IMININGALGORITHMAPRIORIUSER_H_ */


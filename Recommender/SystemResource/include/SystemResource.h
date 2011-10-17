#ifndef _SYSTEMRESOURCE_H_
#define _SYSTEMRESOURCE_H_

extern "C" {
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
//#include <linux/falloc.h> //Not working with libc at the moment.
//Instead:
#define _XOPEN_SOURCE 600 //required for posix_fallocate
}

//#ifdef HAVE_RECOMMENDER
#include <ginga/recommender/utils/Utils.h>
/*#else
#include "../../Utils/include/Utils.h"
#endif
*/

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

using namespace br::ufscar::lince::ginga::recommender::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    class SystemResource {
                    public:

                        /**
                         * Retorna a frequência da CPU.
                         * @return frequência da CPU em Hz.
                         */
                        long getCPUClock();

                        /**
                         * Retorna quantidade de CPUs da máquina.
                         * @return quantidade de CPUs da máquina.
                         */
                        int getCPUCount();

                        /**
                         * Retorna a quantidade total de memória da máquina.
                         * @return total de memória em bytes.
                         */
                        long getMemorySize();

                        /**
                         * Retorna a quantidade de memória livre na máquina.
                         * @return total de memória livre (em bytes).
                         */
                        long getMemoryFree();

                        /**
                         * Retorna a quantidade total de espaço de armazenamento da máquina.
                         * @return total de espaço de armazenamento da máquina (em bytes).
                         */
                        off_t getDiskSize();

                        /**
                         * Retorna a quantidade de espaço de armazenamento livre da máquina.
                         * @return espaço de armazenamento livre da máquina (em bytes).
                         */
                        off_t getDiskFree();

                        /**
                         * Reserva um recurso do sistema para uma aplicação.
                         * @param applicationId identificação da aplicação que solicita o recurso.
                         * @param resource identificação do recurso solicitado: 1 - CPU; 2 - Memória; 3 - Disco.
                         * @param count quantidade do recurso solicitada.
                         * O retorno deste método devolve um identificador para que o recurso seja liberado mais tarde.
                         * @return maior que 0, se a operação foi executada com sucesso, ou -1, caso contrário.
                         */
                        int reserve(const char* applicationId, int resource, long count);

                        /**
                         * Desfaz uma reserva de recurso do sistema.
                         * @param applicationId identificação da aplicação que solicitou o recurso.
                         * @param reserveId identificador do recurso que será liberado.
                         * @return true, se a operação foi executada com sucesso, ou false, caso contrário.
                         */
                        bool freeReserve(const char* applicationId, int reserveId);

                        /**
                         * Faz uso de um recurso do sistema reservado anteriormente.
                         * @param applicationId identificação da aplicação que solicitou o recurso.
                         * @param reserveId identificador do recurso reservado.
                         * @return true, se a operação foi executada com sucesso, ou false, caso contrário.
                         */
                        bool allocate(const char* applicationId, int reserveId);

                        /**
                         * Libera o uso de um recurso do sistema.
                         * @param applicationId identificação da aplicação que solicitou o recurso.
                         * @param reserveId identificador do recurso reservado.
                         * @return true, se a operação foi executada com sucesso, ou false, caso contrário.
                         */
                        bool freeAllocation(const char* applicationId, int reserveId);

                        /**
                         * Construtor.
                         */
                        SystemResource();

                        /**
                         * Destrutor.
                         */
                        ~SystemResource();

                    private:
                        /**
                         * Obtém os valores dos atributos da classe relativos à CPU.
                         */
                        void initDataMembers();

                        struct utsname sn;
                        struct sysinfo info;
                        float CPUClock;
                        int CPUCount;
                    };

                }
            }
        }
    }
}

#endif


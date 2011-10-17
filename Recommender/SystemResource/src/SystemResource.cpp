#include "../include/SystemResource.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    /**
                     * Construtor.
                     */
                    SystemResource::SystemResource() {
                        sysinfo(&info);
                        uname(&sn);

                        initDataMembers();
                    }

                    /**
                     * Destrutor.
                     */
                    SystemResource::~SystemResource() {
                    }

                    /**
                     * Obtém o clock da máquina e o armazena em CPUClock.
                     */
                    void SystemResource::initDataMembers() {
                        ifstream fis;
                        string line = "";

                        CPUClock = 0;
                        CPUCount = 0;
                        fis.open("/proc/cpuinfo", ifstream::in);

                        if (!fis.is_open()) {
                            cerr << "SystemResource::initDataMembers Warning: can't open "
                                    << "file '/proc/cpuinfo'" << endl;
                            return;
                        }

                        while (fis.good()) {
                            fis >> line;
                            if (line == "cpu") {
                                fis >> line;
                                if (line == "MHz") {
                                    fis >> line;
                                    if (line == ":") {
                                        fis >> line;
                                        CPUClock = (float) Utilitario::stod(line);
                                    }
                                }
                            } else if (line == "processor") {
                                fis >> line;
                                if (line == ":") {
                                    CPUCount++;
                                }
                            }
                        }
                    }

                    /**
                     * Retorna a frequência da CPU.
                     * @return frequência da CPU em Hz.
                     */
                    long SystemResource::getCPUClock() {
                        return (long) CPUClock;
                    }

                    /**
                     * Retorna quantidade de CPUs da máquina.
                     * @return quantidade de CPUs da máquina.
                     */
                    int SystemResource::getCPUCount() {
                        return CPUCount;
                    }

                    /**
                     * Retorna a quantidade total de memória da máquina.
                     * @return total de memória em bytes.
                     */
                    long SystemResource::getMemorySize() {
                        return (long) info.totalram / 1024;
                    }

                    /**
                     * Retorna a quantidade de memória livre na máquina.
                     * @return total de memória livre (em bytes).
                     */
                    long SystemResource::getMemoryFree() {
                        return (long) info.freeram / 1024;
                    }

                    /**
                     * Retorna a quantidade total de espaço de armazenamento da máquina.
                     * @return total de espaço de armazenamento da máquina (em bytes).
                     */
                    off_t SystemResource::getDiskSize() {
                        unsigned long long result = 0;
                        int n;
                        struct statvfs sfs;

                        if (statvfs("/", &sfs) != -1) {
                            result = (unsigned long long) sfs.f_bsize * sfs.f_blocks;
                        }

                        if (result > 0) {
                            return (off_t) result;
                        }

                        return 0;
                    }

                    /**
                     * Retorna a quantidade de espaço de armazenamento livre da máquina.
                     * @return espaço de armazenamento livre da máquina (em bytes).
                     */
                    off_t SystemResource::getDiskFree() {
                        unsigned long long result = 0;
                        int n;
                        struct statvfs sfs;

                        if (statvfs("/", &sfs) != -1) {
                            result = (unsigned long long) sfs.f_bsize * sfs.f_bfree;
                        }

                        if (result > 0) {
                            return (off_t) result;
                        }
                        return 0;
                    }

                    /**
                     * Reserva um recurso do sistema para uma aplicação.
                     * @param applicationId identificação da aplicação que solicita o recurso.
                     * @param resource identificação do recurso solicitado: 1 - CPU; 2 - Memória; 3 - Disco.
                     * @param count quantidade do recurso solicitada.
                     * O retorno deste método devolve um identificador para que o recurso seja liberado mais tarde.
                     * @return maior que 0, se a operação foi executada com sucesso, ou -1, caso contrário.
                     */
                    int SystemResource::reserve(const char* applicationId, int resource, long count) {
                        int fd;

                        if (resource == 1) {

                        } else if (resource == 2) {

                        } if (resource == 3) {
                            fd = creat(applicationId, 0644);

                            if (fd == -1 || ftruncate(fd, count) == 1) {
                              return -1;
                            }
                        } else {
                            return -1;
                        }

                        return fd;
                    }

                    /**
                     * Desfaz uma reserva de recurso do sistema.
                     * @param applicationId identificação da aplicação que solicitou o recurso.
                     * @param reserveId identificador do recurso que será liberado.
                     * @return true, se a operação foi executada com sucesso, ou false, caso contrário.
                     */
                    bool SystemResource::freeReserve(const char* applicationId, int reserveId) {
                        if (close(reserveId) == -1) {
                            return false;
                        }
                       
                        return (unlink(applicationId) != -1);
                    }

                    /**
                     * Faz uso de um recurso do sistema reservado anteriormente.
                     * @param applicationId identificação da aplicação que solicitou o recurso.
                     * @param reserveId identificador do recurso reservado.
                     * @return true, se a operação foi executada com sucesso, ou false, caso contrário.
                     */
                    bool SystemResource::allocate(const char* applicationId, int reserveId) {
                        struct stat fileStatus;

                        if (stat(applicationId, &fileStatus) == -1) {
                            return false;
                        }

                        //return (fallocate(reserveId, FALLOC_FL_KEEP_SIZE, 0, fileStatus.st_size - 1) == 0);
                        return (posix_fallocate(reserveId, 0, fileStatus.st_size - 1) == 0);
                    }

                    /**
                     * Libera o uso de um recurso do sistema.
                     * @param applicationId identificação da aplicação que solicitou o recurso.
                     * @param reserveId identificador do recurso reservado.
                     * @return true, se a operação foi executada com sucesso, ou false, caso contrário.
                     */
                    bool SystemResource::freeAllocation(const char* applicationId, int reserveId) {
                        return (remove(applicationId) != -1);
                    }

                };

            }
        }
    }
}


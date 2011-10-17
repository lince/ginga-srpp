#include <../include/Scheduler.h>

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {
		  
		    Scheduler* Scheduler::_instance = 0;
		    
                    /**
                     * Construtor.
                     * Devolve uma instância de Scheduler.
                     */
                    Scheduler::Scheduler() {
			// TODO Auto-generated constructor stub
				cout << "Construindo o Scheduler" << endl;
				ifstream inifile("recommender.ini");
				int tam = 0;
				char *buffer;
				if (inifile.is_open()) {
					inifile.seekg(0, ios::end);
					tam = inifile.tellg();
					inifile.seekg(0, ios::beg);

					buffer = new char[tam];

					inifile.read(buffer, tam);
					inifile.close();

					timeTomin = Utilitario::stod(buffer);

				} else
					timeTomin = 4;

				inifile.close();
				temp = (double *) &timeTomin;

				stopflag = false;
                    }

                    /**
                     * Destrutor.
                     */
                    Scheduler::~Scheduler() {

                    }

		    Scheduler* Scheduler::getInstance() {
			cout << "*******dentro da função Scheduler::getInstance::" << Scheduler::_instance << endl;
			    if (Scheduler::_instance == NULL) {
				    Scheduler::_instance = new Scheduler();
				    cout << "*******Scheduler recebeu uma nova instancia" << endl;

			    }

			    return Scheduler::_instance;

		    }

		   /* void Scheduler::checkRecommenderModule() {
			//cout << "*******iniciando a thread Scheduler::checkRecommenderModule" << endl;
			    int rc;
			    char *t = "thread_recommender";

			    rc = pthread_create(&recomenda, NULL, (void * (*)(void *)) runThread, temp);
			//cout << "*******thread recomenda inicializada" << endl;
		    }

		    pthread_t Scheduler::getThread() {
			    return recomenda;
		    }

		    void * runThread(void * p) {

			    double d = *((double *) p);
			    unsigned int tempo = (unsigned int) d * 1000000;

			    __useconds_t t = tempo;

			    while (!((Scheduler::getInstance())->should_stop())) {

				    if (ImplicitContextManager::canInstantiate()) {
					    (ImplicitContextManager::getInstance())->guessImplicitUser();
				    cout << "Instanciei o ICTM" << endl;
				    }
				    cout << "Passei por aqui" << endl;
				    // (Scheduler::getInstance())->startRecommenderModule();
				    ::usleep(t);
			    }
		    }*/


                    /**
                     * Agenda a execução de uma aplicação.
                     * @param lib nome da biblioteca que contém a aplicação a ser executada.
                     * @param constructor construtor da aplicação que devolve uma instância para a interface SchedulerItem.
                     * @param destructor destrutor da aplicação.
                     * @param params parâmetros para a aplicação.
                     * @param begin data e horário para iniciar a execução da aplicação (formato: dd/MM/aaaa-hh:mm:ss).
                     * @param end data e horário para o fim da execução da aplicação (formato: dd/MM/aaaa-hh:mm:ss).
                     * @param period periodicidade do agendamento: 1 - uma única vez; 2 - uma vez por dia; 3 - indefinidamente; 4 - uma vez por semana; 5 - uma vez por mês.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Scheduler::schedule(const char* lib, const char* constructor, const char* destructor, vector<char*>* params, const char* begin, const char* end, int period) {

                    }

                    /**
                     * Carrega para a memória as informações de agendamentos armazenadas em arquivo.
                     * @param file arquivo XML com informações de agendamentos.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Scheduler::load(const char* file) {

                    }

                    /**
                     * Salva em arquivo as informações de agendamentos em memória.
                     * @param file arquivo XML para destino das informações de agendamentos.
                     * @return true, caso a operação seja executada com sucesso, ou false, caso contrário.
                     */
                    bool Scheduler::store(const char* file) {

                    }
                }
            }
        }
    }
}

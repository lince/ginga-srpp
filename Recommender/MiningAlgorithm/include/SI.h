#ifndef _IMININGALGORITHMAPRIORISI_H_
#define _IMININGALGORITHMAPRIORISI_H_

#include <vector>
#include <string>
#include <map>
#include <fstream>

#ifdef HAVE_RECOMMENDER
#include <ginga/recommender/sdt/SDT.h>
#include <ginga/recommender/eit/EIT.h>
#else
#include "../../SDT/include/SDT.h"
#include "../../EIT/include/EIT.h"
#endif

using namespace std;
using namespace br::ufscar::lince::ginga::recommender;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    class si : public eit, public sdt {
                    public:
                        si();

                    };
                }
            }
        }
    }
}

#endif


#include "recommender/database/Database.h"
#include "recommender/utils/Utils.h"
#include "recommender/localagent/Key.h"
using namespace ::br::ufscar::lince::ginga::recommender;

#include "cm/IComponentManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::cm;

#include "contextmanager/IContextManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::contextmanager;

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMWriter.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string.h>
#include <map>
#include <stdlib.h>

static char* goutputfile= "/";

// options for DOMLSSerializer's features
static XMLCh* gOutputEncoding= 0;
static bool gSplitCdataSections = true;
static bool gDiscardDefaultContent = true;
static bool gUseFilter = false;
static bool gFormatPrettyPrint = false;
static bool gWriteBOM = false;


#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif

XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public :
    XStr(const char* const toTranscode)
    {
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }

    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()

string getValue(string varAmbiente);

int exportXML(string path);

vector<string> head3;
vector<string> data3;
vector<string> head;
vector<string> data;
vector<string> head2;
vector<string> data2;
int rows;
int cols;
int rows2;
int cols2;
int rows3;
int cols3;



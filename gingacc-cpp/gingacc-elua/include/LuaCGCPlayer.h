extern "C"{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#include <map>

#include "cgc/ProgramRecorder.h"
#include "cgc/ProgramHeader.h"
using namespace ::br::ufscar::lince::ginga::core::cgc;

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace player {

	class LuaCGCPlayer {
	
		public:

			static int lua_getExt(lua_State* L);

			static int lua_completePath(lua_State* L);

			static int lua_listRecords(lua_State* L);
		
			static int lua_getRecord(lua_State* L);
		
			static int lua_removeRecord(lua_State* L);

	};

}
}
}
}
}
}


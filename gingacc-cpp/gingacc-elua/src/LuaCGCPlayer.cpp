#include "../include/LuaCGCPlayer.h"

namespace br {
namespace ufscar {
namespace lince {
namespace ginga {
namespace core {
namespace player {

	int LuaCGCPlayer::lua_getExt(lua_State* L)
	{
		string ext;
		int args = lua_gettop(L);
	
		if(args == 1)
		{
			ext = ProgramRecorder::getExt(lua_tonumber(L, 1));
			lua_pushstring(L, ext.c_str());
			return 1;
		}
		else
		{
			//código de erro
			return 0;
		}
	}

	int LuaCGCPlayer::lua_completePath(lua_State* L)
	{
		string path;	
		int args = lua_gettop(L);
	
		if(args == 3)
		{
			path = ProgramRecorder::completePath(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tostring(L, 3));
			lua_pushstring(L, path.c_str());
			return 1;
		}
		else
		{
			//código de erro
			return 0;
		}
	}

	int LuaCGCPlayer::lua_listRecords(lua_State* L)
	{
		string path;
		int args = lua_gettop(L);
	
		if(args == 1)
		{
			path = lua_tostring(L, 1);
			DIR *dir = opendir(path.c_str());
			size_t pos;
			string aux, ext, opath;
			struct dirent *ent;
			while (ent = readdir(dir))
			{
				aux = string(ent->d_name);
				pos = aux.find('.');
				if(pos != string::npos)
				{
					ext = aux.substr(pos + 1);
					if(ext == "xml")
					{
						opath = path;
						opath += ent->d_name;
						ProgramHeader rdXml;
						rdXml.readXmlProgramHeader(opath);
				
						lua_newtable(L);
						lua_pushnumber(L, rdXml.getId());
						lua_setfield(L, -2, "id");
						lua_pushnumber(L, rdXml.getPcr());
						lua_setfield(L, -2, "pcr");
						lua_pushnumber(L, rdXml.getProgramNumber());
						lua_setfield(L, -2, "programnumber");
						lua_pushnumber(L, rdXml.getUser());
						lua_setfield(L, -2, "user");
						lua_pushnumber(L, rdXml.getDia());
						lua_setfield(L, -2, "dia");
						lua_pushnumber(L, rdXml.getMes());
						lua_setfield(L, -2, "mes");
						lua_pushnumber(L, rdXml.getAno());
						lua_setfield(L, -2, "ano");
						lua_pushnumber(L, rdXml.getHora());
						lua_setfield(L, -2, "hora");
						lua_pushnumber(L, rdXml.getMinuto());
						lua_setfield(L, -2, "minuto");
						lua_pushnumber(L, rdXml.getSegundo());
						lua_setfield(L, -2, "segundo");
				
						lua_setglobal(L, "tableXml");				
					}
					else if(ext == "hdr")
					{
						opath = path;
						opath += ent->d_name;
						ProgramHeader rdTxt;
						rdTxt.readTxtProgramHeader(opath);
				
						lua_newtable(L);
						lua_pushnumber(L, rdTxt.getId());
						lua_setfield(L, -2, "id");
						lua_pushnumber(L, rdTxt.getPcr());
						lua_setfield(L, -2, "pcr");
						lua_pushnumber(L, rdTxt.getProgramNumber());
						lua_setfield(L, -2, "programnumber");
						lua_pushnumber(L, rdTxt.getUser());
						lua_setfield(L, -2, "user");
						lua_pushnumber(L, rdTxt.getDia());
						lua_setfield(L, -2, "dia");
						lua_pushnumber(L, rdTxt.getMes());
						lua_setfield(L, -2, "mes");
						lua_pushnumber(L, rdTxt.getAno());
						lua_setfield(L, -2, "ano");
						lua_pushnumber(L, rdTxt.getHora());
						lua_setfield(L, -2, "hora");
						lua_pushnumber(L, rdTxt.getMinuto());
						lua_setfield(L, -2, "minuto");
						lua_pushnumber(L, rdTxt.getSegundo());
						lua_setfield(L, -2, "segundo");
				
						lua_setglobal(L, "tableTxt");
					}
				}
			}
		}
		else
		{
			//código de erro
		}
		
		return 0;
	}

	int LuaCGCPlayer::lua_getRecord(lua_State* L)
	{
		string path;
		ProgramHeader* file;
		int args = lua_gettop(L);
	
		if(args == 1)
		{
			path = lua_tostring(L, 1);
			file = ProgramRecorder::getRecord(path);
			lua_newtable(L);
			lua_pushnumber(L, file->getId());
			lua_setfield(L, -2, "id");
			lua_pushnumber(L, file->getPcr());
			lua_setfield(L, -2, "pcr");
			lua_pushnumber(L, file->getProgramNumber());
			lua_setfield(L, -2, "programnumber");
			lua_pushnumber(L, file->getUser());
			lua_setfield(L, -2, "user");
			lua_pushnumber(L, file->getDia());
			lua_setfield(L, -2, "dia");
			lua_pushnumber(L, file->getMes());
			lua_setfield(L, -2, "mes");
			lua_pushnumber(L, file->getAno());
			lua_setfield(L, -2, "ano");
			lua_pushnumber(L, file->getHora());
			lua_setfield(L, -2, "hora");
			lua_pushnumber(L, file->getMinuto());
			lua_setfield(L, -2, "minuto");
			lua_pushnumber(L, file->getSegundo());
			lua_setfield(L, -2, "segundo");
			
			/*for(int i = -7; i <= 3; i++)
			{
				cout << i << " " << lua_type(L, i) << " " <<lua_istable(L, i) << endl;
			}
			*/
			lua_settop(L, -1);
			lua_setglobal(L, "table");
			
		}
		else
		{
			//código de erro
		}
		
		return 1;
	}

	int LuaCGCPlayer::lua_removeRecord(lua_State* L)
	{
		bool check;
		int args = lua_gettop(L);

		if(args == 1)
		{
			check = ProgramRecorder::removeRecord(lua_tostring(L, 1));
			lua_pushboolean(L, check);
			return 1;
		}
		else
		{
			//código de erro
			return 0;
		}
	}

}
}
}
}
}
}

#include <RmlUi/Lua/LuauAuxLib.h>

#include <lua.h>

#define FREELIST_REF 0

int Rml::Lua::luaL_ref(lua_State* L, int t) {
	t = lua_absindex(L, t);

	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return LUA_REFNIL;
	}

	lua_rawgeti(L, t, FREELIST_REF);
	int ref = lua_tointeger(L, -1);
	lua_pop(L, 1);

	if (ref != 0) {
		lua_rawgeti(L, t, ref);
		lua_rawseti(L, t, FREELIST_REF);
	}
	else {
		ref = lua_objlen(L, t) + 1;
	}

	lua_rawseti(L, t, ref);

	return ref;
}

void Rml::Lua::luaL_unref(lua_State* L, int t, int ref) {
	if (ref >= 0) {
		t = lua_absindex(L, t);
		lua_rawgeti(L, t, FREELIST_REF);
		lua_rawseti(L, t, ref);
		lua_pushinteger(L, ref);
		lua_rawseti(L, t, FREELIST_REF);
	}
}

#pragma once

struct lua_State;

namespace Rml {
namespace Lua {

int luaL_ref(lua_State* L, int t);
void luaL_unref(lua_State* L, int t, int ref);

}
}

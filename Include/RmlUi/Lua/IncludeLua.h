#pragma once

#if !defined(RMLUI_LUA_AS_CXX) && !defined(RMLUI_LUAU)
extern "C" {
#endif

// The standard Lua headers
#ifndef RMLUI_LUAU
#include <lauxlib.h>
#endif
#include <lua.h>
#include <lualib.h>

#if !defined(RMLUI_LUA_AS_CXX) && !defined(RMLUI_LUAU)
}
#endif

#ifdef RMLUI_LUAU
#include <RmlUi/Lua/LuauAuxLib.h>
#define RMLUI_LUA_PUSHCFUNCTION(L, fn) lua_pushcfunction(L, fn, #fn)
#define RMLUI_LUA_PUSHCCLOSURE(L, fn, nup) lua_pushcclosure(L, fn, #fn, nup)
#define RMLUI_LUA_TESTUDATA(L, ud, tname) luaL_checkudata
#else
#define RMLUI_LUA_PUSHCFUNCTION(L, fn) lua_pushcfunction(L, fn)
#define RMLUI_LUA_PUSHCCLOSURE(L, fn, nup) lua_pushcclosure(L, fn, nup)
#define RMLUI_LUA_TESTUDATA(L, ud, tname) luaL_testudata
#endif

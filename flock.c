#include <lua.h>
#include <lauxlib.h>

#include <sys/file.h>
#include <errno.h>
#include <string.h>

static int lua_flock(lua_State* L) {
  FILE** f = luaL_checkudata(L, 1, "FILE*");
  int operation = 0;
  int fd = fileno(*f);
  if(fd < 0) return luaL_error(L, "file handle is closed");
  switch(luaL_checkoption(L, 2, "exclusive", (const char*[]){
        "sh", "shared", "rd", "read", "passive",
          "un", "unlock", "close", "release",
          "ex", "excl", "exclusive", "wr", "write", "active",
          NULL
      })) {
  case 0: case 1: case 2: case 3: case 4: operation = LOCK_SH; break;
  case 5: case 6: case 7: case 8: operation = LOCK_UN; break;
  default: operation = LOCK_EX; break;
  }
  if(lua_gettop(L) >= 3 && lua_toboolean(L, 3))
    operation |= LOCK_NB;
  if(flock(fd, operation) < 0) {
    lua_pushnil(L);
    if(errno == EWOULDBLOCK) lua_pushliteral(L, "would block");
    else lua_pushstring(L, strerror(errno));
    return 2;
  }
  else {
    lua_pushboolean(L, 1);
    return 1;
  }
}

int luaopen_flock(lua_State* L) {
  lua_pushcfunction(L, lua_flock);
  return 1;
}

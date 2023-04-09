#include <lua.h>
#include <lauxlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define CHECK_BOUNDS // Disable to avoid checking index bounds, will fail tests in carray_spec

#define metatable(shorttype) "carray<" #shorttype ">"

#ifdef LOG_ALLOCATIONS
	void *__cdecl realloc_and_log(void *_Block, size_t _Size)
	{
		void* ptr = (void*) realloc(_Block, _Size);
		printf("Re-allocating %zd bytes ( @%p -> @%p )\n", _Size, _Block, ptr);
		return ptr;
	}

	void __cdecl free_log(void *_Block)
	{
		printf("Freeing %p\n", _Block);
		free(_Block);
	}
	#define STBDS_REALLOC(c,p,s) realloc_and_log(p,s)
	#define STBDS_FREE(c,p) free_log(p)
#endif

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#ifdef CHECK_BOUNDS
	#define check_bounds_lower(index,stackLevel,ptr) { luaL_argcheck(L, index >= 0, stackLevel, "index < 1"); }
	#define check_bounds_upper(index,stackLevel,ptr) { luaL_argcheck(L, index < arrlen(*(ptr)), stackLevel, "index > length"); }
#else
	#define check_bounds_lower
	#define check_bounds_upper
#endif

#define array_set(type,shorttype) \
static int array_set_##shorttype##(lua_State *L) { \
	void* validPtr = (void*) luaL_checkudata(L, 1, metatable(shorttype)); \
	assert(validPtr != NULL); \
	type** ptr = (type**) validPtr; \
	assert(ptr != NULL); \
	int index = (int) luaL_checkinteger(L, 2) - 1; \
	check_bounds_lower(index, 2, ptr); \
	double value = luaL_checknumber(L, 3); \
	int minSize = 1 + index; \
	if (minSize > arrlen(*ptr)) {\
		arrsetlen(*ptr, minSize); \
	} \
	(*ptr)[index] = (type) value; \
	return 0; }

#define array_get(type,shorttype) \
static int array_get_##shorttype##(lua_State* L){ \
	void* validPtr = (void*) luaL_checkudata(L, 1, metatable(shorttype)); \
	assert(validPtr != NULL); \
	type** ptr = (type**) validPtr; \
	assert(ptr != NULL); \
	int index = (int) luaL_checkinteger(L, 2) - 1; \
	check_bounds_lower(index, 2, ptr); \
	check_bounds_upper(index, 2, ptr); \
	lua_pushnumber(L, (*ptr)[index]); \
	return 1; }

#define array_len(type,shorttype) \
static int array_len_##shorttype##(lua_State* L){ \
	void* validPtr = (void*) luaL_checkudata(L, 1, metatable(shorttype)); \
	assert(validPtr != NULL); \
	type** ptr = (type**) validPtr; \
	assert(ptr != NULL); \
	lua_pushinteger(L, arrlen(*ptr)); \
	return 1; }

array_set(int8_t, i8)
array_set(uint8_t, u8)
array_set(int16_t, i16)
array_set(uint16_t, u16)
array_set(int32_t, i32)
array_set(uint32_t, u32)
array_set(float, f32)
array_set(double, f64)

array_get(int8_t, i8)
array_get(uint8_t, u8)
array_get(int16_t, i16)
array_get(uint16_t, u16)
array_get(int32_t, i32)
array_get(uint32_t, u32)
array_get(float, f32)
array_get(double, f64)

array_len(int8_t, i8)
array_len(uint8_t, u8)
array_len(int16_t, i16)
array_len(uint16_t, u16)
array_len(int32_t, i32)
array_len(uint32_t, u32)
array_len(float, f32)
array_len(double, f64)

#define array_tostring(type,shorttype) \
static int array_tostring_##shorttype##(lua_State *L) { \
	void* validPtr = (void*) luaL_checkudata(L, 1, metatable(shorttype)); \
	assert(validPtr != NULL); \
	type** ptr = (type**) validPtr; \
	assert(ptr != NULL); \
	lua_pushfstring(L, "carray<" #shorttype ">: %p", *ptr); \
	return 1; }

array_tostring(int8_t, i8)
array_tostring(uint8_t, u8)
array_tostring(int16_t, i16)
array_tostring(uint16_t, u16)
array_tostring(int32_t, i32)
array_tostring(uint32_t, u32)
array_tostring(float, f32)
array_tostring(double, f64)

#define array_new(type,shorttype) \
static int array_new_##shorttype##(lua_State *L) { \
	int n = (int) luaL_checkinteger(L, 1); \
	luaL_argcheck(L, n >= 0, 1, "invalid size"); \
	void** ptr = (void**)lua_newuserdata(L, sizeof(void*)); \
	*ptr = NULL; \
	if (n>0) arrsetlen((type*)(*ptr), n); \
	luaL_getmetatable(L, metatable(shorttype)); \
	lua_setmetatable(L, -2); \
	return 1; }

array_new(int8_t,i8)
array_new(uint8_t,u8)
array_new(int16_t, i16)
array_new(uint16_t, u16)
array_new(int32_t, i32)
array_new(uint32_t, u32)
array_new(float,f32)
array_new(double,f64)

#define array_gc(type,shorttype) \
static int array_gc_##shorttype##(lua_State *L) { \
	void* validPtr = (void*) luaL_checkudata(L, 1, metatable(shorttype)); \
	assert(validPtr != NULL); \
	type** ptr = (type**) validPtr; \
	arrfree(*ptr); \
	return 0; \
}

array_gc(int8_t,i8)
array_gc(uint8_t,u8)
array_gc(int16_t, i16)
array_gc(uint16_t, u16)
array_gc(int32_t, i32)
array_gc(uint32_t, u32)
array_gc(float,f32)
array_gc(double,f64)

#define array_meta(shorttype) \
static const struct luaL_Reg array_meta_##shorttype##[] = { \
	{"__newindex", array_set_##shorttype##}, \
	{"__index", array_get_##shorttype##}, \
	{"__len", array_len_##shorttype##}, \
	{"__tostring", array_tostring_##shorttype##}, \
	{"__gc", array_gc_##shorttype##}, \
	{NULL, NULL}}; \

array_meta(i8)
array_meta(u8)
array_meta(i16)
array_meta(u16)
array_meta(i32)
array_meta(u32)
array_meta(f32)
array_meta(f64)

#define setup_array_meta(shorttype) \
	luaL_newmetatable(L, metatable(shorttype)); \
	lua_pushvalue(L, -1); \
	lua_setfield(L, -2, "__index"); \
	luaL_setfuncs(L, array_meta_##shorttype##, 0);

// constructors
static const struct luaL_Reg array[] = {
	{"new_i8", array_new_i8},
	{"new_u8", array_new_u8},
	{"new_i16", array_new_i16},
	{"new_u16", array_new_u16},
	{"new_i32", array_new_i32},
	{"new_u32", array_new_u32},
	{"new_f32", array_new_f32},
	{"new_f64", array_new_f64},
	{NULL, NULL}
};

int luaopen_carray(lua_State *L) {
	setup_array_meta(i8)
	setup_array_meta(u8)
	setup_array_meta(i16)
	setup_array_meta(u16)
	setup_array_meta(i32)
	setup_array_meta(u32)
	setup_array_meta(f32)
	setup_array_meta(f64)
		
	luaL_openlib(L, "carray", array, 0);
	return 1;
}

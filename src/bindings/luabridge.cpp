#include <iostream>
#include <LuaBridge.h>
#include "voicemod.h"

extern "C" int luaopen_voicemod(lua_State* L) {
    using namespace luabridge;

    getGlobalNamespace(L)
        .beginNamespace("voicemod")
        .addFunction("modifyVoice", [](lua_State* L) -> int {
            size_t frameCount = luaL_checkinteger(L, 1);
            const float* input = static_cast<const float*>(lua_touserdata(L, 2));
            float* output = static_cast<float*>(lua_touserdata(L, 3));

            modifyVoice(input, output, frameCount);

            return 0;
        })
        .addFunction("reduceVolume", [](lua_State* L) -> int {
            size_t frameCount = luaL_checkinteger(L, 1);
            const float* input = static_cast<const float*>(lua_touserdata(L, 2));
            float* output = static_cast<float*>(lua_touserdata(L, 3));
            float factor = luaL_checknumber(L, 4);

            reduceVolume(input, output, frameCount, factor);

            return 0;
        })
        .addFunction("applyDistortion", [](lua_State* L) -> int {
            size_t frameCount = luaL_checkinteger(L, 1);
            const float* input = static_cast<const float*>(lua_touserdata(L, 2));
            float* output = static_cast<float*>(lua_touserdata(L, 3));
            float threshold = luaL_checknumber(L, 4);

            applyDistortion(input, output, frameCount, threshold);

            return 0;
        })
        .addFunction("applyCombinedEffects", [](lua_State* L) -> int {
            size_t frameCount = luaL_checkinteger(L, 1);
            const float* input = static_cast<const float*>(lua_touserdata(L, 2));
            float* output = static_cast<float*>(lua_touserdata(L, 3));

            applyCombinedEffects(input, output, frameCount);

            return 0;
        })
        .endNamespace();

    std::cout << "LuaBridge bindings for VoiceMod initialized." << std::endl;

    return 0;
}

#include <iostream>
#include <portaudio.h>
#include "lua.hpp"
#include "voicemod.h"

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

static bool isProcessing = false;

typedef struct {
    float *inputBuffer;
    float *outputBuffer;
    size_t bufferSize;
} AudioData;

static int audioCallback(const void *input, void *output,
                         unsigned long frameCount,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    AudioData *data = (AudioData *)userData;

    if (isProcessing) {
        const float *inputBuffer = (const float *)input;
        float *outputBuffer = (float *)output;
        modifyVoice(inputBuffer, outputBuffer, frameCount);
    } else {
        memset(output, 0, frameCount * sizeof(float));
    }

    return paContinue;
}

bool initializeAudioSystem(PaStream **stream, AudioData *audioData) {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    err = Pa_OpenDefaultStream(stream,
                               1,                  // Input channels
                               1,                  // Output channels
                               paFloat32,          // Sample format
                               SAMPLE_RATE,        // Sample rate
                               FRAMES_PER_BUFFER,  // Frames per buffer
                               audioCallback,      // Callback
                               audioData);         // User data
    if (err != paNoError) {
        std::cerr << "Failed to open PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    err = Pa_StartStream(*stream);
    if (err != paNoError) {
        std::cerr << "Failed to start PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    return true;
}

void cleanupAudioSystem(PaStream *stream) {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
}

void bindLuaFunctions(lua_State *L) {
    lua_register(L, "startVoiceMod", [](lua_State *L) -> int {
        isProcessing = true;
        std::cout << "Voice modifier activated." << std::endl;
        return 0;
    });

    lua_register(L, "stopVoiceMod", [](lua_State *L) -> int {
        isProcessing = false;
        std::cout << "Voice modifier deactivated." << std::endl;
        return 0;
    });

    lua_register(L, "getVoiceModStatus", [](lua_State *L) -> int {
        lua_pushboolean(L, isProcessing);
        return 1;
    });
}

int main(int argc, char *argv[]) {
    std::cout << "Initializing Voice Modifier..." << std::endl;

    PaStream *stream = nullptr;
    AudioData audioData = {nullptr, nullptr, FRAMES_PER_BUFFER};

    if (!initializeAudioSystem(&stream, &audioData)) {
        return -1;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    bindLuaFunctions(L);

    if (argc > 1) {
        if (luaL_dofile(L, argv[1])) {
            std::cerr << "Error running LUA script: " << lua_tostring(L, -1) << std::endl;
        }
    } else {
        std::cout << "No LUA script provided. Running in interactive mode." << std::endl;
    }

    std::cout << "Press Enter to exit." << std::endl;
    std::cin.get();

    lua_close(L);
    cleanupAudioSystem(stream);
    std::cout << "Voice Modifier terminated." << std::endl;
    return 0;
}

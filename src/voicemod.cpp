#include "voicemod.h"
#include <cmath>
#include <cstring>

void modifyVoice(const float* input, float* output, size_t frameCount) {
    static float phase = 0.0f;
    static const float frequencyShift = 2.0f;
    const float sampleRate = 44100.0f;

    for (size_t i = 0; i < frameCount; ++i) {
        float modulatedSample = input[i] * sin(phase);
        phase += (2.0f * M_PI * frequencyShift) / sampleRate;

        if (phase > 2.0f * M_PI) {
            phase -= 2.0f * M_PI;
        }

        output[i] = modulatedSample;
    }
}

void reduceVolume(const float* input, float* output, size_t frameCount, float factor) {
    for (size_t i = 0; i < frameCount; ++i) {
        output[i] = input[i] * factor;
    }
}

void applyDistortion(const float* input, float* output, size_t frameCount, float threshold) {
    for (size_t i = 0; i < frameCount; ++i) {
        if (input[i] > threshold) {
            output[i] = threshold;
        } else if (input[i] < -threshold) {
            output[i] = -threshold;
        } else {
            output[i] = input[i];
        }
    }
}

void applyCombinedEffects(const float* input, float* output, size_t frameCount) {
    float tempBuffer[frameCount];
    reduceVolume(input, tempBuffer, frameCount, 0.8f);
    applyDistortion(tempBuffer, output, frameCount, 0.5f);
}

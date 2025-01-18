#ifndef VOICEMOD_H
#define VOICEMOD_H

#include <cstddef>

void modifyVoice(const float* input, float* output, size_t frameCount);

void reduceVolume(const float* input, float* output, size_t frameCount, float factor);

void applyDistortion(const float* input, float* output, size_t frameCount, float threshold);

void applyCombinedEffects(const float* input, float* output, size_t frameCount);

#endif // VOICEMOD_H

#ifndef WAVDATA_H
#define WAVDATA_H

#include <AL/al.h>
#include <AL/alc.h>

struct WavData {
    ALuint buffer; // Buffer OpenAL
    ALenum format; // Audio format (mono, estéreo, 8-bit, 16-bit)
    ALsizei size;  // Size of audio data
    ALvoid* data;  // Audio data
    ALsizei sampleRate; // Taxa de amostragem
};


#endif // WAVDATA_H

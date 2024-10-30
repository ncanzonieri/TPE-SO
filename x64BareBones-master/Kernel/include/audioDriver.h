#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#include <stdint.h>

/**
 * Plays a sound with the given frequency.
 * @param f The frequency of the sound.
 */
uint64_t playSound(uint32_t f);

/**
 * Stops the sound.
 */
void stopSound();

#endif //TPE_ARQ_AUDIODRIVER_H
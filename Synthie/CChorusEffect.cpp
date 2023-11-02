#include "pch.h"
#include "CChorusEffect.h"

void CChorusEffect::ApplyChorus(float& sample, float time)
{
    float delay = (1 + sin(2 * 3.14159 * rate * time)) * depth;
    size_t delaySamples = static_cast<size_t>(delay * delayBuffer.size());

    size_t readIndex = (currentIndex + delayBuffer.size() - delaySamples) % delayBuffer.size();

    float delayedSample = delayBuffer[readIndex];
    delayBuffer[currentIndex] = sample;
    sample = (sample + delayedSample) * 0.5;

    currentIndex = (currentIndex + 1) % delayBuffer.size();
}

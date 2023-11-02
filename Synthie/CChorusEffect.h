#pragma once

#include <vector>

class CChorusEffect
{
private:
    std::vector<float> delayBuffer;
    size_t currentIndex = 0;
    float depth = 0.005; 
    float rate = 1.5;   

public:
    CChorusEffect(int sampleRate) : delayBuffer(sampleRate, 0.0f) {}

    void ApplyChorus(float& sample, float time);

};


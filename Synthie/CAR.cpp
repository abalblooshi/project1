#include "pch.h"
#include "CAR.h"

CAR::CAR() : m_source(nullptr), m_attack(0.05), m_release(0.05), m_sampleRate(44100.0), m_time(0)
{
    m_frame[0] = 0;
    m_frame[1] = 0;
    m_duration = 0.0;
}

CAR::~CAR()
{
}

void CAR::SetSampleRate(double s)
{
    m_sampleRate = s;
}

void CAR::Start()
{
    m_time = 0;
}

bool CAR::Generate()
{
    if (!m_source)
        return false;

    bool valid = m_source->Generate();

    double frame[2];
    frame[0] = m_source->Frame(0);
    frame[1] = m_source->Frame(1);

    if (m_time < m_attack) {
        // If we're in the attack phase
        double fraction = m_time / m_attack;
        m_frame[0] = frame[0] * fraction;
        m_frame[1] = frame[1] * fraction;
    }
    else if (m_time > (m_duration - m_release)) {
        // If we're in the release phase
        double fraction = (m_duration - m_time) / m_release;
        m_frame[0] = frame[0] * fraction;
        m_frame[1] = frame[1] * fraction;
    }
    else {
        // If we're not in the attack or release phase
        m_frame[0] = frame[0];
        m_frame[1] = frame[1];
    }

    // Update the time
    m_time += 1.0 / m_sampleRate;

    // Stop generating after the release time ends
    if (m_time > (m_duration + m_release)) {
        return false;
    }

    return valid;
}

double CAR::Frame(int ch) const
{
    return m_frame[ch];
}
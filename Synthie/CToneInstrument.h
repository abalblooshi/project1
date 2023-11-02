#pragma once

#include "CInstrument.h"
#include "CSineWave.h"
#include "CAR.h"

class CToneInstrument : public CInstrument
{
public:
    CToneInstrument(void);
    ~CToneInstrument(void);

    virtual void Start();
    virtual bool Generate();

    void SetFreq(double f) { m_sinewave.SetFreq(f); }
    void SetAmplitude(double a) { m_sinewave.SetAmplitude(a); }
    void SetDuration(double d) { m_duration = d; }

    void SetNote(CNote* note);

    double GenerateSquareWave(double time);
    double ApplyResonFilter(double input);
    double ApplyEnvelope(double time, double input);

private:
    CSineWave m_sinewave;
    double m_duration;
    double m_time;
    CAR m_ar;

    double m_freq;
    double m_attackTime;
    double m_decayTime;
    double m_sustainLevel;
    double m_releaseTime;
};


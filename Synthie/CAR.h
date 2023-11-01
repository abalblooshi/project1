#pragma once

#include "CAudioNode.h"

class CAR {
public:
    CAR();
    ~CAR();

    void SetSource(CAudioNode* source) { m_source = source; }
    void SetAttack(double attack) { m_attack = attack; }
    void SetRelease(double release) { m_release = release; }
    void SetSampleRate(double s);
    void Start();
    bool Generate();
    double Frame(int ch) const;
    void SetDuration(double d) { m_duration = d; }

private:
    CAudioNode* m_source;
    double m_attack;       
    double m_release;      
    double m_sampleRate;   
    double m_time;        
    double m_frame[2];     
    double m_duration;
};





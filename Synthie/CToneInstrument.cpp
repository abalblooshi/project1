#include "pch.h"
#include "CToneInstrument.h"
#include "Notes.h"
#include "CSineWave.h"

CToneInstrument::CToneInstrument(void)
{
    m_duration = 0.1;
}

CToneInstrument::~CToneInstrument(void)
{
}

void CToneInstrument::Start()
{
    m_sinewave.SetSampleRate(GetSampleRate());
    m_sinewave.Start();
    m_time = 0;

    m_ar.SetSource(&m_sinewave);
    m_ar.SetSampleRate(GetSampleRate());
    m_ar.SetDuration(m_duration);
    m_ar.Start();
}

bool CToneInstrument::Generate()
{
    // Generate wave
    double sample = GenerateSquareWave(m_time);  

    // reson filter
    sample = ApplyResonFilter(sample);

    // envelope
    sample = ApplyEnvelope(m_time, sample);

    bool valid = m_ar.Generate();

    m_frame[0] = m_ar.Frame(0);
    m_frame[1] = m_ar.Frame(1);

    m_time += GetSamplePeriod();

    return valid && (m_time < m_duration);
}

void CToneInstrument::SetNote(CNote* note)
{
    // Get a list of all attribute nodes and the
    // length of that list
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    // Loop over the list of attributes
    for (int i = 0; i < len; i++)
    {
        // Get attribute i
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        // Get the name of the attribute
        CComBSTR name;
        attrib->get_nodeName(&name);

        // Get the value of the attribute.  A CComVariant is a variable
        // that can have any type. It loads the attribute value as a
        // string (UNICODE), but we can then change it to an integer 
        // (VT_I4) or double (VT_R8) using the ChangeType function 
        // and then read its integer or double value from a member variable.
        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == "duration")
        {
            //value.ChangeType(VT_R8);
            //SetDuration(value.dblVal);
            
            // Task 1
            value.ChangeType(VT_R8);
            double durationInBeats = value.dblVal;
            m_duration = durationInBeats * (60.0 / GetBPM());

            // Task 2
            //m_ar.SetDuration(m_duration);
        }
        else if (name == "note")
        {
            SetFreq(NoteToFrequency(value.bstrVal));
        }

    }
}

double CToneInstrument::ApplyResonFilter(double input)
{
    static double y1 = 0, y2 = 0;
    double output = input - y1 - y2;
    y2 = y1;
    y1 = output;
    return output;
}

double CToneInstrument::ApplyEnvelope(double time, double input)
{
    double envelope = 0;
    if (time < m_attackTime)
        envelope = time / m_attackTime;
    else if (time < (m_attackTime + m_decayTime))
        envelope = 1.0 - (time - m_attackTime) / m_decayTime * (1.0 - m_sustainLevel);
    else
        envelope = m_sustainLevel;

    return input * envelope;
}

double CToneInstrument::GenerateSquareWave(double time)
{
    double period = 1.0 / m_freq;
    double value = fmod(time, period) < period / 2 ? 1.0 : -1.0;
    return value;
}
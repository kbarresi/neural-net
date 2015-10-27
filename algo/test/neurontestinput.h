#ifndef NEURONTESTINPUT_H
#define NEURONTESTINPUT_H

#include "algo/neuroninput.h"

class NeuronTestInput : public NeuronInput {

public:
    NeuronTestInput();
    virtual ~NeuronTestInput() { }

    void setActivity(const float a) { m_activity = a; }
    float activity() const override { return m_activity; }
    float activityHistory() const override { return m_activity; }


private:
    float m_activity;
};

#endif // NEURONTESTINPUT_H

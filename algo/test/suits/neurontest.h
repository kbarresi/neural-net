#ifndef NEURONTEST_H
#define NEURONTEST_H

#include "testsuit.h"

class Neuron;
class NeuronTestInput;
class NeuronTest : public TestSuit {

public:
    NeuronTest(int msRuntime);
    ~NeuronTest();

    void start() override;
    void printResults() const override;

private:
    float* m_neuronOutputs;
    float* m_netCurrents;

    Neuron* m_neuron;
    NeuronTestInput *m_exciteInput;
    NeuronTestInput *m_inhibitInput;
};

#endif // NEURONTEST_H

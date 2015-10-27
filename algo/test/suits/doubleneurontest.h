#ifndef DOUBLENEURONTEST_H
#define DOUBLENEURONTEST_H

#include "testsuit.h"

class Neuron;
class NeuronTestInput;
class DoubleNeuronTest : public TestSuit {

public:
    DoubleNeuronTest(int iterations);
    virtual ~DoubleNeuronTest();

    void start() override;
    void printResults() const override;

private:
    float* m_neuronOneOutputs;
    float* m_neuronTwoOutputs;

    float* m_neuronOneMembraneVoltage;
    float* m_neuronTwoMembraneVoltage;

    NeuronTestInput* m_exciteInput;
    NeuronTestInput* m_inhibitInput;
    Neuron* m_n1;
    Neuron* m_n2;

};

#endif // DOUBLENEURONTEST_H

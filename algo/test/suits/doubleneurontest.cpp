#include <stdlib.h>

#include "doubleneurontest.h"
#include "algo/neuron.h"
#include "algo/test/neurontestinput.h"

DoubleNeuronTest::DoubleNeuronTest(int msDuration) : TestSuit((msDuration / 1000.f) / (1.f / CLK_FREQ)) {

    m_neuronOneOutputs = (float*)calloc(p_iterations, sizeof(float));
    m_neuronTwoOutputs = (float*)calloc(p_iterations, sizeof(float));

    m_neuronOneMembraneVoltage = (float*)calloc(p_iterations, sizeof(float));
    m_neuronTwoMembraneVoltage = (float*)calloc(p_iterations, sizeof(float));

    m_n1 = new Neuron();
    m_n2 = new Neuron();

    m_exciteInput = new NeuronTestInput();
    m_inhibitInput = new NeuronTestInput();

    m_exciteInput->setActivity(0.01f);
    m_inhibitInput->setActivity(0.99f);

    m_n1->addInput(m_exciteInput, 1.f);
    m_n1->addInput(m_inhibitInput, -1.f);
    m_n2->addInput(m_n1, 1.f);
}

DoubleNeuronTest::~DoubleNeuronTest() {
    free(m_neuronOneOutputs);
    free(m_neuronTwoOutputs);
    free(m_neuronOneMembraneVoltage);
    free(m_neuronTwoMembraneVoltage);

    delete m_n1;
    delete m_n2;
    delete m_exciteInput;
    delete m_inhibitInput;
}


void DoubleNeuronTest::start() {

    p_currentIteration = 0;
    for (p_currentIteration = 0; p_currentIteration < p_iterations; p_currentIteration++) {
        m_n1->runCycle();
        m_n2->runCycle();

        m_neuronOneOutputs[p_currentIteration] = m_n1->activity();
        m_neuronTwoOutputs[p_currentIteration] = m_n2->activity();
        m_neuronOneMembraneVoltage[p_currentIteration] = m_n1->m_vMembrane;
        m_neuronTwoMembraneVoltage[p_currentIteration] = m_n2->m_vMembrane;
    }
}

void DoubleNeuronTest::printResults() const {
    float clockCycleDuration = 1.f / CLK_FREQ;
    printf("Iteration,Time (s),Neuron 1 Vm, Neuron 2 Vm, Neuron 1 Activity, Neuron 2 Activity\n");
    for (int i = 0; i < p_iterations; i++)
        printf("%d,%.5f,%.2f,%.2f,%.2f,%.2f\n", i + 1, (i * clockCycleDuration), m_neuronOneMembraneVoltage[i], m_neuronTwoMembraneVoltage[i], m_neuronOneOutputs[i], m_neuronTwoOutputs[i]);
}

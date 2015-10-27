#include <stdlib.h>

#include "neurontest.h"
#include "algo/neuron.h"
#include "algo/test/neurontestinput.h"


NeuronTest::NeuronTest(int msRuntime) : TestSuit((msRuntime / 1000.f) / (1.f / CLK_FREQ)) {
    m_neuronOutputs = (float*)calloc(p_iterations, sizeof(float));
    m_netCurrents = (float*)calloc(p_iterations, sizeof(float));

    m_neuron = new Neuron();
    m_exciteInput = new NeuronTestInput();
    m_inhibitInput = new NeuronTestInput();

    m_exciteInput->setActivity(0.01f);
    m_inhibitInput->setActivity(0.99f);

    m_neuron->addInput(m_exciteInput, 1.f);
    m_neuron->addInput(m_inhibitInput, -1.f);
}

NeuronTest::~NeuronTest() {
    free(m_neuronOutputs);
    free(m_netCurrents);
    m_neuronOutputs = nullptr;

    delete m_neuron;
    delete m_exciteInput;
    delete m_inhibitInput;
}


void NeuronTest::start() {

    p_currentIteration = 0;
    for (p_currentIteration = 0; p_currentIteration < p_iterations; p_currentIteration++) {
        m_neuron->runCycle();

        if (p_currentIteration == p_iterations / 3) {
            m_exciteInput->setActivity(0.f);
            m_inhibitInput->setActivity(0.f);
        }

        m_neuronOutputs[p_currentIteration] = m_neuron->activity();
        m_netCurrents[p_currentIteration] = m_neuron->m_vMembrane;
    }
}

void NeuronTest::printResults() const {
    printf("Iteration,Output Value\n");
    for (int i = 0; i < p_iterations; i++)
        printf("%d,%.2f,%.2f\n", i + 1, m_netCurrents[i], m_neuronOutputs[i]);
}

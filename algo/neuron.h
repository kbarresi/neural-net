#ifndef NEURON_H
#define NEURON_H

#include "neuroninput.h"
#include "third_party/json.h"

#include <list>
#include <queue>

class Neuron;

/**
 * @brief The Synapse struct Structure to keep track of receptor and ion presence in
 * the (mostly post) synapic cleft. We can use this to calculate the overall synaptic
 * weight and module it over time in a bio-realistic way.
 */
struct Synapse {
    explicit Synapse(float w);
    ~Synapse();

    /* Net synapse weight, a calculated value from [0,1] */
    float weight;

    /*   Theta values determine the places where signs change in the ∆ω calculation. Technically,
     * these values relate to higher/lower levels of Ca2+ ions, but since Ca2+ directly impacts
     * synaptic weights, we can use the raw synaptic weight as an approximation.
     *   These values are dynamic, and change in response to pre/post synaptic activation patterns
     * and history. This aids in allowing long term and short term synaptic plasticity.
    */
    float m_thetaNegative;
    float m_thetaPositive;

    NeuronInput* preSynapticNeuron;
    Neuron* postSynapticNeuron;
};


class Neuron : public NeuronInput {

public:
    Neuron(uint32_t id = 0);
    virtual ~Neuron() { }

    /**
     * @brief runCycle Run a single cycle. Will summarize the current excitory and
     * inhibitory inputs, and then update our membrane potential.
     */
    void runCycle();
    void resetCycle() { m_cycleFinished = false; }

    float activity() const override { return m_activity; }
    float activityHistory() const override;

    void addInput(NeuronInput* n, const float weight);
    void removeInput(NeuronInput* n);

    Json::Value serialize() const;
    void deserialize(const Json::Value& obj);

    const std::list<Synapse*>& inputs() const { return m_inputs; }

private:

    float m_calculateActivty();

    static const int POLARIZATION_PERIOD_DUR;
    static const int DEPOLARIZATION_PERIOD_DUR;
    static const int REFRACTORY_PERIOD_DUR;

    static const int DEPOLARIZATION_CYCLES;
    static const int POLARIZATION_CYCLES;
    static const int REFRACTORY_CYCLES;

    static const int ACTION_HISTORY_LENGTH;

    static const float MEMBRANE_RESISTIVITY;

    // Number of cycles left for polarization, repolarization, and refractory
    int m_spikeRestRemaining;

    float m_activity;
    float m_restingPotential;

    float m_vMembrane;          //in mV
    float m_firingThreshold;    //in mV

    float m_excititoryStrength; //in mV
    float m_inhibitoryStrength; //in mV

    float m_excititoryCurrent;  //Total excititory current
    float m_inhibitoryCurrent;  //Total inhibitory current
    float m_leakCurrent;        //Total leaking current;
    float m_netCurrent;         //Total current;

    float m_inhibitoryPotential;
    float m_excititoryPotential;

    float m_rateConstant;

    bool m_cycleFinished;
    int m_iterationsSinceLastTrigger;

    /* History of our action output. */
    std::list<float> m_actionHistory;


    /* List of input neurons on their weights [-1, 1] and whether they excite or inhibit */
    std::list<Synapse*> m_inputs;

    friend class NeuronTest;
    friend class DoubleNeuronTest;
    friend class ImageNetTest;
};

#endif // NEURON_H

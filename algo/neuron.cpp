#include "neuron.h"

#include <QDebug>


const int Neuron::DEPOLARIZATION_PERIOD_DUR = 1;  //1mS
const int Neuron::DEPOLARIZATION_CYCLES = CLK_FREQ / (Neuron::DEPOLARIZATION_PERIOD_DUR * 1000) ;
const int Neuron::POLARIZATION_PERIOD_DUR = 1;  //1mS
const int Neuron::POLARIZATION_CYCLES = CLK_FREQ / (Neuron::POLARIZATION_PERIOD_DUR * 1000) ;
const int Neuron::REFRACTORY_PERIOD_DUR = 2;  //2mS
const int Neuron::REFRACTORY_CYCLES = CLK_FREQ / (Neuron::REFRACTORY_PERIOD_DUR * 1000);

const int Neuron::ACTION_HISTORY_LENGTH = (CLK_FREQ * 500) / 1000; // 500 mS

const float Neuron::MEMBRANE_RESISTIVITY = (16000000.f * 0.000000005f) / (0.002f); //est. 200 um long

Synapse::Synapse(float w) {
    weight = w;

    preSynapticNeuron = nullptr;
    postSynapticNeuron = nullptr;
}

Synapse::~Synapse() {
}

Neuron::Neuron(uint32_t id) : NeuronInput(id) {
    m_iterationsSinceLastTrigger = 0;
    m_spikeRestRemaining = 0;
    m_activity = 0.f;
    m_cycleFinished = false;
    m_restingPotential = -70.f;
    m_vMembrane = m_restingPotential;
    m_netCurrent = 0.f;
    m_firingThreshold = -50.f;

    m_excititoryStrength = 0.f;
    m_inhibitoryStrength = 0.f;

    m_excititoryCurrent = 0.f;
    m_inhibitoryCurrent = 0.f;

    m_excititoryPotential = 0.f;
    m_inhibitoryPotential = -75.f;


    m_rateConstant = (1000.f / CLK_FREQ) * 0.1f;
}

void Neuron::runCycle() {

    if (m_spikeRestRemaining) {
        m_spikeRestRemaining--;
        if (m_spikeRestRemaining < (POLARIZATION_CYCLES + REFRACTORY_CYCLES))
            m_activity = 0.f;
        m_cycleFinished = true;
        return;
    }

    //Calculate the net inputs.
    m_excititoryStrength = 0;
    m_inhibitoryStrength = 0;
    for (auto synapse : m_inputs) {
        //if ω is negative, we count it as inhibitory, otherwise excitatory. We multiply it times
        //the neuron activity (0 if not firing).
        if (synapse->weight > 0)
            m_excititoryStrength += (synapse->preSynapticNeuron->activity() * synapse->weight);
        else
            m_inhibitoryStrength += (synapse->preSynapticNeuron->activity() * synapse->weight);

        //-- Perform learning with this synapse -- //
        /* There are two main pathways that enable synaptic plasticity (and thus learning). The
         * first is pre/post synaptic firing timing. If the Pre-synaptic neuron fires before the
         * post-synaptic neuron, glutamate is able to bind to NMDA receptors, allowing Ca2+ ions to
         * flow into the post-synaptic cell. These calcium ions increase synaptic efficiency
         * (weight) by having AMPA receptors make the local dendrite more receptive to signals.
         *
         * The other pathway is VGCC channels, which allow Ca2+ ions through when an electric
         * potential is encountered. AKA when the post-synaptic cell fires, it gains some Ca2+ ions
         * regardless of the pre-synaptic activity. This accounts for much less AMDA receptor
         * changes than the NMDA receptors, but still has some weight.
         */

        /*First, lets look at the VGCC pathway, since we don't have to look at the pre-synaptic
         * cell to figure this out. We're going to calculate the VGCC's affect by looking at the
         * the history of our neuron's activity output (over ~250ms). We'll use all the history
         * available, use a Fick-inspired scaling equation to simulate diffusion, so we can put
         * higher emphasis on newer activity. We transform this into a [0,1] factor by keeping the
         * ratio of volume under our curve versus maximum (i.e. if the neuron fired every cycle).
         * It's a non-continuous curve, that has a value for each cycle.
         */

        float vgccContribution = 0.f; //0.0 - 1.0




        /* For the NMDAR contribution, we can utilize the VGCC contribution, since it is basically
         * a summary of the post-synaptic neural activity. We need to pre-synaptic activity, and
         * the two of these together make up the NMDAR activity contribution to plasticity.
         */

        float nmdaContribution = 1.f; //0.0 - 1.0



        //TODO: Figure out the comparability of the NMDA pathway vs VGCC pathway for weights.


        float xy = synapse->preSynapticNeuron->activity() * m_activity;
        float averagePreSynapseActivity = synapse->preSynapticNeuron->activityHistory();
        float averagePostSynapseActivity = activityHistory();
    }


    m_excititoryCurrent = m_excititoryStrength * (m_excititoryPotential - m_vMembrane);
    m_inhibitoryCurrent = m_inhibitoryStrength * (m_inhibitoryPotential - m_vMembrane);

    //Leak Current = -(V / R_membrane)
    m_leakCurrent = -(((m_vMembrane - m_restingPotential) / 1000.f) / MEMBRANE_RESISTIVITY);
    m_leakCurrent *= 1000.f;

    m_netCurrent = m_excititoryCurrent + m_inhibitoryCurrent + m_leakCurrent;
    m_vMembrane += m_rateConstant * m_netCurrent;

    m_activity = m_calculateActivty();
    if (m_vMembrane > m_firingThreshold) {
        m_vMembrane = m_restingPotential;
        m_spikeRestRemaining = DEPOLARIZATION_CYCLES + POLARIZATION_CYCLES + REFRACTORY_CYCLES;
        m_iterationsSinceLastTrigger = 0;
    }

    m_actionHistory.push_front(m_activity);
    if (m_actionHistory.size() > ACTION_HISTORY_LENGTH)
        m_actionHistory.pop_back();

    m_iterationsSinceLastTrigger++;
    m_cycleFinished = true;
}

float Neuron::activityHistory() const {
    float sum = 0;
    for (auto value : m_actionHistory)
        sum += value;

    sum /= m_actionHistory.size();
    return sum;
}

void Neuron::addInput(NeuronInput* n, const float weight) {
    for (auto synapse : m_inputs) {
        if (synapse->preSynapticNeuron == n)
            return;
    }

    Synapse *synapse = new Synapse(weight);
    synapse->preSynapticNeuron = n;
    synapse->postSynapticNeuron = this;
    m_inputs.push_back(synapse);
}

void Neuron::removeInput(NeuronInput* n) {
    for (auto it = m_inputs.begin(); it != m_inputs.end(); ++it) {
        if ((*it)->preSynapticNeuron == n) {
            delete (*it);
            m_inputs.erase(it);
            break;
        }
    }
}

Json::Value Neuron::serialize() const {
    Json::Value root;

    root["id"] = p_id;
    root["activity"] = m_activity;
    root["restingPotential"] = m_restingPotential;
    root["membranePotential"] = m_vMembrane;
    root["firingThreshold"] = m_firingThreshold;
    root["excititoryStrength"] = m_excititoryStrength;
    root["inhibitoryStrength"] = m_inhibitoryStrength;
    root["excititoryCurrent"] = m_excititoryCurrent;
    root["inhibitoryCurrent"] = m_inhibitoryCurrent;
    root["leakCurrent"] = m_leakCurrent;
    root["netCurrent"] = m_netCurrent;
    root["inhibitoryPotential"] = m_inhibitoryPotential;
    root["excititoryPotential"] = m_excititoryPotential;
    root["rateConstant"] = m_rateConstant;
    root["cycleFinished"] = m_cycleFinished;

    return root;
}

void Neuron::deserialize(const Json::Value& obj) {
    p_id = obj["id"].asUInt();
    m_activity = obj["activity"].asFloat();
    m_restingPotential = obj["restingPotential"].asFloat();
    m_vMembrane = obj["membranePotential"].asFloat();
    m_firingThreshold = obj["firingThreshold"].asFloat();
    m_excititoryStrength = obj["excititoryStrength"].asFloat();
    m_inhibitoryStrength = obj["inhibitoryStrength"].asFloat();
    m_excititoryCurrent = obj["excititoryCurrent"].asFloat();
    m_inhibitoryCurrent = obj["inhibitoryCurrent"].asFloat();
    m_leakCurrent = obj["leakCurrent"].asFloat();
    m_netCurrent = obj["netCurrent"].asFloat();
    m_inhibitoryPotential = obj["inhibitoryPotential"].asFloat();
    m_excititoryPotential = obj["excititoryPotential"].asFloat();
    m_rateConstant = obj["rateConstant"].asFloat();
    m_cycleFinished = obj["cycleFinished"].asFloat();
}

float Neuron::m_calculateActivty() {
    return m_vMembrane >= m_firingThreshold;
    //return (1.f / (1.f + pow(2.71828,(-(m_vMembrane-0.5f)*30.f))));
}

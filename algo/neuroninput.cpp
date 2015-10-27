#include "neuroninput.h"
#include <random>
NeuronInput::NeuronInput(uint32_t id) {
    if (id == 0)
        p_id = random() % UINT32_MAX;
    else
        p_id = id;
}


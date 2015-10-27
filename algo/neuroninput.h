#ifndef NEURONINPUT_H
#define NEURONINPUT_H

#include <stdint.h>

class NeuronInput {
public:
    NeuronInput(uint32_t id = 0);


    virtual float activity() const = 0;
    virtual float activityHistory() const = 0;
protected:
    uint32_t p_id;

};

#endif // NEURONINPUT_H

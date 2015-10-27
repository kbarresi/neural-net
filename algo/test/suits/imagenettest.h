#ifndef TESTFROMFILE_H
#define TESTFROMFILE_H

#include "testsuit.h"

class Neuron;
class NeuronTestInput;

/*
 * First layer is a NeuronTestInput with acitivity values corresponding to the current input image.
 * The image is changed every few cycles, so self-organizing behavior can take over. Ideally, the
 * output layer will be similar to an edge detector. Layers are fully connected.
 */
class ImageNetTest : public TestSuit {

public:
    ImageNetTest(int iterations = 1);
    virtual ~ImageNetTest();

    void start() override;
    void printResults() const override;

private:
    const int DIMS;

    NeuronTestInput*** m_inputLayer;
    Neuron*** m_h1;
    Neuron*** m_outputLayer;
};

#endif // TESTFROMFILE_H

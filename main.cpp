#include "simulationwindow.h"

#include "algo/test/suits/neurontest.h"
#include "algo/test/suits/imagenettest.h"
#include "algo/test/suits/doubleneurontest.h"

#include <QDebug>
int main(int argc, char *argv[]) {
    (void)(argc);
    (void)(argv);

    /*
    DoubleNeuronTest test(50);
    test.start();
    test.printResults();
    */


    NeuronTest test(1000);
    test.start();
    test.printResults();

    /*qDebug() << "Getting ready to start test..";
    ImageNetTest test;
    test.start();
    test.printResults();
    */
    return 0;
}

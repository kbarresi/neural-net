#ifndef TESTSUIT_H
#define TESTSUIT_H


class TestSuit {

public:
    TestSuit(int iterations);

    virtual void start() = 0;
    virtual void printResults() const = 0;

protected:
    int p_iterations;
    int p_currentIteration;

};

#endif // TESTSUIT_H

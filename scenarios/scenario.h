#ifndef SCENARIOS_SCENARIO_H_
#define SCENARIOS_SCENARIO_H_

#include "../types.h"

namespace scenarios
{

class Scenario
{

protected:
    /** Number of cells */
    const unsigned int m_size;

public:
    Scenario(unsigned int size) : m_size(size) { }

    /**
     * @return Initial water height at pos
     */
    virtual unsigned int getHeight(unsigned int pos) = 0;

    /**
     * @return Space time dependent momentum at pos
     */
    virtual int getMomentum(unsigned int pos) = 0;

    /**
     * @return Cell size of one cell (= domain size/number of cells)
     */
    virtual T getCellSize()
    {
        return 1000.f / m_size;
    }
};
};

#endif

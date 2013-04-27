#ifndef SCENARIOS_SCENARIO_H_
#define SCENARIOS_SCENARIO_H_

namespace scenarios
{

template <typename T> class Scenario
{

protected:
    /** Number of cells */
    const unsigned int m_size;

public:
    Scenario(unsigned int size) : m_size(size) { }

    /**
     * @return Initial water height at pos
     */
    virtual T getHeight(unsigned int pos) = 0;

    /**
     * @return Space time dependent momentum at pos
     */
    virtual T getMomentum(unsigned int pos) = 0;

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

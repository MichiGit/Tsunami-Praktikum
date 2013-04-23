#ifndef SCENARIOS_RARERARE_H_
#define SCENARIOS_RARERARE_H_

#include "../types.h"

namespace scenarios
{

class RareRare
{
private:
    /** Number of cells */
    const unsigned int m_size;

public:
    ShockShock(unsigned int size) : m_size(size) { }

    /**
     * @return Initial water height at pos
     */
    unsigned int getHeight(unsigned int pos)
    {
        return 300;
    }

    int getMomentum(unsigned int pos)
    {
        int v = 200;
        if (pos <= m_size/2)
            return -v;
        return v;
    }

    /**
     * @return Cell size of one cell (= domain size/number of cells)
     */
    T getCellSize()
    {
        return 1000.f / m_size;
    }
};

}

#endif

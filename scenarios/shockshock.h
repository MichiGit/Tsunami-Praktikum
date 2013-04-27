#ifndef SCENARIOS_SHOCKSHOCK_H_
#define SCENARIOS_SHOCKSHOCK_H_

#include "scenario.h"

namespace scenarios
{

class ShockShock : public Scenario<T>
{

public:

    ShockShock(unsigned int size) : Scenario(size, 300, 300, 200, -200) { }

    ShockShock(unsigned int size, const T h, const T hul, const T hur) : Scenario(size, h, h, hul, hur) { }

    T getHeight(unsigned int pos)
    {
        return m_hl;
    }

    T getMomentum(unsigned int pos)
    {
        if (pos <= m_size/2)
            return m_hul;
        return m_hur;
    }
};

}

#endif

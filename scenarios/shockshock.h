#ifndef SCENARIOS_SHOCKSHOCK_H_
#define SCENARIOS_SHOCKSHOCK_H_

#include "scenario.h"
#include "../types.h"

namespace scenarios
{

class ShockShock : public Scenario<T>
{

public:

    ShockShock(unsigned int size) : Scenario(size) { }

    T getHeight(unsigned int pos)
    {
        return 300;
    }

    T getMomentum(unsigned int pos)
    {
        T v = 200;
        if (pos <= m_size/2)
            return v;
        return -v;
    }
};

}

#endif

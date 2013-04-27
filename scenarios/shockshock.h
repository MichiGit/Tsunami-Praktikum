#ifndef SCENARIOS_SHOCKSHOCK_H_
#define SCENARIOS_SHOCKSHOCK_H_

#include "scenario.h"
#include "../types.h"

namespace scenarios
{

class ShockShock : public Scenario
{

public:

    unsigned int getHeight(unsigned int pos)
    {
        return 300;
    }

    int getMomentum(unsigned int pos)
    {
        int v = 200;
        if (pos <= m_size/2)
            return v;
        return -v;
    }
};

}

#endif

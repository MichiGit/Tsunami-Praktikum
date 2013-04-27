#ifndef SCENARIOS_RARERARE_H_
#define SCENARIOS_RARERARE_H_

#include "scenario.h"

namespace scenarios
{

class RareRare : public Scenario<T>
{

public:

    RareRare(unsigned int size) : Scenario(size, 300, 300, 200, -200) { }

    RareRare(unsigned int size, const T h, const T hul, const T hur) : Scenario(size, h, h, hul, hur) { }

    T getHeight(unsigned int pos)
    {
        return 300;
    }

    T getMomentum(unsigned int pos)
    {
        T v = 200;
        if (pos <= m_size/2)
            return -v;
        return v;
    }
};

}

#endif

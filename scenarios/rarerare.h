#ifndef SCENARIOS_RARERARE_H_
#define SCENARIOS_RARERARE_H_

#include "scenario.h"

namespace scenarios
{

class RareRare : public Scenario<T>
{

public:

    RareRare(unsigned int size) : Scenario(size, 300, 300, 200, -200) { }

    RareRare(unsigned int size, const T h, const T hu) :
        Scenario(size, h, h, hu >= 0 ? -hu : hu, hu >= 0 ? hu : -hu) { }

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

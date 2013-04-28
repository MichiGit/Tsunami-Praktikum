#ifndef SCENARIOS_RARERARE_H_
#define SCENARIOS_RARERARE_H_

#include "scenario.h"

namespace scenarios
{

class RareRare : public Scenario<T>
{

public:

    /**
     * Constructor which will initialize the vector components using some default values.
     */
    RareRare(unsigned int size) : Scenario(size, 300, 300, -200, 200) { }

    /**
     * Constructor which defines the height and the momentum
     * @param [in] h The height for both wave vectors
     * @param [in] hu The momentum. If the given value is negative/positive, it will be used
     * for the momentum of the left/right wave vector and the same value with opposite sign
     * will be used for the right/left wave vector.
     */
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

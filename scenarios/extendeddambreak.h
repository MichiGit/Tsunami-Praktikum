#ifndef SCENARIOS_EXTDAMBREAK_H_
#define SCENARIOS_EXTDAMBREAK_H_

#include "scenario.h"

namespace scenarios
{

class ExtendedDamBreak : public Scenario<T>
{

public:

    ExtendedDamBreak(unsigned int size) : Scenario(size, 300, 300, 200, -200) { }

    ExtendedDamBreak(unsigned int size, const T hl, const T hr, const T hur) : Scenario(size, hl, hr, 0, hur) { }

	T getHeight(unsigned int pos)
	{
		if (pos <= m_size/2)
			return m_hl;

		return m_hr;
	}

    T getMomentum(unsigned int pos)
    {
        if (pos <= m_size/2)
            return 0;
        return m_hur;
    }
};

}

#endif /* SCENARIOS_EXTDAMBREAK_H_ */

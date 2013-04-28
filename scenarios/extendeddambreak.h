#ifndef SCENARIOS_EXTDAMBREAK_H_
#define SCENARIOS_EXTDAMBREAK_H_

#include "scenario.h"

namespace scenarios
{

class ExtendedDamBreak : public Scenario<T>
{

public:

    /**
     * Constructor which will initialize the vector components using some default values.
     */
    ExtendedDamBreak(unsigned int size) : Scenario(size, 14, 3.5, 0, 0.7) { }

    /**
     * Constructor which defines the height and the momentum
     * @param [in] hl The height for the left wave
     * @param [in] hr The height for the right wave
     * @param [in] hu The momentum for the right wave. The momentum of the left wave will
     * always default to 0.
     */
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

    T getCellSize()
    {
        return 30000.f / m_size;
    }
};

}

#endif /* SCENARIOS_EXTDAMBREAK_H_ */

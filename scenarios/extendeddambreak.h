/**
 * @file
 *  This file is part of SWE1D
 *
 *  SWE1D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SWE1D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SWE1D.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Diese Datei ist Teil von SWE1D.
 *
 *  SWE1D ist Freie Software: Sie koennen es unter den Bedingungen
 *  der GNU General Public License, wie von der Free Software Foundation,
 *  Version 3 der Lizenz oder (nach Ihrer Option) jeder spaeteren
 *  veroeffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 *  SWE1D wird in der Hoffnung, dass es nuetzlich sein wird, aber
 *  OHNE JEDE GEWAEHELEISTUNG, bereitgestellt; sogar ohne die implizite
 *  Gewaehrleistung der MARKTFAEHIGKEIT oder EIGNUNG FUER EINEN BESTIMMTEN
 *  ZWECK. Siehe die GNU General Public License fuer weitere Details.
 *
 *  Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 *  Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 *
 * @copyright 2013 Technische Universitaet Muenchen
 * @author Sebastian Rettenberger <rettenbs@in.tum.de>
 */

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

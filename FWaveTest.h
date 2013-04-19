/*
 * File:   FWaveTest.h
 * Author: gutbrod
 *
 * Created on April 18, 2013, 4:52 PM
 */

#ifndef _FWAVETEST_H
#define	_FWAVETEST_H

typedef float T;

#include <cxxtest/TestSuite.h>
#include "FWave.h"

class FWaveTest : public CxxTest::TestSuite {
public:

    void testEigenvalueComputation() {

    }

    void testZeroNetUpdates() {
        T b1 = 0.0;
        T b2 = 0.0;
        T *updates = new T[4];
        T hl = 10;
        T hr = 10;
        T hul = 5;
        T hur = 5;
        T maxEdgeSpeed;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
        hl = hr = 48;
        hul = hur = -39;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
        hl = hr = hul = hur = 0;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
    }

    void testSupersonicProblems() {

    }

private:
    solver::FWave<T> m_solver;
    void checkIfUpdatesAreApproximatelyZero(const T updates[4]) {

        for (int i = 0; i < 4; i++) {
            TS_ASSERT_LESS_THAN(updates[i], 1);
            TS_ASSERT_LESS_THAN(-1, updates[i]);
        }
    }

};


#endif	/* _FWAVETEST_H */


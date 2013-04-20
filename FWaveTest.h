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
        T expectedEigenvalues[2];
        T actualEigenvalues[2];
        hl = 10;
        hul = 20;
        hr = 1;
        hur = 8;
        expectedEigenvalues[0] = -3.9038;
        expectedEigenvalues[1] = 10.7869;
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[0], actualEigenvalues[0]));
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[1], actualEigenvalues[1]));
        hl = 300;
        hul = -4;
        hr = 540;
        hur = 40;
        expectedEigenvalues[0] = -64.152;
        expectedEigenvalues[1] = 64.2255;
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[0], actualEigenvalues[0]));
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[1], actualEigenvalues[1]));
        hl = 64;
        hul = -128;
        hr = 1000;
        hur = -647;
        expectedEigenvalues[0] = -73.1622;
        expectedEigenvalues[1] = 71.3219;
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[0], actualEigenvalues[0]));
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[1], actualEigenvalues[1]));
    }

    void testZeroNetUpdates() {
        T b1 = 0.0;
        T b2 = 0.0;
        T updates[4];
        hl = hr = 10;
        hul = hur = 5;
        T maxEdgeSpeed;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
        hl = hr = 48;
        hul = hur = -39;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
        hl = hr = 3000;
        hul = hur = 0;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
    }

    void testSupersonicProblems() {
        // the components of the wave speed vector will have non opposite signs if hl = -hr
        hl = 10;
        hr = -10;
        hul = 45;
        hur = 435;
        T maxEdgeSpeed;
        T b1 = 0.0;
        T b2 = 0.0;
        T updates[4];
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        bool leftUpdateIsZero = updates[0] == 0 && updates[2] == 0;
        bool rightUpdateIsZero = updates[1] == 0 && updates[3] == 0;
        TS_ASSERT(leftUpdateIsZero || rightUpdateIsZero);
    }

private:

    solver::FWave<T> m_solver;

    T hl;
    T hul;
    T hr;
    T hur;

    void checkIfUpdatesAreApproximatelyZero(const T updates[4]) {

        for (int i = 0; i < 4; i++) {
            TS_ASSERT_LESS_THAN(updates[i], 1);
            TS_ASSERT_LESS_THAN(-1, updates[i]);
        }
    }

    void compareActualToExpectedEigenvalues(T actualEigenvalues[2], T expectedEigenvalues[2]) {
        for (int i = 0; i < 2; i++)
            TS_ASSERT_EQUALS(actualEigenvalues[i], expectedEigenvalues[i]);
    }

    bool areValuesAlmostEqual(T value1, T value2) {
        return fabs(value1 - value2) < 0.0001;
    }

};


#endif	/* _FWAVETEST_H */


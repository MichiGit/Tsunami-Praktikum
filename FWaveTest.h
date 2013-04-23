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

class FWaveTest : public CxxTest::TestSuite
{
public:

    FWaveTest(): b1(0.0), b2(0.0) { }

    void testEigenvalueComputation()
    {
        testSingleEigenvalueComputation(10, 1, 20, 8, -3.9038, 10.7869);
        testSingleEigenvalueComputation(300, 540, -4, 40, -64.152, 64.2255);
        testSingleEigenvalueComputation(64, 1000, -128, -647, -73.1622, 71.3219);
    }

    void testZeroNetUpdates()
    {
        T updates[4];
        hl = hr = 10;
        hul = hur = 5;
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

    void testSupersonicProblems()
    {
        // the components of the wave speed vector will have non opposite signs if hl = -hr
        hl = 10;
        hr = -10;
        hul = 45;
        hur = 435;
        T updates[4];
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        bool leftUpdateIsZero = updates[0] == 0 && updates[2] == 0;
        bool rightUpdateIsZero = updates[1] == 0 && updates[3] == 0;
        TS_ASSERT(leftUpdateIsZero || rightUpdateIsZero);
    }

private:

    void testSingleEigenvalueComputation(const T hl, const T hr, const T hul, const T hur, const T expected1, const T expected2)
    {
        T actualEigenvalues[2];
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT(areValuesAlmostEqual(expected1, actualEigenvalues[0]));
        TS_ASSERT(areValuesAlmostEqual(expected2, actualEigenvalues[1]));
    }

    solver::FWave<T> m_solver;

    T hl;
    T hul;
    T hr;
    T hur;
    T b1;
    T b2;
    T maxEdgeSpeed;

    void singleEigenvalueComputationTest(const T &hl, const T &hr, const T &hul, const T &hur, const T expectedEigenvalues[2], T actualEigenvalues[2])
    {
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[0], actualEigenvalues[0]));
        TS_ASSERT(areValuesAlmostEqual(expectedEigenvalues[1], actualEigenvalues[1]));
    }

    void checkIfUpdatesAreApproximatelyZero(const T updates[4])
    {

        for (int i = 0; i < 4; i++)
        {
            TS_ASSERT_LESS_THAN(updates[i], 1);
            TS_ASSERT_LESS_THAN(-1, updates[i]);
        }
    }

    void compareActualToExpectedEigenvalues(const T actualEigenvalues[2], const T expectedEigenvalues[2])
    {
        for (int i = 0; i < 2; i++)
            TS_ASSERT_EQUALS(actualEigenvalues[i], expectedEigenvalues[i]);
    }

    bool areValuesAlmostEqual(const T value1, const T value2)
    {
        return fabs(value1 - value2) < 0.0001;
    }

};


#endif	/* _FWAVETEST_H */


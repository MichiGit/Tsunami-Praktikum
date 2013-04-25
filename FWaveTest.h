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

    /** \brief calls the testSingleEigenvalueComputation method with different sets of values
     * 
     *  Runs the testSingleEigenvalueComputation method three times.
     *  Every method call gets different parameters.
     *  The expected Eigenvalues were calulated manually.
     * 
     */
    void testEigenvalueComputation()
    {
        testSingleEigenvalueComputation(10, 1, 20, 8, -3.9038, 10.7869);
        testSingleEigenvalueComputation(300, 540, -4, 40, -64.152, 64.2255);
        testSingleEigenvalueComputation(64, 1000, -128, -647, -73.1622, 71.3219);
    }
    
    /** \brief calls the testSingleZeroNetUpdate method with different sets of values
     * 
     * 
     */
    void testZeroNetUpdates()
    {
        testSingleZeroNetUpdate(10.0, 10.0, 5.0, 5.0, 0.0, 0.0);
        testSingleZeroNetUpdate(48.0, 48.0, -39.0, -39.0, 0.0, 0.0);
        testSingleZeroNetUpdate(3000.0, 3000.0, 0.0, 0.0, 0.0, 0.0);
    }
    
    /** \brief calls the testSingleSupersonicProblem method a set of values
     * 
     */
    void testSupersonicProblems()
    {
        testSingleSupersonicProblem(1.0, 1.0, 10.0, 20.0, 0.0, 0.0);
    }

private:

    /** \brief tests if the net updates are approximately zero
     * 
     *  Computes the net updates and then tests if they are with respect to machine precision approximately zero.
     * 
     * @param [in] hl The height of the left water column
     * @param [in] hr The height of the right water column
     * @param [in] hul The space time dependent momentum of the left water column
     * @param [in] hur The space time dependent momentum of the right water column
     * @param [in] b1 The first bathymetry component
     * @param [in] b2 The second bathymetry component
     */
    void testSingleZeroNetUpdate(T hl, T hr, T hul, T hur, T b1, T b2)
    {
        T updates[4] = {0};
        T maxEdgeSpeed;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
    }

    /** \brief tests the correctness for a supersonic problem
     * 
     *  updates RoeEigenvalues, computes the net updates and tests if the supersonic implication, 
     *  that one of the net updates is zero, is true.
     *
     * @param [in] hl The height of the left water column
     * @param [in] hr The height of the right water column
     * @param [in] hul The space time dependent momentum of the left water column
     * @param [in] hur The space time dependent momentum of the right water column
     * @param [in] b1 The first bathymetry component
     * @param [in] b2 The second bathymetry component
     */
    void testSingleSupersonicProblem(T hl, T hr, T hul, T hur, T b1, T b2)
    {
        T updates[4] = {0};
        T maxEdgeSpeed;
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        T roe[2];
        m_solver.getRoeEigenvalues(roe);
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        bool leftUpdateIsZero = updates[0] == 0 && updates[2] == 0;
        bool rightUpdateIsZero = updates[1] == 0 && updates[3] == 0;
        TS_ASSERT(leftUpdateIsZero || rightUpdateIsZero);
        T particleVelocity = m_solver.computeParticleVelocity(hl, hr, hul, hur);
        if (particleVelocity > 0)
        {
            TS_ASSERT(maxEdgeSpeed >= 0);

        }
        else if (particleVelocity < 0)
        {
            TS_ASSERT(maxEdgeSpeed == 0);
        }
    }

    void testSingleEigenvalueComputation(const T hl, const T hr, const T hul, const T hur, const T expected1, const T expected2)
    {
        T actualEigenvalues[2];
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT(areValuesAlmostEqual(expected1, actualEigenvalues[0]));
        TS_ASSERT(areValuesAlmostEqual(expected2, actualEigenvalues[1]));
    }

    solver::FWave<T> m_solver;

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


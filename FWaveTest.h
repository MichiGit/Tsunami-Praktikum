/*
 * File:   FWaveTest.h
 * Author: gutbrod
 *
 * Created on April 18, 2013, 4:52 PM
 */

#ifndef _FWAVETEST_H
#define	_FWAVETEST_H

#include "../types.h"
#include <cxxtest/TestSuite.h>
#include "../scenarios/scenario.h"
#include "../scenarios/rarerare.h"
#include "../scenarios/shockshock.h"
#include "../WavePropagation.h"
#include "../solvers/FWave.hpp"

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
     *  Calls testSingleZeroNetUpdate with values that represent steady states to test
     *  if the net updates will be zero as expected.     *
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
        // requirement: u > sqrt(g*h)
        testSingleSupersonicProblem(1.0, 1.0, 10.0, 20.0, 0.0, 0.0);
    }

    void testShockShockProblems()
    {
        int size = 10;
        int hValue = 0, huValue = 1, expected = 2;
        T **testValues = new T*[3];
        for (int i = 0; i < 3; i++)
            testValues[i] = new T[3];

        testValues[0][hValue] = 8741.6;
        testValues[0][huValue] = 287.7;
        testValues[0][expected] = 8742.5;
        testValues[1][hValue] = 8276.9;
        testValues[1][huValue] = 531.9;
        testValues[1][expected] = 8278.8;
        testValues[2][hValue] = 10483.4;
        testValues[2][huValue] = 984.2;
        testValues[2][expected] = 10486.5;

        scenarios::Scenario<T> *scenario;
        for (int i = 0; i < 3; i++)
        {
            scenario = new scenarios::ShockShock(size, testValues[i][hValue], testValues[i][huValue]);
            testSingleScenario(scenario, size, 1000, testValues[i][expected]);
        }

        delete scenario;
        for (int i = 0; i < 3; i++)
            delete [] testValues[i];
        delete [] testValues;
    }

    void testRareRareProblems()
    {
        int size = 10;
        int hValue = 0, huValue = 1, expected = 2;
        T **testValues = new T*[3];
        for (int i = 0; i < 3; i++)
            testValues[i] = new T[3];

        testValues[0][hValue] = 1387.1;
        testValues[0][huValue] = -101.9;
        testValues[0][expected] = 1386.3;
        testValues[1][hValue] = 6907.4;
        testValues[1][huValue] = -180.6;
        testValues[1][expected] = 6906.7;
        testValues[2][hValue] = 10539.6;
        testValues[2][huValue] = -988.5;
        testValues[2][expected] = 10536.5;

        scenarios::Scenario<T> *scenario;
        for (int i = 0; i < 3; i++)
        {
            scenario = new scenarios::RareRare(size, testValues[i][hValue], testValues[i][huValue]);
            testSingleScenario(scenario, size, 1000, testValues[i][expected]);
        }

        delete scenario;
        for (int i = 0; i < 3; i++)
            delete [] testValues[i];
        delete [] testValues;
    }

    void testComputeParticleVelocity()
    {
        testSingleComputeParticleVelocity(200, 150, -40, -1, -0.11027);
        testSingleComputeParticleVelocity(2, 87, 10, 94, 1.59649);
        testSingleComputeParticleVelocity(112, 34, 127, 209, 2.91481);
    }

    void testComputeFluxDeltaValues()
    {
        T *flux = new T[2];
        T *expected = new T[2];
        expected[0] = 14;
        expected[1] = -86366.41;
        testSingleComputeFluxDeltaValues(200.5, 150.3, 40, 54, expected, flux);
        expected[0] = 21.8;
        expected[1] = -29894.8;
        testSingleComputeFluxDeltaValues(80.7, 20, 21.2, 43, expected, flux);
        expected[0] = 58.5;
        expected[1] = 66169.2;
        testSingleComputeFluxDeltaValues(40, 123, -89, -30.5, expected, flux);
        delete [] flux;
        delete [] expected;
    }

private:

    void testSingleComputeFluxDeltaValues(const T &hl, const T &hr, const T &hul, const T &hur, const T expected[2], T *flux)
    {
        m_solver.computeFluxDeltaValues(hl, hr, hul, hur, flux);
        for (int i = 0; i < 2; i++)
            TS_ASSERT_DELTA(flux[i], expected[i], 0.1);
    }


    void testSingleComputeParticleVelocity(const T &hl, const T &hr, const T &hul, const T &hur, const T &expected)
    {
        T result = m_solver.computeParticleVelocity(hl, hr, hul, hur);
        TS_ASSERT_DELTA(result, expected, 0.0001);
    }

    void testSingleScenario(scenarios::Scenario<T> *scenario, const int size, const int timeSteps, const T expectedValue)
    {
        T *h = new T[size+2];
        T *hu = new T[size+2];
        for (unsigned int j = 0; j < size+2; j++)
        {
            hu[j] = scenario->getMomentum(j);
            h[j] = scenario->getHeight(j);
        }
        WavePropagation wavePropagation(h, hu, size, scenario->getCellSize());
        for (unsigned int j = 0; j < timeSteps; j++)
        {
            wavePropagation.setOutflowBoundaryConditions();
            T maxTimeStep = wavePropagation.computeNumericalFluxes();
            wavePropagation.updateUnknowns(maxTimeStep);
        }
        TS_ASSERT_DELTA(h[size / 2], expectedValue, 0.1);
        TS_ASSERT_DELTA(h[size / 2 + 1], expectedValue, 0.1);
        delete [] h;
        delete [] hu;
    }

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
        T *updates = new T[4]();
        T maxEdgeSpeed;
        m_solver.computeNetUpdates(hl, hr, hul, hur, b1, b2, updates[0], updates[1], updates[2], updates[3], maxEdgeSpeed);
        checkIfUpdatesAreApproximatelyZero(updates);
        delete [] updates;
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
        T *updates = new T[4]();
        T maxEdgeSpeed;
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        T *roe = new T[2]();
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
        delete [] updates;
        delete [] roe;
    }

    /** \brief tests the Eigenvalue Computation
     *
     *  Updates and reads the calculated RoeEigenvalues.
     *  Then it compares it to the ones which were calculated manually and tests if they are approximatly equal.
     *
     * @param [in] hl The height of the left water column
     * @param [in] hr The height of the right water column
     * @param [in] hul The space time dependent momentum of the left water column
     * @param [in] hur The space time dependent momentum of the right water column
     * @param [in] expected1 first expected Eigenvalue
     * @param [in] expected2 second expected Eigenvalue
     */
    void testSingleEigenvalueComputation(const T hl, const T hr, const T hul, const T hur, const T expected1, const T expected2)
    {
        T *actualEigenvalues = new T[2]();
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT_DELTA(expected1, actualEigenvalues[0], 0.0001);
        TS_ASSERT_DELTA(expected2, actualEigenvalues[1], 0.0001);
        delete [] actualEigenvalues;
    }

    solver::FWave<T> m_solver;

    void singleEigenvalueComputationTest(const T &hl, const T &hr, const T &hul, const T &hur, const T expectedEigenvalues[2], T actualEigenvalues[2])
    {
        m_solver.updateRoeEigenvalues(hl, hr, hul, hur);
        m_solver.getRoeEigenvalues(actualEigenvalues);
        TS_ASSERT_DELTA(expectedEigenvalues[0], actualEigenvalues[0], 0.00001);
        TS_ASSERT_DELTA(expectedEigenvalues[1], actualEigenvalues[1], 0.00001);
    }

    /** \brief tests if the given values are approximately zero
     *
     * @param [in] updates
     */
    void checkIfUpdatesAreApproximatelyZero(const T updates[4])
    {
        for (int i = 0; i < 4; i++)
            TS_ASSERT_DELTA(updates[i], 0, 0.00001);
    }
    /** \brief tests if the given actual Eigenvalues equal the expected Eigenvalues
     *
     * @param [in] actualEigenvalues array with the actual Eigenvalues
     * @param [in] expectedEigenvalues array with the expected Eigenvalues
     */
    void compareActualToExpectedEigenvalues(const T actualEigenvalues[2], const T expectedEigenvalues[2])
    {
        for (int i = 0; i < 2; i++)
            TS_ASSERT_EQUALS(actualEigenvalues[i], expectedEigenvalues[i]);
    }

};


#endif	/* _FWAVETEST_H */


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

    }

    void testSupersonicProblems() {

    }

    void setUp() {
        m_solver = new FWave;
    }

    void tearDown() {
        delete m_solver;
    }

private:
    solver::FWave<T> m_solver;


};


#endif	/* _FWAVETEST_H */


/* 
 * File:   FWave.h
 * Author: gutbrod
 *
 * Created on April 18, 2013, 11:38 AM
 */

#ifndef _FWAVE_H
#define	_FWAVE_H



template <class T> class FWave {

public:
    void computeNetUpdates(T hl, T hr, T hul, T hur, T b1, T b2, T hNetUpdatesLeft, T hNetUpdatesRight, T huNetUpdatesLeft, T huNetUpdatesRight,
				T maxEdgeSpeed);

    void computeRoeEigenvalues(T hl, T hr, T hul, T hur, T roe1, T roe2);

    T computeParticleVelocity(T hl, T hr, T hul, T hur);

    void computeFluxFunction(T hl, T hr, T hul, T hur, T f1, T f2);

    void computeEigencoefficients(T roe1, T roe2, T deltaF, T a1, T a2);
};

#endif	/* _FWAVE_H */


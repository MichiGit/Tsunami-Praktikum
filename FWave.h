/* 
 * File:   FWave.h
 * Author: gutbrod
 *
 * Created on April 18, 2013, 11:38 AM
 */

#ifndef _FWAVE_H
#define	_FWAVE_H
#include <math.h>
#define g 9.81

template <class T> class FWave {

public:
    void computeNetUpdates(T hl, T hr, T hul, T hur, T b1, T b2, T hNetUpdatesLeft, T hNetUpdatesRight, T huNetUpdatesLeft, T huNetUpdatesRight,
				T maxEdgeSpeed);

    void computeRoeEigenvalues(T hl, T hr, T hul, T hur, T roe1, T roe2);

    T computeParticleVelocity(T hl, T hr, T hul, T hur);

    void computeFluxFunction(T h, T hu, T f1, T f2);

    void computeEigencoefficients(T hl, T hr, T hul, T hur, T deltaF, T deltaF2, T a1, T a2);
};

template <class T> T FWave<T>::computeParticleVelocity(T hl, T hr, T hul, T hur) {
    T ul = hul / hl;
    T ur = hur / hr;
    T particleVelocity = (ul * sqrt(hl) + ur * sqrt(hr)) / (sqrt(hl) + sqrt(hr));
    return particleVelocity;
}

template <class T> void FWave<T>::computeRoeEigenvalues(T hl, T hr, T hul, T hur, T roe1, T roe2) {
    T pVelocity = computeParticleVelocity(hl, hr, hul, hur);
    T height = 1.0 / 2.0 * (hl + hr);
    T root = sqrt(g * height);
    roe1 = pVelocity - root;
    roe2 = pVelocity + root;
}

template <class T> void FWave<T>::computeFluxFunction(T h, T hu, T f1, T f2) {
    f1 = hu;
    f2 = pow(hu, 2) + 1.0 / 2.0 * g * pow(h, 2);
}

template <class T> void FWave<T>::computeEigencoefficients(T hl, T hr, T hul, T hur, T deltaF1, T deltaF2, T a1, T a2) {
    T a = 1;
    T b = 1;
    T c, d;
    computeRoeEigenvalues(hl, hr, hul, hur, c, d);
    T coefficient = 1.0 / (d - c);
    a1 = coefficient * (d * deltaF1 - b * deltaF2);
    a2 = coefficient * (-c * deltaF1 + a * deltaF2);
}

#endif	/* _FWAVE_H */


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

template <class T> void FWave<T>::computeNetUpdates(T hl, T hr, T hul, T hur, T b1, T b2, T hNetUpdatesLeft, T hNetUpdatesRight, T huNetUpdatesLeft, T huNetUpdatesRight, T maxEdgeSpeed){
    T fr1, fr2;
    computeFluxFunction(hr, hur, fr1, fr2);
    T fl1, fl2;
    computeFluxFunction(hl, hul, fl1, fl2);
    T deltaF1 = fr1 -fl1;
    T deltaF2 = fr2 -fl2;
    T a1, a2;
    computeEigencoefficients(hl, hr, hul, hur, deltaF1, deltaF2, a1, a2);
    T roe1, roe2;
    computeRoeEigenvalues(hl, hr, hul, hur, roe1, roe2);
    T z11 = a1 * roe1;
    T z12 = a1 * roe2;
    T z21 = a2 * roe1;
    T z22 = a2 * roe2;
    T ql1, ql2, qr1, qr2;
    if (roe1 < 0){
        ql1 += z11;
        ql2 += z12;
    } else if(roe1 >  0){
        qr1 += z11;
        qr2 += z12;
    }
    if (roe2 < 0){
        ql1 += z21;
        ql2 += z22;
    } else if (roe2 >  0){
        qr1 += z21;
        qr2 += z22;
    }
    hNetUpdatesLeft = ql1;
    hNetUpdatesRight = qr1;
    huNetUpdatesLeft = ql2;
    huNetUpdatesRight = qr2;
    if(roe1 > 0 && roe2 > 0){
        maxEdgeSpeed = roe2;
    } else if(roe1 < 0 && roe2 < 0){
        maxEdgeSpeed = 0;
    } else if(roe1 >= roe2) {
        maxEdgeSpeed = roe1;
    } else {
        maxEdgeSpeed = roe2;
    }
}

#endif	/* _FWAVE_H */


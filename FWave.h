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
namespace solver {

    template <class T> class FWave {
    public:
        void computeNetUpdates(T hl, T hr, T hul, T hur, T b1, T b2, T hNetUpdatesLeft, T hNetUpdatesRight, T huNetUpdatesLeft, T huNetUpdatesRight,
                T maxEdgeSpeed);

        void computeRoeEigenvalues(T hl, T hr, T hul, T hur, T roe1, T roe2);

        T computeParticleVelocity(T hl, T hr, T hul, T hur);

        void computeFluxValues(T h, T hu, T fluxValues);

        void computeEigencoefficients(T hl, T hr, T hul, T hur, T deltaF, T deltaF2, T a1, T a2);
    };

    /** \brief calculates the particle velocity
     *
     * @param [in] hl The height of the left water column
     * @param [in] hr The height of the right water column
     * @param [in] hul The space time dependent momentum of the left water column
     * @param [in] hur The space time dependent momentum of the right water column
     * @return The particle velocity for the given waves
     */
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

    template <class T> void FWave<T>::computeFluxValues(T h, T hu, T fluxValues) {
        fluxValues[0] = hu;
        fluxValues[1] = pow(hu, 2) + 1.0 / 2.0 * g * pow(h, 2);
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

    /** \brief Computes the net updates and the maxiumum edge speed for a given set of wave quantities and the given bathymetry
     *
     * @param [in] hl The height of the left water column
     * @param [in] hr The height of the right water column
     * @param [in] hul The space time dependent momentum of the left water column
     * @param [in] hur The space time dependent momentum of the right water column
     * @param [in] b1
     * @param [in] b2
     * @param [out] hNetUpdatesLeft
     * @param [out] hNetUpdatesRight
     * @param [out] huNetUpdatesLeft
     * @param [out] huNetUpdatesRight
     * @param [out] maxEdgeSpeed the maximum of the two waves speed values
     *
     */
    template <class T> void FWave<T>::computeNetUpdates(T hl, T hr, T hul, T hur, T b1, T b2, T hNetUpdatesLeft, T hNetUpdatesRight, T huNetUpdatesLeft, T huNetUpdatesRight, T maxEdgeSpeed) {
        T fr[2];
        computeFluxValues(hr, hur, fr);
        T fl[2];
        computeFluxValues(hl, hul, fl);
        T deltaF1 = fr[0] - fl[0];
        T deltaF2 = fr[1] - fl[1];
        T a1, a2;
        computeEigencoefficients(hl, hr, hul, hur, deltaF1, deltaF2, a1, a2);
        T roe1, roe2;
        computeRoeEigenvalues(hl, hr, hul, hur, roe1, roe2);
        T z11 = a1 * roe1;
        T z12 = a1 * roe2;
        T z21 = a2 * roe1;
        T z22 = a2 * roe2;
        T ql1, ql2, qr1, qr2;
        if (roe1 < 0) {
            ql1 += z11;
            ql2 += z12;
        } else if (roe1 > 0) {
            qr1 += z11;
            qr2 += z12;
        }
        if (roe2 < 0) {
            ql1 += z21;
            ql2 += z22;
        } else if (roe2 > 0) {
            qr1 += z21;
            qr2 += z22;
        }
        hNetUpdatesLeft = ql1;
        hNetUpdatesRight = qr1;
        huNetUpdatesLeft = ql2;
        huNetUpdatesRight = qr2;
        if (roe1 > 0 && roe2 > 0) {
            maxEdgeSpeed = roe2;
        } else if (roe1 < 0 && roe2 < 0) {
            maxEdgeSpeed = 0;
        } else if (roe1 >= roe2) {
            maxEdgeSpeed = roe1;
        } else {
            maxEdgeSpeed = roe2;
        }
    }

}

#endif	/* _FWAVE_H */


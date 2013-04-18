/*
 * File:   FWave.h
 * Author: gutbrod
 *
 * Created on April 18, 2013, 11:38 AM
 */

#ifndef _FWAVE_H
#define	_FWAVE_H

#include <math.h>
#include <algorithm>

#define g 9.81
namespace solver {

    template <class T> class FWave {
    public:
        void computeNetUpdates(T hl, T hr, T hul, T hur, T b1, T b2, T hNetUpdatesLeft, T hNetUpdatesRight, T huNetUpdatesLeft, T huNetUpdatesRight,
                T maxEdgeSpeed);

        void computeRoeEigenvalues(T hl, T hr, T hul, T hur, T roe1);

        T computeParticleVelocity(T hl, T hr, T hul, T hur);

        void computeFluxValues(T h, T hu, T fluxValues);

        void computeEigencoefficients(T hl, T hr, T hul, T hur, T fluxDeltaValues, T alpha);

    private:
        void computeFluxDeltaValues(T hl, T hr, T hul, T hur, T fluxDeltaValues);
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

    template <class T> void FWave<T>::computeRoeEigenvalues(T hl, T hr, T hul, T hur, T roe) {
        T pVelocity = computeParticleVelocity(hl, hr, hul, hur);
        T height = 1.0 / 2.0 * (hl + hr);
        T root = sqrt(g * height);
        roe[0] = pVelocity - root;
        roe[1] = pVelocity + root;
    }

    template <class T> void FWave<T>::computeFluxValues(T h, T hu, T fluxValues) {
        fluxValues[0] = hu;
        fluxValues[1] = pow(hu, 2) + 1.0 / 2.0 * g * pow(h, 2);
    }

    template <class T> void FWave<T>::computeEigencoefficients(T hl, T hr, T hul, T hur, T fluxDeltaValues, T alpha) {
        T a = 1;
        T b = 1;
        T c, d;
        computeRoeEigenvalues(hl, hr, hul, hur, c, d);
        T coefficient = 1.0 / (d - c);
        alpha[0] = coefficient * (d * fluxDeltaValues[0] - b * fluxDeltaValues[1]);
        alpha[1] = coefficient * (-c * fluxDeltaValues[0] + a * fluxDeltaValues[1]);
    }

    template <class T> void FWave<T>::computeFluxDeltaValues(T hl, T hr, T hul, T hur, T fluxDeltaValues) {
        T fl[2], fr[2];
        computeFluxValues(hl, hul, fl);
        computeFluxValues(hr, hur, fr);
        fluxDeltaValues[0] = fr[0] - fl[0];
        fluxDeltaValues[1] = fr[1] - fl[1];
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
        T fluxDeltaValues[2];
        computeFluxDeltaValues(hl, hr, hul, hur, fluxDeltaValues);
        T alpha[2];
        computeEigencoefficients(hl, hr, hul, hur, fluxDeltaValues, alpha);
        T roe[2];
        computeRoeEigenvalues(hl, hr, hul, hur, roe);

        T z[2][2];
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                z[i][j] = alpha[i] * roe[j];

        T ql[2], qr[2];
        for (int i = 0; i < 2; i++) {
            if (roe[i] < 0) {
                ql[0] += z[i][0];
                ql[1] += z[i][1];
            }
            else if (roe[i] > 0) {
                qr[0] += z[i][0];
                qr[1] += z[i][1];
            }
        }

        hNetUpdatesLeft = ql[0];
        hNetUpdatesRight = qr[0];
        huNetUpdatesLeft = ql[1];
        huNetUpdatesRight = qr[1];
        if (roe[0] > 0 && roe[1] > 0)
            maxEdgeSpeed = roe[1];
        else if (roe[0] < 0 && roe[1] < 0)
            maxEdgeSpeed = 0;
        else
            std::max(roe[0], roe[1]);
    }

}

#endif	/* _FWAVE_H */


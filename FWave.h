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

        /** \brief Computes the net updates and the maxiumum edge speed for a given set of wave quantities and the given bathymetry
         *
         * @param [in] hl The height of the left water column
         * @param [in] hr The height of the right water column
         * @param [in] hul The space time dependent momentum of the left water column
         * @param [in] hur The space time dependent momentum of the right water column
         * @param [in] bl
         * @param [in] br
         * @param [out] hNetUpdatesLeft
         * @param [out] hNetUpdatesRight
         * @param [out] huNetUpdatesLeft
         * @param [out] huNetUpdatesRight
         * @param [out] maxEdgeSpeed the maximum of the two waves speed values
         *
         */
        void computeNetUpdates(T &hl, T &hr, T &hul, T &hur, T &bl, T &br, T &hNetUpdatesLeft, T &hNetUpdatesRight, T &huNetUpdatesLeft, T &huNetUpdatesRight,
                T &maxEdgeSpeed);

        void computeRoeEigenvalues(const T &hl, const T &hr, const T &hul, const T &hur, T roe[2]);

        /** \brief calculates the particle velocity
         *
         * @param [in] hl The height of the left water column
         * @param [in] hr The height of the right water column
         * @param [in] hul The space time dependent momentum of the left water column
         * @param [in] hur The space time dependent momentum of the right water column
         * @return The particle velocity for the given waves
         */
        T computeParticleVelocity(T hl, T hr, T hul, T hur);

        void computeFluxValues(T h, T hu, T fluxValues);

        void computeEigencoefficients(const T &hl, const T &hr, const T &hul, const T &hur, const T fluxDeltaValues[2], T alpha[2]);

    private:
        void computeFluxDeltaValues(const T &hl, const T &hr, const T &hul, const T &hur, T fluxDeltaValues[2]);
    };

    template <class T> T FWave<T>::computeParticleVelocity(T hl, T hr, T hul, T hur) {
        T ul = hul / hl;
        T ur = hur / hr;
        T particleVelocity = (ul * sqrt(hl) + ur * sqrt(hr)) / (sqrt(hl) + sqrt(hr));
        return particleVelocity;
    }

    template <class T> void FWave<T>::computeRoeEigenvalues(const T &hl, const T &hr, const T &hul, const T &hur, T roe[2]) {
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

    template <class T> void FWave<T>::computeEigencoefficients(const T &hl, const T &hr, const T &hul, const T &hur, const T fluxDeltaValues[2], T alpha[2]) {
        T a = 1, b = 1, c, d;
        computeRoeEigenvalues(hl, hr, hul, hur, c, d);
        T coefficient = 1.0 / (d - c);
        // computing the alpha values by multiplying the inverse of
        // the matrix of right eigenvectors to the jump in the fluxes
        alpha[0] = coefficient * (d * fluxDeltaValues[0] - b * fluxDeltaValues[1]);
        alpha[1] = coefficient * (-c * fluxDeltaValues[0] + a * fluxDeltaValues[1]);
    }

    template <class T> void FWave<T>::computeFluxDeltaValues(const T &hl, const T &hr, const T &hul, const T &hur, T fluxDeltaValues[2]) {
        T fl[2], fr[2];
        computeFluxValues(hl, hul, fl);
        computeFluxValues(hr, hur, fr);
        fluxDeltaValues[0] = fr[0] - fl[0];
        fluxDeltaValues[1] = fr[1] - fl[1];
    }

    template <class T> void FWave<T>::computeNetUpdates(T &hl, T &hr, T &hul, T &hur, T &bl, T &br, T &hNetUpdatesLeft, T &hNetUpdatesRight, T &huNetUpdatesLeft, T &huNetUpdatesRight,
                T &maxEdgeSpeed) {
        T fluxDeltaValues[2];
        computeFluxDeltaValues(hl, hr, hul, hur, fluxDeltaValues);
        T alpha[2];
        computeEigencoefficients(hl, hr, hul, hur, fluxDeltaValues, alpha);
        T roe[2];
        computeRoeEigenvalues(hl, hr, hul, hur, roe);

        // compute the wave vectors
        T z[2][2];
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                z[i][j] = alpha[i] * roe[j];

        // Add the
        T ql[2], qr[2];
        for (int i = 0; i < 2; i++) {
            if (roe[i] < 0) {
                hNetUpdatesLeft += z[i][0];
                huNetUpdatesLeft += z[i][1];
            }
            else if (roe[i] > 0) {
                hNetUpdatesRight += z[i][0];
                huNetUpdatesRight += z[i][1];
            }
        }

        if (roe[0] > 0 && roe[1] > 0)
            maxEdgeSpeed = roe[1];
        else if (roe[0] < 0 && roe[1] < 0)
            maxEdgeSpeed = 0;
        else
            std::max(roe[0], roe[1]);
    }

}

#endif	/* _FWAVE_H */


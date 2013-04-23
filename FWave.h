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
#include <cassert>

#define g 9.81
namespace solver {

    template <class T> class FWave {
    public:

        /** \brief Computes the net updates and the maxiumum edge speed for a given set of parameters.
         *
         * Computes the net updates and the maximum edge speed for a given set of water columns and bathymetry values.
         * The roe eigenvalues, the wave speeds and the wave speeds of both the left and the right water column
         * which are calculated during the computation of the updates can be retrieved via the appropriate getters.
         *
         * @param [in] hl The height of the left water column
         * @param [in] hr The height of the right water column
         * @param [in] hul The space time dependent momentum of the left water column
         * @param [in] hur The space time dependent momentum of the right water column
         * @param [in] bl The first bathymetry component
         * @param [in] br The second bathymetry component
         * @param [out] hNetUpdatesLeft The net update for the height of the left water column
         * @param [out] hNetUpdatesRight The net update for the height of the right water column
         * @param [out] huNetUpdatesLeft The net update for the momentum of the left water column
         * @param [out] huNetUpdatesRight The net update for the momentum of the right water column
         * @param [out] maxEdgeSpeed The maximum of the two waves speed values
         *
         */
        void computeNetUpdates(T &hl, T &hr, T &hul, T &hur, T bl, T br, T &hNetUpdatesLeft, T &hNetUpdatesRight, T &huNetUpdatesLeft, T &huNetUpdatesRight,
                T &maxEdgeSpeed);

        /** \brief Updates the stored roe eigenvalues.
         *
         * Updates the values of the member variables storing the roe eigenvalues for a given set of water columns.
         * The eigenvalues can be accessed using the corresponding getter method.
         *
         * @param [in] hl The height of the left water column
         * @param [in] hr The height of the right water column
         * @param [in] hul The space time dependent momentum of the left water column
         * @param [in] hur The space time dependent momentum of the right water column
         */
        void updateRoeEigenvalues(const T &hl, const T &hr, const T &hul, const T &hur);

        /** \brief calculates the particle velocity
         *
         * @param [in] hl The height of the left water column
         * @param [in] hr The height of the right water column
         * @param [in] hul The space time dependent momentum of the left water column
         * @param [in] hur The space time dependent momentum of the right water column
         * @return The particle velocity for the given waves
         */
        T computeParticleVelocity(const T hl, const T hr, const T hul, const T hur) const;

        /**
         * Computes the values of the flux function for a given water column.
         *
         * @param [in] h The height of the water column
         * @param [in] hu The space time dependent momentum of the water column
         * @param [out] fluxValues The values of the flux function for the given water column
         */
        void computeFluxValues(const T &h, const T &hu, T fluxValues[2]) const;

        /**
         * Computes the eigencoefficients for a given set of water columns and
         * the corresponding jump in the fluxes.
         *
         * @param [in] hl The height of the left water column
         * @param [in] hr The height of the right water column
         * @param [in] hul The space time dependent momentum of the left water column
         * @param [in] hur The space time dependent momentum of the right water column
         * @param [in] fluxDeltaValues The jump in the fluxes
         * @param [out] alpha The eigencofficients (alpha values) for the given input
         */
        void computeEigencoefficients(const T &hl, const T &hr, const T fluxDeltaValues[2], T alpha[2]);

        /**
         * Returns the values of the member variables storing the eigenvalues
         * of a given set of water columns.
         *
         * @param [in] eigenvalues The values of the member variables
         */
        void getRoeEigenvalues(T eigenvalues[2]);

    private:

        T roeEigenvalues[2];

        void computeFluxDeltaValues(const T &hl, const T &hr, const T &hul, const T &hur, T fluxDeltaValues[2]) const;
    };

    template <class T> T FWave<T>::computeParticleVelocity(const T hl, const T hr, const T hul, const T hur) const {
        // we should not divide by zero
        assert(hl != (T)0);
        assert(hr != (T)0);
        T ul = hul / hl;
        T ur = hur / hr;
        assert(sqrt(hl) + sqrt(hr) != (T)0);
        T particleVelocity = (ul * sqrt(hl) + ur * sqrt(hr)) / (sqrt(hl) + sqrt(hr));
        return particleVelocity;
    }

    template <class T> void FWave<T>::updateRoeEigenvalues(const T &hl, const T &hr, const T &hul, const T &hur) {
        // The height should not be negative
        assert(hl >= 0 && hr >= 0);
        T pVelocity = computeParticleVelocity(hl, hr, hul, hur);
        T height = 0.5 * (hl + hr);
        T root = sqrt(g * height);
        roeEigenvalues[0] = pVelocity - root;
        roeEigenvalues[1] = pVelocity + root;
    }

    template <class T> void FWave<T>::computeFluxValues(const T &h, const T &hu, T fluxValues[2]) const {
        // The height should not be negative
        assert(h >= 0);
        fluxValues[0] = hu;
        fluxValues[1] = pow(hu, 2) + 1.0 / 2.0 * g * pow(h, 2);
    }

    template <class T> void FWave<T>::computeEigencoefficients(const T &hl, const T &hr, const T fluxDeltaValues[2], T alpha[2]) {
        // The height should not be negative
        assert(hl >= 0 && hr >= 0);
        T a = 1, b = 1, c = roeEigenvalues[0], d = roeEigenvalues[1];
        // We should not divide by zero
        assert(d - c != (T)0);
        T coefficient = 1.0 / (d - c);
        // computing the alpha values by multiplying the inverse of
        // the matrix of right eigenvectors to the jump in the fluxes
        alpha[0] = coefficient * (d * fluxDeltaValues[0] - b * fluxDeltaValues[1]);
        alpha[1] = coefficient * (-c * fluxDeltaValues[0] + a * fluxDeltaValues[1]);
    }

    template <class T> void FWave<T>::computeFluxDeltaValues(const T &hl, const T &hr, const T &hul, const T &hur, T fluxDeltaValues[2]) const {
        T fl[2], fr[2];
        computeFluxValues(hl, hul, fl);
        computeFluxValues(hr, hur, fr);
        fluxDeltaValues[0] = fr[0] - fl[0];
        fluxDeltaValues[1] = fr[1] - fl[1];
    }

    template <class T> void FWave<T>::getRoeEigenvalues(T eigenvalues[2]) {
       eigenvalues[0] = roeEigenvalues[0];
       eigenvalues[1] = roeEigenvalues[1];
    }

    template <class T> void FWave<T>::computeNetUpdates(T &hl, T &hr, T &hul, T &hur, T bl, T br, T &hNetUpdatesLeft, T &hNetUpdatesRight, T &huNetUpdatesLeft, T &huNetUpdatesRight,
                T &maxEdgeSpeed) {
        // The height should not be negative
        assert(hl >= 0 && hr >= 0);
        updateRoeEigenvalues(hl, hr, hul, hur);
        T fluxDeltaValues[2];
        computeFluxDeltaValues(hl, hr, hul, hur, fluxDeltaValues);
        T alpha[2];
        computeEigencoefficients(hl, hr, fluxDeltaValues, alpha);

        // compute the wave vectors
        T z[2][2];
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                z[i][j] = alpha[i] * roeEigenvalues[j];

        for (int i = 0; i < 2; i++) {
            if (roeEigenvalues[i] < 0) {
                hNetUpdatesLeft += z[i][0];
                huNetUpdatesLeft += z[i][1];
            }
            else if (roeEigenvalues[i] > 0) {
                hNetUpdatesRight += z[i][0];
                huNetUpdatesRight += z[i][1];
            }
        }

        if (roeEigenvalues[0] > 0 && roeEigenvalues[1] > 0)
            maxEdgeSpeed = roeEigenvalues[1];
        else if (roeEigenvalues[0] < 0 && roeEigenvalues[1] < 0)
            maxEdgeSpeed = 0;
        else
            std::max(roeEigenvalues[0], roeEigenvalues[1]);
    }

}

#endif	/* _FWAVE_H */


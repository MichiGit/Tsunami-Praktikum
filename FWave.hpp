/*
 * File:   FWave.h
 * Author: gutbrod
 *
 * Created on April 18, 2013, 11:38 AM
 */

#ifndef _FWAVE_H
#define	_FWAVE_H

#include <cmath>
#include <algorithm>
#include <cassert>

#define g 9.81
namespace solver
{

template <typename T> class FWave
{
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
                           T &maxEdgeSpeed)
    {
        hNetUpdatesLeft = hNetUpdatesRight = huNetUpdatesLeft = huNetUpdatesRight = maxEdgeSpeed = (T)0;
        // The height should not be negative
        assert(hl >= 0 && hr >= 0);
        updateRoeEigenvalues(hl, hr, hul, hur);
        T fluxDeltaValues[2] = {0};
        computeFluxDeltaValues(hl, hr, hul, hur, fluxDeltaValues);
        T alpha[2] = {0};
        computeEigencoefficients(hl, hr, fluxDeltaValues, alpha);

        // compute the wave vectors
        T z[2][2];

        z[0][0] = alpha[0];
        z[0][1] = alpha[0] * roeEigenvalues[0];
        z[1][0] = alpha[1];
        z[1][1] = alpha[1] * roeEigenvalues[1];

        for (int i = 0; i < 2; i++)
        {
            if (roeEigenvalues[i] < 0)
            {
                hNetUpdatesLeft += z[i][0];
                huNetUpdatesLeft += z[i][1];
            }
            else if (roeEigenvalues[i] > 0)
            {
                hNetUpdatesRight += z[i][0];
                huNetUpdatesRight += z[i][1];
            }
        }

        if (roeEigenvalues[0] > 0 && roeEigenvalues[1] > 0)
            maxEdgeSpeed = roeEigenvalues[1];
        else if (roeEigenvalues[0] < 0 && roeEigenvalues[1] < 0)
            maxEdgeSpeed = 0;
        else
            maxEdgeSpeed = std::max(std::fabs(roeEigenvalues[0]), std::fabs(roeEigenvalues[1]));
    }

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
    void updateRoeEigenvalues(const T &hl, const T &hr, const T &hul, const T &hur)
    {
        // The height should not be negative
        assert(hl >= 0 && hr >= 0);
        T pVelocity = computeParticleVelocity(hl, hr, hul, hur);
        T height = 0.5 * (hl + hr);
        T root = sqrt(g * height);
        roeEigenvalues[0] = pVelocity - root;
        roeEigenvalues[1] = pVelocity + root;
    }

    /** \brief calculates the particle velocity
     *
     * @param [in] hl The height of the left water column
     * @param [in] hr The height of the right water column
     * @param [in] hul The space time dependent momentum of the left water column
     * @param [in] hur The space time dependent momentum of the right water column
     * @return The particle velocity for the given waves
     */
    T computeParticleVelocity(const T hl, const T hr, const T hul, const T hur) const
    {
        // we should not divide by zero
        assert(hl != (T)0);
        assert(hr != (T)0);
        T ul = hul / hl;
        T ur = hur / hr;
        assert(sqrt(hl) + sqrt(hr) != (T)0);
        T particleVelocity = (ul * sqrt(hl) + ur * sqrt(hr)) / (sqrt(hl) + sqrt(hr));
        return particleVelocity;
    }

    /**
     * Computes the values of the flux function for a given water column.
     *
     * @param [in] h The height of the water column
     * @param [in] hu The space time dependent momentum of the water column
     * @param [out] fluxValues The values of the flux function for the given water column
     */
    void computeFluxValues(const T &h, const T &hu, T fluxValues[2]) const
    {
        // The height should not be negative
        assert(h >= 0);
        fluxValues[0] = hu;
        fluxValues[1] = h * hu + 0.5 * g * h * h;
    }

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
    void computeEigencoefficients(const T &hl, const T &hr, const T fluxDeltaValues[2], T alpha[2])
    {
        // The height should not be negative
        assert(hl >= 0 && hr >= 0);
        // We should not divide by zero
        assert(roeEigenvalues[1] - roeEigenvalues[0] != (T)0);
        T coefficient = 1.0 / (roeEigenvalues[1] - roeEigenvalues[0]);
        // computing the alpha values by multiplying the inverse of
        // the matrix of right eigenvectors to the jump in the fluxes
        alpha[0] = coefficient * (roeEigenvalues[1] * fluxDeltaValues[0] - fluxDeltaValues[1]);
        alpha[1] = coefficient * (-roeEigenvalues[0] * fluxDeltaValues[0] + fluxDeltaValues[1]);
    }

    /**
     * Returns the values of the member variables storing the eigenvalues
     * of a given set of water columns.
     *
     * @param [in] eigenvalues The values of the member variables
     */
    void getRoeEigenvalues(T eigenvalues[2])
    {
        eigenvalues[0] = roeEigenvalues[0];
        eigenvalues[1] = roeEigenvalues[1];
    }

private:

    T roeEigenvalues[2];

    void computeFluxDeltaValues(const T &hl, const T &hr, const T &hul, const T &hur, T fluxDeltaValues[2]) const
    {
        fluxDeltaValues[0] = hur - hul;
        fluxDeltaValues[1] = hur * (hur / hr) + 0.5 * g * hr * hr - (hul  * (hul / hl)  + 0.5 * g * hl * hl);
    }
};

}

#endif	/* _FWAVE_H */

#ifndef WAGNER_SIMULATION_HH_
#define WAGNER_SIMULATION_HH_

#include "wagner/model.hh"

namespace wagner {

/**
  \brief Wagner simulation

  \param m                  The model to use.
  \param seed               Seed for the random number generator.
  \param t_max              Number of time steps (preferably a power of two).
  \param communities        Number of communities (vertices in the spatial network).
  \param traits             Number of traits per species.
  \param ext_max            Extinction rate.
  \param mig_max            Max migration rate.
  \param aleph
  \param speciation         Speciation rate.
  \param speciation_exp
  \param radius             Threshold distance for the spatial network (higher: more connected landscapes).
  \param white_noise_std    Standard deviation of the white noise applied to all traits.
  \param verbose
  \param shuffle            Shuffle the communties at t / 2.
  \param discard
 */
void simulation(model m, size_t seed, size_t t_max, size_t communities,
                size_t traits, double ext_max, double mig_max,
                double aleph, double speciation, double speciation_exp,
                double radius, float white_noise_std, bool verbose,
                bool shuffle, bool discard) noexcept;

}

#endif

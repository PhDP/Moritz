#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

#include <cstring>
#include <cassert>

#include "wagner/common.hh"
#include "wagner/speciestree.hh"
#include "wagner/point.hh"
#include "wagner/species.hh"
#include "wagner/network.hh"
#include "wagner/n-sphere.hh"
#include "wagner/model.hh"

namespace wagner {

void simulation(model m, size_t seed, size_t t_max, size_t communities,
                size_t traits, double ext_max, double mig_max,
                double aleph, double speciation, double radius,
                float white_noise_std) noexcept {

  std::cout << seed << std::endl;

  const bool has_traits = m == model::euclidean_traits || m == model::fuzzy_traits;

  std::vector<size_t> speciation_per_t;
  std::vector<size_t> ext_per_t;
  std::vector<size_t> species_per_t;

  std::mt19937_64 rng(seed); // The engine
  std::uniform_real_distribution<> unif;
  std::uniform_real_distribution<float> uniff;
  std::normal_distribution<float> noise(0.0f, white_noise_std);

  char buffer[50]; // Yep, for good old C methods :P

  size_t trials = 0; // Attempts to build the spatial networks.
  wagner::network<wagner::point> landscape;
  do {
    if (++trials > 100000) {
      std::cout << "Terminating after 100 000 attempts were made to generate the spatial network.\n";
      return;
    }
    landscape.rgg(communities, radius, rng);
  } while (!landscape.connected());

  std::sprintf(buffer, "w-network-%lu.graphml", seed);
  std::ofstream out_net(buffer);
  out_net << landscape;
  out_net.close();

  std::sprintf(buffer, "w-%lu.xml", seed);
  std::ofstream out_info(buffer);
  out_info << "<wagner>\n";
  out_info << "   <version>" << wagner_version << "</version>\n";
  out_info << "   <revision>" << wagner_revision << "</revision>\n";
  out_info << "   <model>" << m << "</model>\n";
  out_info << "   <master_seed>" << seed << "</master_seed>\n";
  out_info << "   <t_max>" << t_max << "</t_max>\n";
  out_info << "   <communities>" << communities << "</communities>\n";
  out_info << "   <radius>" << radius << "</radius>\n";
  out_info << "   <attempts>" << trials << "</attempts>\n";
  if (has_traits) {
    out_info << "   <num_traits>" << traits << "</num_traits>\n";
    out_info << "   <white_noise_std>" << white_noise_std << "</white_noise_std>\n";
  }
  if (m != model::neutral) {
    out_info << "   <aleph>" << aleph << "</aleph>\n";
  }
  out_info << "   <speciation>" << speciation << "</speciation>\n";
  out_info << "   <migration>" << mig_max << "</migration>\n";
  out_info << "   <extinction>" << ext_max << "</extinction>\n";

  // Where the species are stored:
  wagner::speciestree tree(wagner::random_n_sphere<float>(rng, traits, 0.5f)); // Starts with one species.
  for (auto sp : tree) {
    for (auto p : landscape) {
      sp->add_to(p.first);
    }
  }

  assert(tree.num_species() == 1);

  size_t n_pops = landscape.order();

  ////////////////////////////////
  //        SIMULATIONS         //
  ////////////////////////////////
  size_t t = 0;
  for (; t <= t_max && n_pops != 0; ++t) {
    ////////////////
    // MIGRATION  //
    ////////////////
    for (auto s0 : tree) {
      auto const& presences = s0->get_locations();
      for (auto const& presence : presences) {
        auto const& neighbors = landscape.neighbors(presence.first);
        for (auto const& location : neighbors) {
          if (presences.find(location) == presences.end()) {
            double mig = mig_max;

            // TODO: check maths (seem wrong for traits), handle fuzzy:
            if (m != model::neutral) {
              double delta = 0.0;
              for (auto s1 : tree) {
                if (s1 != s0) {
                  auto const& presences1 = s1->get_locations();
                  if (presences1.find(location) != presences1.end()) {
                    if (m == model::euclidean_traits) {
                      const auto dist = wagner::euclidean_distance(s0->traits(), s1->traits());
                      assert(dist >= 0.0f && dist <= 1.0f);
                      delta += 1.0 - dist;
                    } else if (m == model::phylo_dist) {
                      delta += 1.0 / (t - s0->get_mrca(*s1));
                      break;
                    }
                  }
                }
              }
              mig *= exp(-aleph * delta);
            }

            if (!s0->is_in(location) && unif(rng) < mig) {
              s0->add_to(location);
              ++n_pops;
            }
          }

        }
      }
    }

    ////////////////
    // EXTINCTION //
    ////////////////
    std::binomial_distribution<> binom(n_pops, ext_max);
    size_t extinctions = binom(rng);

    while (extinctions > 0) {
      size_t i = (size_t)(unif(rng) * n_pops);
      size_t j = 0;
      wagner::species *species_to_die = nullptr;
      for (auto s0 : tree) {
        j += s0->size();
        if (i < j) {
          species_to_die = s0;
          break;
        }
      }
      assert(species_to_die != nullptr);
      i = (size_t)(unif(rng) * species_to_die->size());
      j = 0;

      auto const& locations = species_to_die->get_locations();
      for (auto const& location : locations) {
        if (i == j) {
          species_to_die->rmv_from(location.first);
          break;
        } else {
          ++j;
        }
      }
      --n_pops;
      --extinctions;
    }

    ////////////////
    // SPECIATION //
    ////////////////
    size_t n_groups = 0;
    for (auto s0 : tree) {
      n_groups += s0->up_groups(landscape);
    }
    size_t speciation_events = 0;
    if (n_groups > 0) {
      std::binomial_distribution<> binom2(n_groups, speciation);
      speciation_events = binom2(rng);
    }
    speciation_per_t.push_back(speciation_events);
    while (speciation_events > 0) {
      // Select species.
      size_t i = (size_t)(unif(rng) * n_groups);
      size_t j = 0;
      wagner::species *to_speciate = nullptr;
      for (auto s0 : tree) {
        j += s0->num_groups();
        if (i < j) {
          to_speciate = s0;
          break;
        }
      }
      assert(to_speciate != nullptr);
      i = (size_t)(unif(rng) * to_speciate->num_groups());

      // Speciate and get the new species:
      wagner::species *new_species = tree.speciate(to_speciate, t);

      assert(new_species->num_traits() == traits);

      // Transfer populations:
      boost::container::flat_set<wagner::point> to_transfer = to_speciate->pop_group(i);
      new_species->add_to(to_transfer);
      --speciation_events;
    }

    // For all species: white noise
    if (has_traits) {
      for (auto sp : tree) {
        wagner::white_noise(sp->traits(), rng, noise, 0.5f);
      }
    }

    // Epilogue = remove extinct species from the most recent common ancestor
    boost::container::flat_set<wagner::species*> to_rmv = tree.rmv_extinct(t);
    ext_per_t.push_back(to_rmv.size());
    species_per_t.push_back(tree.num_species());

    if (power_of_two(t)) {
      tree.stop(t);
      out_info << "   <newick><t>" << t << "</t>" << tree.newick()
               << "</newick>\n";
      std::sprintf(buffer, "w-species-%lu-t%lu.xml", seed, t);
      std::ofstream out_res(buffer);
      out_res << "<extant_species>\n";
      out_res << "  <t>" << t << "</t>\n";
      for (auto s0 : tree) out_res << "  " << s0->get_info(t) << '\n';
      out_res << "</extant_species>\n";
      out_res.close();
    }
  } // end simulation

  out_info << "   <speciation_per_t> ";
  for (size_t i : speciation_per_t) out_info << i << ' ';
  out_info << "</speciation_per_t>\n   <extinctions_per_t> ";
  for (size_t i : ext_per_t) out_info << i << ' ';
  out_info << "</extinctions_per_t>\n   <species_per_t> ";
  for (size_t i : species_per_t) out_info << i << ' ';
  out_info << "</species_per_t>\n";
  out_info << "</wagner>\n";
  out_info.close();
}

} /* end namespace wagner */
